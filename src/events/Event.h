//
// Created by djcur on 4/20/2024.
//

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <unordered_map>
#include <variant>
#include <Eigen/Eigen>

#include "EventTypes.h"
#include "../models/Point.h"
#include "../ui/ColorMap.h"
#include "../models/Element.h"

namespace models {
    class Element;
}

/** @namespace events
 *  @brief Encapsulates event-related functionalities within the system.
 */
namespace events {

    /**
     * @brief Alias for storing different types of event data.
     */
    using EventData = std::variant<int, float, ViewMode, std::string, std::vector<models::Element>, Eigen::VectorXd, std::vector<models::Point>, ColorMaps>;

    /**
     * @class Event
     * @brief Abstract base class for all events in the application.
     *
     * This class provides the interface for all events, ensuring that they can be identified and data can be accessed uniformly.
     */
    class Event {
    public:
        virtual ~Event() = default;

        /**
         * @brief Retrieve the event type.
         * @return EventType representing the type of the event.
         */
        [[nodiscard]] virtual EventType getType() const = 0;

        /**
         * @brief Access data associated with a specific key in the event.
         * @param key The key corresponding to the data item.
         * @return The data item associated with the key, or an empty variant if the key is not found.
         */
        [[nodiscard]] virtual EventData getData(const EventDataKey &key) const {
            if (const auto it = data.find(key); it != data.end()) {
                return it->second;
            }
            return {}; // Return empty variant if key not found
        }

    protected:
        std::unordered_map<EventDataKey, EventData> data; /**< Map storing event data keyed by EventDataKey. */
    };

    // Define individual event types below:

    /**
     * @class GridWidthChangeEvent
     * @brief Event triggered when the grid width is changed.
     */
    class GridWidthChangeEvent final : public Event {
    public:
        explicit GridWidthChangeEvent(int w) {
            data[EventDataKey::Width] = w;
        }
        [[nodiscard]] EventType getType() const override { return EventType::GridWidthChange; }
    };

    /**
     * @class GridHeightChangeEvent
     * @brief Event triggered when the grid height is changed.
     */
    class GridHeightChangeEvent final : public Event {
    public:
        explicit GridHeightChangeEvent(int h) {
            data[EventDataKey::Height] = h;
        }
        [[nodiscard]] EventType getType() const override { return EventType::GridHeightChange; }
    };

