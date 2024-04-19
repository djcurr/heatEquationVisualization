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
        static Solver &getInstance(int width = 50, int height = 50) {
            static Solver instance(width, height);
            return instance;
        }

        Solver(Solver const &) = delete;

        void operator=(Solver const &) = delete;

        std::vector<Eigen::VectorXf> performSimulation(int numTimesteps, float timeStep);

        const std::vector<models::Element> *getElements() const { return grid.getElements(); }

        const int getGridWidth() const { return grid.getWidth(); }

        const int getGridHeight() const { return grid.getHeight(); }

        void updateGridSize(int newWidth, int newHeight);

        void setInitialTemperatureKelvin(int x, int y, float temperature) { grid.setInitialTemperature(x, y, temperature); resetMatrices(); }

        void setElementSource(int x, int y, float source) { grid.setElementSource(x, y, source); resetMatrices(); }

        void setMaterial(int x, int y, models::Material material) { grid.setMaterial(x, y, material); resetMatrices(); }

        int getTimeStepsCompleted() const { return timeStepsCompleted.load(); }

    private:
        Solver::Solver(int width, int height) : globalStiffnessMatrix((height + 1) * (width + 1), (height + 1) * (width + 1)),
                                                globalLoadVector((height + 1) * (width + 1)),
                                                globalMassMatrix((height + 1) * (width + 1), (height + 1) * (width + 1)),
                                                globalTemperatureVector((height + 1) * (width + 1)),
                                                grid(width, height) {}

        Eigen::SparseMatrix<float> globalStiffnessMatrix;
        Eigen::SparseVector<float> globalLoadVector;
        Eigen::SparseMatrix<float> globalMassMatrix;
        Eigen::VectorXf globalTemperatureVector;

        bool initialized = false;

        std::atomic_int timeStepsCompleted = 0;

        models::Grid grid;

        Eigen::VectorXf solveTimeStep(float timeStep);

        void assembleGlobalSystem();

        static Eigen::Matrix4d createStiffnessMatrix(const models::Element &element);

        static Eigen::Vector4d createLoadVector(const models::Element &element);

        static Eigen::Matrix4d createMassMatrix(const models::Element &element);

        void Solver::initializeSystem();

        void computeInitialTemperatures();

        void resetMatrices();
    };

} // solver

#endif //HEATEQUATIONVISUALIZATION_SOLVER_H
