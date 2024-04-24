//
// Created by djcur on 4/20/2024.
//

#ifndef EVENTTYPES_H
#define EVENTTYPES_H
enum class EventType {
    GridWidthChange,
    GridHeightChange,
    StartSimulation,
    StopSimulation,
    ResetSimulation,
    SimulationCompleted,
    ViewChange,
    TemperatureInputEvent,
    SourceChangeEvent,
    BrushSizeChangeEvent,
    TimeStepSizeChangeEvent,
    TimeDurationChangeEvent,
    MaterialChangeEvent,
    TimeStepChangeEvent,
    ApplyMaterial,
    ApplyInitialTemperature,
    ApplySource,
    ElementsUpdate,
    SimulationTemperatureUpdate,
    SimulationRunning,
    TimeStepCompleted,
    ColorMapChange
};

enum class EventDataKey {
    Width,
    Height,
    ViewMode,
    Temperature,
    Source,
    BrushSize,
    TimeStepSize,
    TimeDuration,
    ActiveTimeStep,
    MaterialName,
    Elements,
    Points,
    SimulationTemperatures,
    ColorMap
};

enum class ViewMode {
    VIEW_INITIAL_TEMPERATURE, VIEW_SOURCES, VIEW_MATERIAL, VIEW_VISUALIZATION
};

std::string inline getEventTypeName(EventType type) {
    switch (type) {
        case EventType::GridWidthChange:
            return "Grid Width Change";
        case EventType::GridHeightChange:
            return "Grid Height Change";
        case EventType::StartSimulation:
            return "Start Simulation";
        case EventType::StopSimulation:
            return "Stop Simulation";
        case EventType::ResetSimulation:
            return "Reset Simulation";
        case EventType::SimulationCompleted:
            return "Simulation Completed";
        case EventType::ViewChange:
            return "View Change";
        case EventType::TemperatureInputEvent:
            return "Temperature Input Event";
        case EventType::SourceChangeEvent:
            return "Source Change Event";
        case EventType::BrushSizeChangeEvent:
            return "Brush Size Change Event";
        case EventType::TimeStepSizeChangeEvent:
            return "Time Step Size Change Event";
        case EventType::TimeDurationChangeEvent:
            return "Time Duration Change Event";
        case EventType::MaterialChangeEvent:
            return "Material Change Event";
        case EventType::TimeStepChangeEvent:
            return "Time Step Change Event";
        case EventType::ApplyMaterial:
            return "Apply Material";
        case EventType::ApplyInitialTemperature:
            return "Apply Initial Temperature";
        case EventType::ApplySource:
            return "Apply Source";
        case EventType::ElementsUpdate:
            return "Elements Update";
        case EventType::SimulationTemperatureUpdate:
            return "Simulation Temperature Update";
        case EventType::SimulationRunning:
            return "Simulation Running";
        default:
            return "Unknown Event Type";
    }
}

#endif //EVENTTYPES_H
