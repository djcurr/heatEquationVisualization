//
// Created by djcur on 4/20/2024.
//

#ifndef EVENT_H
#define EVENT_H
#include <string>
#include <unordered_map>
#include <variant>
#include <Eigen>

#include "EventTypes.h"
#include "../models/Point.h"
#include "../ui/ColorMap.h"

namespace models {
    class Element;
}

namespace events {
    using EventData = std::variant<int, float, ViewMode, std::string, std::vector<models::Element>, Eigen::VectorXd, std::vector<models::Point>, ColorMaps>;

    class Event {
    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual EventType getType() const = 0;

        [[nodiscard]] virtual EventData getData(const EventDataKey &key) const {
            if (const auto it = data.find(key); it != data.end()) {
                return it->second;
            }
            return {}; // Return empty variant if key not found
        }

    protected:
        std::unordered_map<EventDataKey, EventData> data;
    };

    class GridWidthChangeEvent final : public Event {
    public:
        explicit GridWidthChangeEvent(int w) {
            data[EventDataKey::Width] = w;
        }

        [[nodiscard]] EventType getType() const override { return EventType::GridWidthChange; }
    };

    class GridHeightChangeEvent final : public Event {
    public:
        explicit GridHeightChangeEvent(int h) {
            data[EventDataKey::Height] = h;
        }

        [[nodiscard]] EventType getType() const override { return EventType::GridHeightChange; }
    };

    class StartSimulationEvent final : public Event {
    public:
        StartSimulationEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::StartSimulation; }
    };

    class StopSimulationEvent final : public Event {
    public:
        StopSimulationEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::StopSimulation; }
    };

    class SimulationCompletedEvent : public Event {
    public:
        SimulationCompletedEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::SimulationCompleted; }
    };

    class SimulationRunningEvent : public Event {
    public:
        SimulationRunningEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::SimulationRunning; }
    };

    class ResetSimulationEvent : public Event {
    public:
        ResetSimulationEvent() = default;
        [[nodiscard]] EventType getType() const override { return EventType::ResetSimulation; }
    };

    class TimeStepCompleted final : public Event {
    public:
        TimeStepCompleted() = default;
        [[nodiscard]] EventType getType() const override { return EventType::TimeStepCompleted; }
    };

    class ViewChangeEvent final : public Event {
    public:
        explicit ViewChangeEvent(ViewMode newView) {
            data[EventDataKey::ViewMode] = newView;
        }

        [[nodiscard]] EventType getType() const override { return EventType::ViewChange; }
    };

    class TemperatureInputEvent final : public Event {
    public:
        explicit TemperatureInputEvent(float temperature) {
            data[EventDataKey::Temperature] = temperature;
        }

        [[nodiscard]] EventType getType() const override { return EventType::TemperatureInputEvent; }
    };

    class SourceChangeEvent final : public Event {
    public:
        explicit SourceChangeEvent(float source) {
            data[EventDataKey::Source] = source;
        }

        [[nodiscard]] EventType getType() const override { return EventType::SourceChangeEvent; }
    };

    class MaterialChangeEvent final : public Event {
    public:
        explicit MaterialChangeEvent(const std::string &materialName) {
            data[EventDataKey::MaterialName] = materialName;
        }

        [[nodiscard]] EventType getType() const override { return EventType::MaterialChangeEvent; }
    };

    class BrushSizeChangeEvent final : public Event {
    public:
        explicit BrushSizeChangeEvent(int brushSize) {
            data[EventDataKey::BrushSize] = brushSize;
        }

        [[nodiscard]] EventType getType() const override { return EventType::BrushSizeChangeEvent; }
    };

    class TimeStepSizeChangeEvent final : public Event {
    public:
        explicit TimeStepSizeChangeEvent(int timestepSize) {
            data[EventDataKey::TimeStepSize] = timestepSize;
        }

        [[nodiscard]] EventType getType() const override { return EventType::TimeStepSizeChangeEvent; }
    };

    class TimeDurationChangeEvent final : public Event {
    public:
        explicit TimeDurationChangeEvent(int duration) {
            data[EventDataKey::TimeDuration] = duration;
        }

        [[nodiscard]] EventType getType() const override { return EventType::TimeDurationChangeEvent; }
    };

    class ActiveTimeStepChangeEvent final : public Event {
    public:
        explicit ActiveTimeStepChangeEvent(int newTimeStep) {
            data[EventDataKey::ActiveTimeStep] = newTimeStep;
        }

        [[nodiscard]] EventType getType() const override { return EventType::TimeStepChangeEvent; }
    };

    class ApplyMaterial final : public Event {
    public:
        explicit ApplyMaterial(std::vector<models::Point> locations) {
            data[EventDataKey::Points] = locations;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ApplyMaterial; }
    };

    class ApplyInitialTemperature final : public Event {
    public:
        explicit ApplyInitialTemperature(std::vector<models::Point> locations) {
            data[EventDataKey::Points] = locations;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ApplyInitialTemperature; }
    };

    class ApplySource final : public Event {
    public:
        explicit ApplySource(std::vector<models::Point> locations) {
            data[EventDataKey::Points] = locations;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ApplySource; }
    };

    class ElementsUpdate final : public Event {
    public:
        explicit ElementsUpdate(std::vector<models::Element> elements, int width, int height) {
            data[EventDataKey::Elements] = elements;
            data[EventDataKey::Width] = width;
            data[EventDataKey::Height] = height;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ElementsUpdate; }
    };

    class SimulationTemperatureUpdate final : public Event {
    public:
        explicit SimulationTemperatureUpdate(Eigen::VectorXd elements) {
            data[EventDataKey::SimulationTemperatures] = elements;
        }
        [[nodiscard]] EventType getType() const override { return EventType::SimulationTemperatureUpdate; }
    };

    class ColorMapChange final : public Event {
    public:
        explicit ColorMapChange(ColorMaps c) {
            data[EventDataKey::ColorMap] = c;
        }
        [[nodiscard]] EventType getType() const override { return EventType::ColorMapChange; }
    };
}
#endif //EVENT_H
