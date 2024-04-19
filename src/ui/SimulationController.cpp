//
// Created by djcur on 4/18/2024.
//

#include "SimulationController.h"

#include <imgui.h>
#include <iostream>

#include "../solver/Solver.h"

SimulationController::SimulationController()
    : simulationRunning(false), simulationComplete(false) {
}

SimulationController::~SimulationController() {
    stopSimulation();
}

void SimulationController::startSimulation(double timeStepSizeSeconds, double targetDurationSeconds) {
    if (simulationRunning) {
        std::cout << "Simulation is already running." << std::endl;
        return;
    }
    simulationRunning.store(true);
    simulationComplete.store(false);
    simulationThread = std::thread(&SimulationController::runSimulation, this, timeStepSizeSeconds, targetDurationSeconds);
}

void SimulationController::stopSimulation() {
    if (!simulationRunning.load()) return;
    simulationRunning.store(false);
    if (simulationThread.joinable()) {
        simulationThread.join();
    }
    simulationComplete.store(true);
    std::cout << "Simulation stopped." << std::endl;
}

bool SimulationController::isSimulationRunning() const {
    return simulationRunning.load();
}

bool SimulationController::isSimulationComplete() const {
    return simulationComplete.load();
}

void SimulationController::runSimulation(double timeStepSizeSeconds, double targetDurationSeconds) {
    std::lock_guard<std::mutex> lock(simulationMutex);
    const auto temp = solver::Solver::getInstance().performSimulation(
                static_cast<int>(targetDurationSeconds / timeStepSizeSeconds), timeStepSizeSeconds);
    simulationTemperatures = temp;
    simulationComplete.store(true);
    simulationRunning.store(false);
    std::cout << "Simulation completed." << std::endl;
}

void SimulationController::updateElementMaterial(const int x, const int y) const {
    solver1.setMaterial(x, y,
                        materialManager.getMaterial(materialManager.getAllMaterialNames()[selectedMaterialIndex]));
}

void SimulationController::updateElementInitialTemperatureCelsius(const int x, const int y) const {
    solver1.setInitialTemperatureKelvin(x, y, selectedTemperatureCelsius);
}

void SimulationController::updateElementSource(const int x, const int y) const {
    solver1.setElementSource(x, y, selectedSource);
}

void SimulationController::updateSolverGrid() const {
    static int currentWidth = gridWidth;
    static int currentHeight = gridHeight;

    if (currentWidth != gridWidth || currentHeight != gridHeight) {
        solver1.updateGridSize(gridWidth, gridHeight);
        currentWidth = gridWidth;
        currentHeight = gridHeight;
    }
}

ImVec4 SimulationController::calculateCellColor(const models::Element &element) {
        float heat = 0.0;
        float normalizedHeat = 0.0; // Declare it once, outside the switch statement
        auto color = models::Color();

        switch (currentView) {
            case VIEW_INITIAL_TEMPERATURE:
                heat = element.getInitialHeatKelvin();
            normalizedHeat = std::max(0.0f, std::min(1.0f, heat / 100));
            return {normalizedHeat, 0.0, 1.0f - normalizedHeat, 1.0};

            case VIEW_SOURCES:
                heat = element.getHeatSource(); // Assuming getHeatSource() was a typo and should be getExternalHeat()
            normalizedHeat = std::max(0.0f, std::min(1.0f, heat / 100));
            return {normalizedHeat, 0.0, 1.0f - normalizedHeat, 1.0};

            case VIEW_MATERIAL:
                color = element.getMaterial().getColor();
            return {color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, 1.0};

            case VIEW_VISUALIZATION:
                heat = getCurrentElementTemperatureKelvin(element);
            normalizedHeat = std::max(0.0f, std::min(1.0f, heat / 100));
            return {normalizedHeat, 0.0, 1.0f - normalizedHeat, 1.0};
            default:
                return {1.0f, 0.0f, 0.0f, 1.0};
        }
    }

    double SimulationController::getCurrentElementTemperatureKelvin(const models::Element &element) {
        std::array<models::Node, 4> nodes = element.getNodes();
        double totalTemperature = 0.0;
        int validNodes = 0;
        std::lock_guard<std::mutex> lock(simulationMutex);
        for (const auto &node: nodes) {
            int globalId = node.getGlobalId();
            if (globalId >= 0 && globalId < simulationTemperatures[currentViewTimeStep].size()) {
                totalTemperature += simulationTemperatures[currentViewTimeStep].coeff(globalId);
                ++validNodes;
            }
        }

        if (validNodes == 0) {
            throw std::runtime_error("No valid nodes found for the given element.");
        }

        return totalTemperature / validNodes;
    }