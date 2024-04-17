//
// Created by djcur on 4/10/2024.
//

#include "Solver.h"
#include <iostream>
#include <chrono>
#include <numeric>

namespace solver {
    // https://github.com/MatthewGeleta/Finite-element-method-for-the-heat-equation/blob/master/FEM_for_heat_equation.m
    void Solver::solveTimeStep1(float timeStep) {
        using namespace std::chrono;

        auto start = high_resolution_clock::now();
        if (!initialized) {
            initializeSystem();
        }
        auto initializedTime = high_resolution_clock::now();
        Eigen::SparseMatrix<float> A = globalMassMatrix + timeStep * globalStiffnessMatrix;
//        auto solver = A.colPivHouseholderQr();
        Eigen::SparseQR<Eigen::SparseMatrix<float>, Eigen::COLAMDOrdering<int>> solver;
        solver.compute(A);
        Eigen::SparseVector<float> b = globalMassMatrix * globalTemperatureVector + timeStep * globalLoadVector;
//        Eigen::VectorXf b_dense = Eigen::VectorXf(globalMassMatrix * globalTemperatureVector + timeStep * globalLoadVector);
//        globalTemperatureVector = solver.solve(b_dense);
        globalTemperatureVector = solver.solve(b);
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(initializedTime - start);
        std::cout << "Initializing matrix took " << duration.count() << " microseconds.\n";
        duration = duration_cast<microseconds>(end - initializedTime);
        std::cout << "Solving matrix took " << duration.count() << " microseconds.\n";
    }

    void Solver::initializeSystem() {
        assembleGlobalSystem();
        computeInitialTemperatures();
        initialized = true;
    }


    void Solver::resetMatrices() {
        int height = grid.getHeight();
        int width = grid.getWidth();
        globalStiffnessMatrix.resize((height + 1) * (width + 1), (height + 1) * (width + 1));
        globalLoadVector.resize((height + 1) * (width + 1));
        globalMassMatrix.resize((height + 1) * (width + 1), (height + 1) * (width + 1));
        globalTemperatureVector.resize((height + 1) * (width + 1));
        globalPreviousTemperatures.resize((height + 1) * (width + 1));
    }

    void Solver::assembleGlobalSystem() {
        std::vector<Eigen::Triplet<double>> stiffnessTriplets, massTriplets;
        for (models::Element element: *grid.getElements()) {
            auto K_local = createStiffnessMatrix(element);
            auto M_local = createMassMatrix(element);
            auto F_local = createLoadVector(element);
            std::array<models::Node, 4> nodes = element.getNodes();

            for (int i = 0; i < nodes.size(); ++i) {
                int node_a_id = nodes[i].getGlobalId();

                for (int j = 0; j < nodes.size(); ++j) {
                    int node_b_id = nodes[j].getGlobalId();
                    stiffnessTriplets.push_back({node_a_id, node_b_id, K_local(i, j)});
                    massTriplets.push_back({node_a_id, node_b_id, M_local(i, j)});
                }

                if (!element.isBoundary()) {
                    globalLoadVector.coeffRef(node_a_id) += F_local(i); // Assuming globalLoadVector is a 1D vector
                }
            }
        }
        globalStiffnessMatrix.setFromTriplets(stiffnessTriplets.begin(), stiffnessTriplets.end());
        globalMassMatrix.setFromTriplets(massTriplets.begin(), massTriplets.end());
    }

    Eigen::Matrix4d Solver::createStiffnessMatrix(models::Element element) {
        Eigen::Matrix4d K = Eigen::Matrix4d::Zero(4, 4);
        double c = element.getMaterial().getThermalConductivity();
        c /= 4.0f;
        double halfC = c / -2.0f;

        K << c, halfC, 0, halfC,
                halfC, c, halfC, 0,
                0, halfC, c, halfC,
                halfC, 0, halfC, c;

        return K;
    }

    Eigen::Vector4d Solver::createLoadVector(models::Element element) {
        Eigen::Vector4d F = Eigen::Vector4d::Zero(4);
        double externalHeat = element.getHeatSource();

        F.setConstant(externalHeat / 4.0f);
        return F;
    }

    Eigen::Matrix4d Solver::createMassMatrix(models::Element element) {
        double rho = element.getMaterial().getDensity(); // Density
        double cp = element.getMaterial().getSpecificHeat(); // Specific heat capacity

        Eigen::Matrix4d M = Eigen::Matrix4d::Zero();

        // The mass matrix for a bilinear square element, integrated exactly
        double factor = rho * cp / 36.0;
        M.diagonal().setConstant(4 * factor);
        M.block<2, 2>(0, 0).array() += factor;
        M.block<2, 2>(2, 2).array() += factor;
        M.topRightCorner<2, 2>().array() += factor;
        M.bottomLeftCorner<2, 2>().array() += factor;

        return M;
    }

    void Solver::computeInitialTemperatures() {
        // Temporary structure to hold temperature contributions for averaging
        std::map<int, std::vector<double>> tempContributions;

        for (const auto &element: *grid.getElements()) {
            double elementInitialTemp = element.getInitialHeatKelvin();
            std::array<models::Node, 4> nodes = element.getNodes();

            for (const auto &node: nodes) {
                int global_i = node.getGlobalId();
                tempContributions[global_i].push_back(elementInitialTemp);
            }
        }

        // Average contributions at each node
        for (auto &[nodeId, temps]: tempContributions) {
            double avgTemp = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();
            globalTemperatureVector.coeffRef(nodeId) = avgTemp; // Assuming indexing starts at 0 or matches node IDs
        }
    }

    void Solver::updateGridSize(int newWidth, int newHeight) {
        grid = models::Grid(newWidth, newHeight);
        initialized = false;
        resetMatrices();
    }

    double Solver::getCurrentElementTemperatureKelvin(const models::Element &element) const {
        std::array<models::Node, 4> nodes = element.getNodes();
        double totalTemperature = 0.0;
        int validNodes = 0;

        for (const auto &node: nodes) {
            int globalId = node.getGlobalId();
            if (globalId >= 0 && globalId < globalTemperatureVector.size()) {
                totalTemperature += globalTemperatureVector.coeff(globalId);
                ++validNodes;
            }
        }

        if (validNodes == 0) {
            throw std::runtime_error("No valid nodes found for the given element.");
        }

        return totalTemperature / validNodes;
    }

} // solver