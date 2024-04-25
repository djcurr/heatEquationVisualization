//
// Created by djcur on 4/10/2024.
//

#include "HeatSolver.h"
#include <future>
#include <numeric>
#include <thread>

#include "../events/ISubscriber.h"
#include "../threads/ThreadPool.h"

namespace solver {
    // https://github.com/MatthewGeleta/Finite-element-method-for-the-heat-equation/blob/master/FEM_for_heat_equation.m
    Eigen::VectorXd HeatSolver::solveTimeStep(const float timeStep) {
        Eigen::BiCGSTAB<Eigen::SparseMatrix<double> > solver;

        const Eigen::SparseVector<double> b = globalMassMatrix * globalTemperatureVector + timeStep * globalLoadVector;
        const Eigen::SparseMatrix<double> A = globalMassMatrix + timeStep * globalStiffnessMatrix;

        solver.compute(A);
        Eigen::VectorXd newTemperature = solver.solve(b);
        broker.publish<events::TimeStepCompleted>(events::TimeStepCompleted());

        return newTemperature;
    }

    std::vector<Eigen::VectorXd> HeatSolver::performSimulation(const int numTimesteps, const int timeStep) {
        threads::ThreadPool pool(std::thread::hardware_concurrency() - 2);
        std::vector<std::future<Eigen::VectorXd> > futures;
        std::vector<Eigen::VectorXd> results;
        if (!initialized) {
            initializeSystem();
        }
        futures.reserve(numTimesteps);
        for (int i = 0; i < numTimesteps; ++i) {
            const auto selectedTs = static_cast<float>(timeStep * i);
            futures.emplace_back(
                pool.enqueue([this, selectedTs] {
                    return solveTimeStep(selectedTs);
                })
            );
        }

        // Wait for all results to be available
        results.reserve(futures.size());
        for (auto &future: futures) {
            results.emplace_back(future.get());
        }
        return results;
    }

    void HeatSolver::initializeSystem() {
        assembleGlobalSystem();
        computeInitialTemperatures();
        initialized = true;
    }


    void HeatSolver::resetMatrices() {
        int height = grid.getHeight();
        int width = grid.getWidth();
        globalStiffnessMatrix.resize((height + 1) * (width + 1), (height + 1) * (width + 1));
        globalLoadVector.resize((height + 1) * (width + 1));
        globalMassMatrix.resize((height + 1) * (width + 1), (height + 1) * (width + 1));
        globalTemperatureVector.resize((height + 1) * (width + 1));
        initialized = false;
    }

    void HeatSolver::assembleGlobalSystem() {
        std::vector<Eigen::Triplet<double> > stiffnessTriplets, massTriplets;
        for (models::Element element: *grid.getElements()) {
            auto K_local = createStiffnessMatrix(element);
            auto M_local = createMassMatrix(element);
            auto F_local = createLoadVector(element);
            std::array<models::Node, 4> nodes = element.getNodes();

            for (int i = 0; i < nodes.size(); ++i) {
                int node_a_id = nodes[i].getGlobalId();

                for (int j = 0; j < nodes.size(); ++j) {
                    int node_b_id = nodes[j].getGlobalId();
                    stiffnessTriplets.emplace_back(node_a_id, node_b_id, K_local(i, j));
                    massTriplets.emplace_back(node_a_id, node_b_id, M_local(i, j));
                }

                if (!element.isBoundary()) {
                    globalLoadVector.coeffRef(node_a_id) += F_local(i); // Assuming globalLoadVector is a 1D vector
                }
            }
        }
        globalStiffnessMatrix.setFromTriplets(stiffnessTriplets.begin(), stiffnessTriplets.end());
        globalMassMatrix.setFromTriplets(massTriplets.begin(), massTriplets.end());
    }

    Eigen::Matrix4d HeatSolver::createStiffnessMatrix(const models::Element &element) {
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

    Eigen::Vector4d HeatSolver::createLoadVector(const models::Element &element) {
        Eigen::Vector4d F = Eigen::Vector4d::Zero(4);
        double externalHeat = element.getHeatSource();

        F.setConstant(externalHeat / 4.0f);
        return F;
    }

    Eigen::Matrix4d HeatSolver::createMassMatrix(const models::Element &element) {
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

    void HeatSolver::computeInitialTemperatures() {
        // Temporary structure to hold temperature contributions for averaging
        std::map<int, std::vector<double> > tempContributions;

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
            const double avgTemp = std::accumulate(temps.begin(), temps.end(), 0.0) / static_cast<double>(temps.size());
            globalTemperatureVector.coeffRef(nodeId) = avgTemp; // Assuming indexing starts at 0 or matches node IDs
        }
    }

    void HeatSolver::updateGridSize(const int newWidth, const int newHeight) {
        grid = models::Grid(newWidth, newHeight);
        resetMatrices();
    }
} // solver
