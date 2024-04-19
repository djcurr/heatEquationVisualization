//
// Created by djcur on 4/18/2024.
//

#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H
#include <mutex>
#include <vector>
#include <Eigen>
#include <imgui.h>

#include 
#include 
#include 
#include 
#include 


class SimulationController {
public:
    SimulationController();
    ~SimulationController();

    void startSimulation(double timeStepSizeSeconds, double targetDurationSeconds);
    void stopSimulation();
    bool isSimulationRunning() const;
    bool isSimulationComplete() const;

private:
    std::atomic<bool> simulationRunning;
    std::atomic<bool> simulationComplete;
    std::thread simulationThread;
    std::mutex simulationMutex;

    // Storage for simulation data
    std::vector<Eigen::VectorXf> simulationTemperatures;

    // Internal methods
    void runSimulation(double timeStepSizeSeconds, double targetDurationSeconds);

    void updateElementMaterial(int x, int y) const;

    void updateElementInitialTemperatureCelsius(int x, int y) const;

    void updateElementSource(int x, int y) const;

    void updateSolverGrid() const;

    double getCurrentElementTemperatureKelvin(const __resharper_unknown_type &element);

    ImVec4 calculateCellColor(const __resharper_unknown_type &element);

    double getCurrentElementTemperatureKelvin(const __resharper_unknown_type &element);

    ImVec4 calculateCellColor(const __resharper_unknown_type &element);

    double getCurrentElementTemperatureKelvin(const __resharper_unknown_type &element);
};



#endif //SIMULATIONCONTROLLER_H
