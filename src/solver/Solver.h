//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_SOLVER_H
#define HEATEQUATIONVISUALIZATION_SOLVER_H

#include "Eigen"
#include "../models/Node.h"
#include "../models/Element.h"
#include "../models/Grid.h"

namespace solver {

    class Solver {
    public:
        static Solver &getInstance(int width = 10, int height = 10) {
            static Solver instance(width, height);
            return instance;
        }

        Solver(Solver const &) = delete;

        void operator=(Solver const &) = delete;

        void solveTimeStep1(float timeStep);

        const std::vector<models::Element> *getElements() const { return grid.getElements(); }

        const int getGridWidth() const { return grid.getWidth(); }

        const int getGridHeight() const { return grid.getHeight(); }

        void updateGridSize(int newWidth, int newHeight);

        void setInitialTemperatureKelvin(int x, int y, float temperature) { grid.setInitialTemperature(x, y, temperature); resetMatrices(); initialized = false; }

        void setElementSource(int x, int y, float source) { grid.setElementSource(x, y, source); resetMatrices(); initialized = false; }

        void setMaterial(int x, int y, models::Material material) { grid.setMaterial(x, y, material); resetMatrices(); initialized = false; }

        void resetTemperature();

        double getCurrentElementTemperatureKelvin(const models::Element &element) const;

    private:
        Solver::Solver(int width, int height) : globalStiffnessMatrix((height + 1) * (width + 1), (height + 1) * (width + 1)),
                                                globalLoadVector((height + 1) * (width + 1)),
                                                globalMassMatrix((height + 1) * (width + 1), (height + 1) * (width + 1)),
                                                globalTemperatureVector((height + 1) * (width + 1)),
                                                globalPreviousTemperatures((height + 1) * (width + 1)),
                                                grid(width, height) {}

        Eigen::SparseMatrix<float> globalStiffnessMatrix;
        Eigen::SparseVector<float> globalLoadVector;
        Eigen::SparseMatrix<float> globalMassMatrix;
        Eigen::VectorXf globalTemperatureVector;
        Eigen::VectorXf globalPreviousTemperatures;

        bool initialized = false;

        models::Grid grid;

        void assembleGlobalSystem();

        Eigen::Matrix4d createStiffnessMatrix(models::Element element);

        Eigen::Vector4d createLoadVector(models::Element element);

        Eigen::Matrix4d createMassMatrix(models::Element element);

        void Solver::initializeSystem();

        void computeInitialTemperatures();

        void resetMatrices();
    };

} // solver

#endif //HEATEQUATIONVISUALIZATION_SOLVER_H
