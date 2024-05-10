//
// Created by djcur on 4/19/2024.
//

#include "ControlsRenderer.h"

#include <iomanip>
#include <vector>

#include "ColorMap.h"
#include "../config/Config.h"
#include "StyleManager.h"
#include "../events/Broker.h"
#include "../manager/MaterialManager.h"

namespace ui {
    void ControlsRenderer::render() {
        setWindow();
        renderControls();
    }

    void ControlsRenderer::setWindow() {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - config::Config::config.controlWidth, 0),
                                ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(config::Config::config.controlWidth, ImGui::GetIO().DisplaySize.y));
    }

    void ControlsRenderer::renderControls() {
        StyleManager::applyStyle(controlColors, controlVars);
        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        renderControlsButtons();
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        renderViewSelector();
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        renderTimeStepInput();
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        renderGridSizeSelector();
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        renderBrushSizeSlider();
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        renderColorSettings();

        ImGui::End();
        StyleManager::removeStyle(controlColors.size(), controlVars.size());
    }

    void ControlsRenderer::renderControlsButtons() {
        handleButton<events::StartSimulationEvent>("Process", ImVec2(-1, 0), ViewMode::VIEW_VISUALIZATION,
                                                   ViewMode::VIEW_INITIAL_TEMPERATURE, events::StartSimulationEvent());
        ImGui::Spacing();
        handleButton<events::ResetSimulationEvent>("Reset Simulation", ImVec2(-1, 0), ViewMode::VIEW_VISUALIZATION,
                                                   ViewMode::VIEW_INITIAL_TEMPERATURE, events::ResetSimulationEvent());
    }

    void ControlsRenderer::renderViewSelector() {
        ImGui::Text("View Options");

        if (handleRadioButton("Initial Temperature", ViewMode::VIEW_INITIAL_TEMPERATURE)) {
            renderTemperatureSelector();
        }
        if (handleRadioButton("Source", ViewMode::VIEW_SOURCES)) {
            renderSourceSelector();
        }
        if (handleRadioButton("Material", ViewMode::VIEW_MATERIAL)) {
            renderMaterialSelector();
        }
        if (!simulationComplete && simulationRunning) {
            renderProgressBar();
        }
        if (simulationComplete && handleRadioButton("Visualization", ViewMode::VIEW_VISUALIZATION)) {
            renderTimeStepSlider();
        }
    }

    void ControlsRenderer::renderTimeStepInput() {
        renderStandardInput<int, events::TimeStepSizeChangeEvent>("Timestep Size (seconds)", timeStepSizeSeconds);
        renderStandardInput<int, events::TimeDurationChangeEvent>("Time Duration (seconds)", targetDurationSeconds);
    }

    void ControlsRenderer::renderGridSizeSelector() {
        int oldWidth = gridWidth;
        int oldHeight = gridHeight;

        // Capture changes using the slider inputs for width and height
        renderSliderInput<int, events::GridWidthChangeEvent>("Grid Width", gridWidth, 1,
                                                             config::Config::config.maxGridWidth);
        renderSliderInput<int, events::GridHeightChangeEvent>("Grid Height", gridHeight, 1,
                                                              config::Config::config.maxGridHeight);

        // If either width or height changed and the current view is VIEW_VISUALIZATION
        if ((oldWidth != gridWidth || oldHeight != gridHeight) && currentView == ViewMode::VIEW_VISUALIZATION) {
            broker.publish<events::ViewChangeEvent>(ViewMode::VIEW_INITIAL_TEMPERATURE);
            timeStepsCompleted = 0;
            currentView = ViewMode::VIEW_INITIAL_TEMPERATURE;
        }
    }

    void ControlsRenderer::renderColorSettings() {
        if (ImGui::BeginCombo("Color Map", getColorMapName(currentColorMap).c_str())) {
            if (ImGui::Selectable(getColorMapName(ColorMaps::VIRIDIS).c_str(), currentColorMap == ColorMaps::VIRIDIS)) {
                currentColorMap = ColorMaps::VIRIDIS;
                broker.publish<events::ColorMapChange>(events::ColorMapChange(currentColorMap));
            }
            if (ImGui::Selectable(getColorMapName(ColorMaps::PLASMA).c_str(), currentColorMap == ColorMaps::PLASMA)) {
                currentColorMap = ColorMaps::PLASMA;
                broker.publish<events::ColorMapChange>(events::ColorMapChange(currentColorMap));
            }
            ImGui::EndCombo();
        }
    }

    void ControlsRenderer::renderBrushSizeSlider() {
        renderStandardInput<int, events::BrushSizeChangeEvent>("Brush Size", brushSize);
    }

    void ControlsRenderer::renderTimeStepSlider() {
        renderSliderInput<int, events::ActiveTimeStepChangeEvent>("View Time Step", currentViewTimeStep, 0,
                                                                  maxTimeSteps - 1);
    }

    void ControlsRenderer::renderTemperatureSelector() {
        if (currentView == ViewMode::VIEW_INITIAL_TEMPERATURE) {
            renderStandardInput<float, events::TemperatureInputEvent>("Temperature (°C)", selectedTemperatureCelsius);
        }
    }

    void ControlsRenderer::renderSourceSelector() {
        if (currentView == ViewMode::VIEW_SOURCES) {
            renderStandardInput<float, events::SourceChangeEvent>("Source (Watts)", selectedSource);
        }
    }

    void ControlsRenderer::renderMaterialSelector() {
        if (currentView != ViewMode::VIEW_MATERIAL) {
            return;
        }

        static std::vector<std::string> materialNames = manager::MaterialManager::getInstance().getAllMaterialNames();
        const char *currentMaterial = materialNames[selectedMaterialIndex].c_str();

        if (ImGui::BeginCombo("Materials", currentMaterial)) {
            for (int i = 0; i < materialNames.size(); ++i) {
                bool isSelected = (i == selectedMaterialIndex);
                if (ImGui::Selectable(materialNames[i].c_str(), isSelected)) {
                    if (selectedMaterialIndex != i) {
                        selectedMaterialIndex = i;
                        broker.publish<events::MaterialChangeEvent>(materialNames[i]);
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    void ControlsRenderer::renderProgressBar() const {
        const int maxTimeSteps = targetDurationSeconds / timeStepSizeSeconds;
        float percent = static_cast<float>(timeStepsCompleted) / static_cast<float>(maxTimeSteps);
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(1) << (percent * 100) << '%';
        ImGui::ProgressBar(percent, ImVec2(-1.0f, 0.0f), stream.str().c_str());
    }

    bool ControlsRenderer::handleRadioButton(const std::string &label, ViewMode targetView, bool condition) {
        if (ImGui::RadioButton(label.c_str(), currentView == targetView) && currentView != targetView && condition) {
            changeView(targetView);
        }
        return currentView == targetView;
    }

    template<typename Event>
    void ControlsRenderer::handleButton(const std::string &label, const ImVec2 &size, ViewMode checkView,
                                        ViewMode changeToView, Event event) {
        if (ImGui::Button(label.c_str(), size)) {
            if (currentView == checkView) {
                changeView(changeToView);
            }
            broker.publish<Event>(event);
        }
    }

    void ControlsRenderer::changeView(ViewMode view) {
        currentView = view;
        broker.publish<events::ViewChangeEvent>(view);
    }

    template<typename T, typename Event, typename... Args>
    void ControlsRenderer::renderStandardInput(const std::string &label, T &value, Args &&... args) {
        T oldValue = value;
        if (inputHelper(label, &value)) {
            // Assumed to select the correct ImGui input function based on T
            publishIfChanged<T, Event>(oldValue, value, std::forward<Args>(args)...);
        }
    }

    template<typename T, typename Event, typename... Args>
    void ControlsRenderer::renderSliderInput(const std::string &label, T &value, T min, T max, Args &&... args) {
        T oldValue = value;
        if (inputHelper(label, &value, min, max)) {
            // Handles sliders
            publishIfChanged<T, Event>(oldValue, value, std::forward<Args>(args)...);
        }
    }

    template<typename T, typename Event, typename... Args>
    void ControlsRenderer::publishIfChanged(T &oldValue, const T newValue, Args &&... args) {
        if (oldValue != newValue) {
            oldValue = newValue;
            broker.publish<Event>(newValue, std::forward<Args>(args)...);
        }
    }


    void ControlsRenderer::onEvent(const std::shared_ptr<events::Event> &event) {
        switch (event->getType()) {
            case EventType::SimulationCompleted:
                simulationComplete = true;
                simulationRunning = false;
                changeView(ViewMode::VIEW_VISUALIZATION);
                break;
            case EventType::SimulationRunning:
                simulationComplete = false;
                simulationRunning = true;
                break;
            case EventType::TimeStepChangeEvent:
                currentViewTimeStep = std::get<int>(event->getData(EventDataKey::ActiveTimeStep));
                break;
            case EventType::ResetSimulation:
                changeView(ViewMode::VIEW_INITIAL_TEMPERATURE);
                simulationComplete = false;
                timeStepsCompleted = 0;
                break;
            case EventType::TimeStepCompleted:
                timeStepsCompleted++;
                break;
            case EventType::StartSimulation:
                timeStepsCompleted = 0;
                maxTimeSteps = targetDurationSeconds / timeStepSizeSeconds;
                break;
            default:
                break;
        }
    }

    bool ControlsRenderer::inputHelper(const std::string &label, float *value) {
        return ImGui::InputFloat(label.c_str(), value);
    }

    bool ControlsRenderer::inputHelper(const std::string &label, int *value) {
        int temp = *value;
        bool input = ImGui::InputInt(label.c_str(), &temp);
        if (input && temp != 0) *value = temp;
        return input;
    }

    bool ControlsRenderer::inputHelper(const std::string &label, int *value, int min, int max) {
        return ImGui::SliderInt(label.c_str(), value, min, max);
    }

    bool ControlsRenderer::inputHelper(const std::string &label, float *value, float min, float max) {
        return ImGui::SliderFloat(label.c_str(), value, min, max);
    }
} // ui
