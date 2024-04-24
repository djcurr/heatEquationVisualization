//
// Created by djcur on 4/18/2024.
//

#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H
#include <mutex>
#include <vector>

#include "../events/ISubscriber.h"
#include "../solver/Solver.h"
#include "../events/Broker.h"
#include "../threads/ThreadWrapper.h"


class SimulationController final : public events::ISubscriber {
public:
    SimulationController()
    : simulationRunning(false) {
        // broker.publish<events::ElementsUpdate>(events::ElementsUpdate(solver1.getElements()));
    }

private:
    std::atomic<bool> simulationRunning;
    std::atomic<bool> simulationComplete;
    ThreadWrapper simulationThread;
    std::mutex simulationMutex;
    int timeStepSizeSeconds = config::Config::config.timeStepSizeSeconds;
    int targetDurationSeconds = config::Config::config.targetDurationSeconds;

    // Storage for simulation data
    solver::Solver& solver1 = solver::Solver::getInstance();
    int width = config::Config::config.gridWidth;
    int height = config::Config::config.gridHeight;
    std::string selectedMaterialName = config::Config::config.materialName;
    float selectedTemperatureCelsius = 0;
    float selectedSource = 0;
    std::vector<Eigen::VectorXd> simulationTemperatures;
    int activeTimeStep = 0;
    events::Broker &broker = events::Broker::getInstance();

    // Internal methods
    void startSimulation();

    void runSimulation();

    void onEvent(const std::shared_ptr<events::Event>& event) override;

    void applyMaterials(const std::vector<models::Point>& points) const;

    void applyInitialTemperatures(const std::vector<models::Point>& points) const;

    void applySources(const std::vector<models::Point>& points) const;

    void updateGridSize(int width, int height) const;
};


#endif //SIMULATIONCONTROLLER_H