    /**
     * @class StartSimulationEvent
     * @brief Event triggered to start the simulation process.
     */
    class StartSimulationEvent final : public Event {
    public:
        StartSimulationEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::StartSimulation; }
    };

    /**
     * @class StopSimulationEvent
     * @brief Event triggered to stop the simulation process.
     */
    class StopSimulationEvent final : public Event {
    public:
        StopSimulationEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::StopSimulation; }
    };

    /**
     * @class SimulationCompletedEvent
     * @brief Event triggered when the simulation completes.
     */
    class SimulationCompletedEvent final : public Event {
    public:
        SimulationCompletedEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::SimulationCompleted; }
    };

    /**
     * @class SimulationRunningEvent
     * @brief Event indicating that the simulation is currently running.
     */
    class SimulationRunningEvent final : public Event {
    public:
        SimulationRunningEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::SimulationRunning; }
    };

    /**
     * @class ResetSimulationEvent
     * @brief Event triggered to reset the simulation to its initial state.
     */
    class ResetSimulationEvent final : public Event {
    public:
        ResetSimulationEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::ResetSimulation; }
    };

    /**
     * @class TimeStepCompleted
     * @brief Event triggered when a single timestep of the simulation completes.
     */
    class TimeStepCompleted final : public Event {
    public:
        TimeStepCompleted() = default;
        [[nodiscard]] EventType getType() const override { return EventType::TimeStepCompleted; }
    };

    /**
     * @class ViewChangeEvent
     * @brief Event triggered to change the current view mode of the simulation UI.
     */
    class ViewChangeEvent final : public Event {
    public:
        explicit ViewChangeEvent(ViewMode newView) {
            data[EventDataKey::ViewMode] = newView;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ViewChange; }
    };

    /**
     * @class TemperatureInputEvent
     * @brief Event triggered when a new temperature input is received.
     */
    class TemperatureInputEvent final : public Event {
    public:
        explicit TemperatureInputEvent(float temperature) {
            data[EventDataKey::Temperature] = temperature;
        }
        [[nodiscard]] EventType getType() const override { return EventType::TemperatureInputEvent; }
    };

    /**
     * @class SourceChangeEvent
     * @brief Event triggered when the source parameter is changed.
     */
    class SourceChangeEvent final : public Event {
    public:
        explicit SourceChangeEvent(float source) {
            data[EventDataKey::Source] = source;
        }
        [[nodiscard]] EventType getType() const override { return EventType::SourceChangeEvent; }
    };

    /**
     * @class MaterialChangeEvent
     * @brief Event triggered when the material of the simulation is changed.
     */
    class MaterialChangeEvent final : public Event {
    public:
        explicit MaterialChangeEvent(const std::string &materialName) {
            data[EventDataKey::MaterialName] = materialName;
        }
        [[nodiscard]] EventType getType() const override { return EventType::MaterialChangeEvent; }
    };

    /**
     * @class BrushSizeChangeEvent
     * @brief Event triggered when the brush size used in the UI is changed.
     */
    class BrushSizeChangeEvent final : public Event {
    public:
        explicit BrushSizeChangeEvent(int brushSize) {
            data[EventDataKey::BrushSize] = brushSize;
        }
        [[nodiscard]] EventType getType() const override { return EventType::BrushSizeChangeEvent; }
    };

    /**
     * @class TimeStepSizeChangeEvent
     * @brief Event triggered when the timestep size of the simulation is changed.
     */
    class TimeStepSizeChangeEvent final : public Event {
    public:
        explicit TimeStepSizeChangeEvent(int timestepSize) {
            data[EventDataKey::TimeStepSize] = timestepSize;
        }
        [[nodiscard]] EventType getType() const override { return EventType::TimeStepSizeChangeEvent; }
    };

    /**
     * @class TimeDurationChangeEvent
     * @brief Event triggered when the total duration of the simulation is changed.
     */
    class TimeDurationChangeEvent final : public Event {
    public:
        explicit TimeDurationChangeEvent(int duration) {
            data[EventDataKey::TimeDuration] = duration;
        }
        [[nodiscard]] EventType getType() const override { return EventType::TimeDurationChangeEvent; }
    };

    /**
     * @class ActiveTimeStepChangeEvent
     * @brief Event triggered when the active timestep of the simulation is changed.
     */
    class ActiveTimeStepChangeEvent final : public Event {
    public:
        explicit ActiveTimeStepChangeEvent(int newTimeStep) {
            data[EventDataKey::ActiveTimeStep] = newTimeStep;
        }
        [[nodiscard]] EventType getType() const override { return EventType::TimeStepChangeEvent; }
    };

    /**
     * @class ApplyMaterial
     * @brief Event triggered to apply material to specific points on the grid.
     */
    class ApplyMaterial final : public Event {
    public:
        explicit ApplyMaterial(std::vector<models::Point> locations) {
            data[EventDataKey::Points] = locations;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ApplyMaterial; }
    };

    /**
     * @class ApplyInitialTemperature
     * @brief Event triggered to apply an initial temperature to specific points on the grid.
     */
    class ApplyInitialTemperature final : public Event {
    public:
        explicit ApplyInitialTemperature(std::vector<models::Point> locations) {
            data[EventDataKey::Points] = locations;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ApplyInitialTemperature; }
    };

    /**
     * @class ApplySource
     * @brief Event triggered to apply a heat source to specific points on the grid.
     */
    class ApplySource final : public Event {
    public:
        explicit ApplySource(std::vector<models::Point> locations) {
            data[EventDataKey::Points] = locations;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ApplySource; }
    };

    /**
     * @class ElementsUpdate
     * @brief Event triggered to update elements on the grid, includes size and state.
     */
    class ElementsUpdate final : public Event {
    public:
        explicit ElementsUpdate(const std::vector<models::Element> &elements, int width, int height) {
            data[EventDataKey::Elements] = elements;
            data[EventDataKey::Width] = width;
            data[EventDataKey::Height] = height;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ElementsUpdate; }
    };

    /**
     * @class SimulationTemperatureUpdate
     * @brief Event triggered to update the simulation temperatures across the grid.
     */
    class SimulationTemperatureUpdate final : public Event {
    public:
        explicit SimulationTemperatureUpdate(Eigen::VectorXd elements) {
            data[EventDataKey::SimulationTemperatures] = elements;
        }
        [[nodiscard]] EventType getType() const override { return EventType::SimulationTemperatureUpdate; }
    };

    /**
     * @class ColorMapChange
     * @brief Event triggered to change the color map used in visualization.
     */
    class ColorMapChange final : public Event {
    public:
        explicit ColorMapChange(ColorMaps c) {
            data[EventDataKey::ColorMap] = c;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ColorMapChange; }
    };
}
#endif //EVENT_H
