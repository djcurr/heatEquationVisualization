//
// Created by djcur on 4/20/2024.
//

#ifndef EVENTTYPES_H
#define EVENTTYPES_H

/**
 * @enum EventType
 * @brief Enum class representing different types of events that can be triggered within the system.
 */
enum class EventType {
    GridWidthChange,               ///< Event triggered when the grid width is changed.
    GridHeightChange,              ///< Event triggered when the grid height is changed.
    StartSimulation,               ///< Event to start the simulation process.
    StopSimulation,                ///< Event to stop the simulation process.
    ResetSimulation,               ///< Event to reset the simulation to its initial state.
    SimulationCompleted,           ///< Event signaling the completion of a simulation.
    ViewChange,                    ///< Event signaling a change in the viewing mode of the UI.
    TemperatureInputEvent,         ///< Event for inputting a temperature value.
    SourceChangeEvent,             ///< Event for changing the heat source parameters.
    BrushSizeChangeEvent,          ///< Event for changing the size of a brush used in the UI.
    TimeStepSizeChangeEvent,       ///< Event for changing the size of a simulation timestep.
    TimeDurationChangeEvent,       ///< Event for changing the overall duration of the simulation.
    MaterialChangeEvent,           ///< Event for changing the material properties in the simulation.
    TimeStepChangeEvent,           ///< Event for changing the currently active timestep.
    ApplyMaterial,                 ///< Event to apply material settings to the grid.
    ApplyInitialTemperature,       ///< Event to set initial temperature values on the grid.
    ApplySource,                   ///< Event to apply a heat source to specific points on the grid.
    ElementsUpdate,                ///< Event to update elements on the grid.
    SimulationTemperatureUpdate,   ///< Event to update the temperatures during a simulation.
    SimulationRunning,             ///< Event indicating that the simulation is currently running.
    TimeStepCompleted,             ///< Event signaling the completion of a specific timestep.
    ColorMapChange                 ///< Event for changing the color map used in the UI.
};

/**
 * @enum EventDataKey
 * @brief Enum class representing keys used to access data within an event.
 */
enum class EventDataKey {
    Width,                         ///< Key for accessing grid width data.
    Height,                        ///< Key for accessing grid height data.
    ViewMode,                      ///< Key for accessing the current view mode.
    Temperature,                   ///< Key for accessing temperature data.
    Source,                        ///< Key for accessing heat source data.
    BrushSize,                     ///< Key for accessing brush size data.
    TimeStepSize,                  ///< Key for accessing timestep size data.
    TimeDuration,                  ///< Key for accessing the duration of the simulation.
    ActiveTimeStep,                ///< Key for accessing the active timestep data.
    MaterialName,                  ///< Key for accessing the name of the material used.
    Elements,                      ///< Key for accessing grid elements data.
    Points,                        ///< Key for accessing grid points data.
    SimulationTemperatures,        ///< Key for accessing simulation temperature data.
    ColorMap                       ///< Key for accessing the color map data.
};

/**
 * @enum ViewMode
 * @brief Enum class representing different view modes available in the UI.
 */
enum class ViewMode {
    VIEW_INITIAL_TEMPERATURE,      ///< View mode for displaying initial temperatures.
    VIEW_SOURCES,                  ///< View mode for displaying heat sources.
    VIEW_MATERIAL,                 ///< View mode for displaying material properties.
    VIEW_VISUALIZATION             ///< View mode for general visualization.
};

/**
 * @brief Returns the string representation of an EventType.
 * @param type The EventType whose name is to be retrieved.
 * @return A string representing the name of the EventType.
 */
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
