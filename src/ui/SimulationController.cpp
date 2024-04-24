//
// Created by djcur on 4/18/2024.
//

#include "SimulationController.h"

#include "../solver/Solver.h"

void SimulationController::startSimulation() {
    if (simulationRunning.load()) {
        return;
    }
    broker.publish<events::SimulationRunningEvent>(events::SimulationRunningEvent());
    simulationRunning.store(true);
    simulationThread = ThreadWrapper(std::thread(&SimulationController::runSimulation, this));
}

void SimulationController::runSimulation() {
    std::lock_guard<std::mutex> lock(simulationMutex);
    simulationTemperatures = solver::Solver::getInstance().performSimulation(
        targetDurationSeconds / timeStepSizeSeconds, timeStepSizeSeconds);
    broker.publish<events::ActiveTimeStepChangeEvent>(events::ActiveTimeStepChangeEvent(0));
    simulationRunning.store(false);
    broker.publish<events::SimulationCompletedEvent>(events::SimulationCompletedEvent());
}

void SimulationController::updateGridSize(const int width, const int height) const {
    solver1.updateGridSize(width, height);
    broker.publish<events::ElementsUpdate>(events::ElementsUpdate(solver1.getElements(), width, height));
}

void SimulationController::applyMaterials(const std::vector<models::Point> &points) const {
    for (const auto [x, y]: points) {
        solver1.setMaterial(x, y, selectedMaterialName);
    }
    broker.publish<events::ElementsUpdate>(events::ElementsUpdate(solver1.getElements(), width, height));
}

void SimulationController::applyInitialTemperatures(const std::vector<models::Point> &points) const {
    for (const auto [x, y]: points) {
        solver1.setInitialTemperatureKelvin(x, y, selectedTemperatureCelsius);
    }
    broker.publish<events::ElementsUpdate>(events::ElementsUpdate(solver1.getElements(), width, height));
}

void SimulationController::applySources(const std::vector<models::Point> &points) const {
    for (const auto [x, y]: points) {
        solver1.setElementSource(x, y, selectedSource);
    }
    broker.publish<events::ElementsUpdate>(events::ElementsUpdate(solver1.getElements(), width, height));
}

void SimulationController::onEvent(const std::shared_ptr<events::Event> &event) {
    switch (event->getType()) {
        case EventType::StartSimulation:
            startSimulation();
            break;
        case EventType::ResetSimulation:
            updateGridSize(width, height);
            break;
        case EventType::GridWidthChange:
            width = std::get<int>(event->getData(EventDataKey::Width));
            updateGridSize(width, height);
            break;
        case EventType::GridHeightChange:
            height = std::get<int>(event->getData(EventDataKey::Height));
            updateGridSize(width, height);
            break;
        case EventType::ApplyMaterial: {
            const auto points = std::get<std::vector<models::Point> >(event->getData(EventDataKey::Points));
            applyMaterials(points);
        }
        break;

        case EventType::ApplyInitialTemperature: {
            const auto points = std::get<std::vector<models::Point> >(event->getData(EventDataKey::Points));
            applyInitialTemperatures(points);
        }
        break;

        case EventType::ApplySource: {
            const auto points = std::get<std::vector<models::Point> >(event->getData(EventDataKey::Points));
            applySources(points);
        }
        break;
        case EventType::MaterialChangeEvent:
            selectedMaterialName = std::get<std::string>(event->getData(EventDataKey::MaterialName));
            break;
        case EventType::TemperatureInputEvent:
            selectedTemperatureCelsius = std::get<float>(event->getData(EventDataKey::Temperature));
            break;
        case EventType::SourceChangeEvent:
            selectedSource = std::get<float>(event->getData(EventDataKey::Source));
            break;
        case EventType::TimeDurationChangeEvent:
            targetDurationSeconds = std::get<int>(event->getData(EventDataKey::TimeDuration));
            break;
        case EventType::TimeStepSizeChangeEvent:
            timeStepSizeSeconds = std::get<int>(event->getData(EventDataKey::TimeStepSize));
            break;
        case EventType::TimeStepChangeEvent: {
            // std::lock_guard<std::mutex> lock(simulationMutex);
            activeTimeStep = std::get<int>(event->getData(EventDataKey::ActiveTimeStep));
            broker.publish<events::SimulationTemperatureUpdate>(simulationTemperatures[activeTimeStep]);
            break;
        }
        default:
            break;
    }
}
