//
// Created by djcur on 4/18/2024.
//

#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include <mutex>
#include <vector>
#include <atomic>
#include <memory>

#include "../events/ISubscriber.h"
#include "../solver/HeatSolver.h"
#include "../events/Broker.h"
#include "../threads/ThreadWrapper.h"

/**
 * @class SimulationController
 * @brief Manages the execution and state of simulations, coordinating events and updates.
 *
 * This class is responsible for managing the overall simulation process, including starting,
 * stopping, and updating the simulation according to user inputs or predefined scenarios.
 * It acts as an event subscriber to respond to various simulation events and control the
 * simulation accordingly.
 */
class SimulationController final : public events::ISubscriber {
public:
    /**
     * @brief Constructor for the SimulationController.
     */
 SimulationController() : simulationRunning(false) {}

private:
    std::atomic<bool> simulationRunning; ///< Flag indicating if the simulation is currently running.
    std::atomic<bool> simulationComplete; ///< Flag indicating if the simulation has completed.
    ThreadWrapper simulationThread; ///< Wrapper for the simulation thread to ensure proper joining.
    std::mutex simulationMutex; ///< Mutex to synchronize access to the simulation data.

    // Configuration values pulled from the global configuration
    int timeStepSizeSeconds = config::Config::config.timeStepSizeSeconds; ///< Time step size in seconds.
    int targetDurationSeconds = config::Config::config.targetDurationSeconds; ///< Total duration of the simulation in seconds.

    solver::HeatSolver& solver1 = solver::HeatSolver::getInstance(); ///< Reference to the heat solver.
    int width = config::Config::config.gridWidth; ///< Width of the grid being simulated.
    int height = config::Config::config.gridHeight; ///< Height of the grid being simulated.
    std::string selectedMaterialName = config::Config::config.materialName; ///< Selected material for simulation.
    float selectedTemperatureCelsius = 0; ///< Selected initial temperature in Celsius.
    float selectedSource = 0; ///< Selected heat source intensity.
    std::vector<Eigen::VectorXd> simulationTemperatures; ///< Temperatures recorded during the simulation.
    int activeTimeStep = 0; ///< Current active timestep in the simulation.
    events::Broker &broker = events::Broker::getInstance(); ///< Broker for publishing and subscribing to events.

    /**
     * @brief Starts the simulation on a new thread.
     */
    void startSimulation();

    /**
     * @brief Main function that runs the simulation.
     */
    void runSimulation();

    /**
     * @brief Handles incoming events that affect the simulation.
     * @param event The event to handle.
     */
    void onEvent(const std::shared_ptr<events::Event>& event) override;

    /**
     * @brief Applies material properties to specified points on the grid.
     * @param points Points where the material should be applied.
     */
    void applyMaterials(const std::vector<models::Point>& points) const;

    /**
     * @brief Applies initial temperatures to specified points on the grid.
     * @param points Points where the temperature should be set.
     */
    void applyInitialTemperatures(const std::vector<models::Point>& points) const;

    /**
     * @brief Applies source intensities to specified points on the grid.
     * @param points Points where the source should be applied.
     */
    void applySources(const std::vector<models::Point>& points) const;

    /**
     * @brief Updates the size of the grid being simulated.
     * @param width New width of the grid.
     * @param height New height of the grid.
     */
    void updateGridSize(int width, int height) const;

    /**
     * @brief Resets the grid to its initial state.
     */
    void resetGrid() const;
};

#endif //SIMULATIONCONTROLLER_H
