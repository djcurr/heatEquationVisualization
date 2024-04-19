//
// Created by djcur on 4/19/2024.
//

#include "ControlsRenderer.h"

#include <vector>

namespace ui {
    void ControlsRenderer::renderControls() {
        renderControlsButtons();

        renderViewSelector();

        renderTimeStepInput();

        renderGridSizeSelector();

        renderBrushSizeSlider();
    }

    void ControlsRenderer::render() {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - config.controlWidth, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(config.controlWidth, ImGui::GetIO().DisplaySize.y));

        setControlsStyle();

        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        renderControls();

        ImGui::End();

        removeControlsStyle();
    }

    void ControlsRenderer::renderControlsButtons() {
        if (ImGui::Button("Process", ImVec2(-1, 0))) {
            if (currentView == VIEW_VISUALIZATION) {
                currentView = VIEW_INITIAL_TEMPERATURE;
            }
            startSimulation();
        }
        if (ImGui::Button("Reset Simulation", ImVec2(-1, 0))) {
            if (currentView == VIEW_VISUALIZATION) {
                currentView = VIEW_INITIAL_TEMPERATURE;
            }
            stopSimulation();
            solver::Solver::getInstance().updateGridSize(gridWidth, gridHeight);
        }
    }

    void ControlsRenderer::renderViewSelector() {
        ImGui::Text("View Options");
        if (ImGui::RadioButton("Initial Temperature", currentView == VIEW_INITIAL_TEMPERATURE)) {
            currentView = VIEW_INITIAL_TEMPERATURE;
        }
        renderTemperatureSelector();
        if (ImGui::RadioButton("Source", currentView == VIEW_SOURCES)) {
            currentView = VIEW_SOURCES;
        }
        renderSourceSelector();
        if (ImGui::RadioButton("Material", currentView == VIEW_MATERIAL)) {
            currentView = VIEW_MATERIAL;
        }
        renderMaterialSelector();
        if (simulationComplete.load()) {
            if (ImGui::RadioButton("Visualization", currentView == VIEW_VISUALIZATION)) {
                currentView = VIEW_VISUALIZATION;
            }
            if (currentView == VIEW_VISUALIZATION) {
                renderTimeStepSlider();
            }
        }
    }

    void ControlsRenderer::renderTimeStepSlider() {
        if (ImGui::SliderInt("View Time Step", &currentViewTimeStep, 0, static_cast<int>(targetDurationSeconds / timeStepSizeSeconds) - 1)) {}
    }

    void ControlsRenderer::renderGridSizeSelector() {
        ImGui::SliderInt("Grid Width", &gridWidth, 1, 200); // Min = 1, Max = 100
        ImGui::SliderInt("Grid Height", &gridHeight, 1, 200); // Min = 1, Max = 100
    }

    void ControlsRenderer::renderTemperatureSelector() {
        if (currentView == VIEW_INITIAL_TEMPERATURE) {
            // Only enable the text box if the temperature checkbox is checked
            ImGui::InputFloat("Temperature (°C)", &selectedTemperatureCelsius);
            }
    }

    void ControlsRenderer::renderSourceSelector() {
        if (currentView == VIEW_SOURCES) {
            // Only enable the text box if the temperature checkbox is checked
            ImGui::InputFloat("Source (Watts)", &selectedSource);
        }
    }

    void ControlsRenderer::renderMaterialSelector() {
        if (currentView == VIEW_MATERIAL) {
            std::vector<std::string> materialNames = materialManager.getAllMaterialNames();
            if (ImGui::BeginCombo("Materials", materialNames[selectedMaterialIndex].c_str())) {
                for (int i = 0; i < materialNames.size(); i++) {
                    bool isSelected = (selectedMaterialIndex == i);
                    if (ImGui::Selectable(materialNames[i].c_str(), isSelected)) {
                        selectedMaterialIndex = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
    }

    void ControlsRenderer::renderBrushSizeSlider() {
        if (ImGui::SliderInt("Brush Size", &brushSize, 1, 10)) {}
    }

    void ControlsRenderer::renderTimeStepInput() {
        if (ImGui::InputFloat("Timestep Size (seconds)", &timeStepSizeSeconds)) {
        }
        if (ImGui::InputFloat("Time Duration (seconds)", &targetDurationSeconds)) {
        }
    }

    void ControlsRenderer::setControlsStyle() {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, config.windowBackgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Button, config.buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.buttonHoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.buttonActiveColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config.buttonPadding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.buttonRounding);
    }

    void ControlsRenderer::removeControlsStyle() {
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
    }
} // ui