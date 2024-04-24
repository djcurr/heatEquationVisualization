//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_SOLVER_H
#define HEATEQUATIONVISUALIZATION_SOLVER_H

#include "../config/Config.h"
#include "../events/Broker.h"
#include "../models/Element.h"
#include "../models/Grid.h"

namespace solver {
    class Solver {
    public:
        static Solver &getInstance() {
            static Solver instance(config::Config::config.gridWidth, config::Config::config.gridHeight);
            return instance;
        }

        Solver(Solver const &) = delete;

        void operator=(Solver const &) = delete;

        std::vector<Eigen::VectorXd> performSimulation(int numTimesteps, int timeStep);

        std::vector<models::Element> getElements() const { return *grid.getElements(); }

        void updateGridSize(int newWidth, int newHeight);

        void setInitialTemperatureKelvin(int x, int y, float temperature) {
            grid.setInitialTemperature(x, y, temperature);
            resetMatrices();
        }

        void setElementSource(int x, int y, float source) {
            grid.setElementSource(x, y, source);
            resetMatrices();
        }

        void setMaterial(int x, int y, const std::string &materialName) {
            grid.setMaterial(x, y, material_manager.getMaterial(materialName));
            resetMatrices();
        }

        int getTimeStepsCompleted() const { return timeStepsCompleted.load(); }

    private:
        Solver::Solver(const int width, const int height) : globalStiffnessMatrix((height + 1) * (width + 1),
                                                                                  (height + 1) * (width + 1)),
                                                            globalLoadVector((height + 1) * (width + 1)),
                                                            globalMassMatrix((height + 1) * (width + 1),
                                                                             (height + 1) * (width + 1)),
                                                            globalTemperatureVector((height + 1) * (width + 1)),
                                                            grid(width, height) {
        }

        Eigen::SparseMatrix<double> globalStiffnessMatrix;
        Eigen::SparseVector<double> globalLoadVector;
        Eigen::SparseMatrix<double> globalMassMatrix;
        Eigen::VectorXd globalTemperatureVector;
        events::Broker &broker = events::Broker::getInstance();

        bool initialized = false;

        manager::MaterialManager& material_manager = manager::MaterialManager::getInstance();

        std::atomic<int> timeStepsCompleted = 0;

        models::Grid grid;

        Eigen::VectorXd solveTimeStep(float timeStep);

        void assembleGlobalSystem();

        static Eigen::Matrix4d createStiffnessMatrix(const models::Element &element);

        static Eigen::Vector4d createLoadVector(const models::Element &element);

        static Eigen::Matrix4d createMassMatrix(const models::Element &element);

        void initializeSystem();

        void computeInitialTemperatures();

        void resetMatrices();
    };
} // solver

#endif //HEATEQUATIONVISUALIZATION_SOLVER_H
