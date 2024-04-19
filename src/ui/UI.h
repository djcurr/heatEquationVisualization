//
// Created by djcur on 4/11/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_UI_H
#define HEATEQUATIONVISUALIZATION_UI_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "glad.h"
#include "glfw3.h"
#include "../models/Element.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../manager/MaterialManager.h"
#include "../solver/Solver.h"

namespace ui {

    class UI {

    public:
        static UI &
        getInstance(int windowWidth = 1920, int windowHeight = 900, int gridWidth = 50, int gridHeight = 50) {
            static UI instance(windowWidth, windowHeight, "Heat Transfer Simulation Test", gridWidth, gridHeight);
            return instance;
        }

        UI(UI const &) = delete;

        void operator=(UI const &) = delete;

        bool isInitialized() const;

        bool shouldClose() const;

        GLFWwindow *getWindow() const;

        void processInput();

        void updateSolverGrid() const;

        void render(const std::vector<models::Element> &elements, const int gridWidth, const int gridHeight);

    private:
        UI(int windowWidth, int windowHeight, const std::string &title, int gridWidth, int gridHeight);

        ~UI();

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

        static void initializeImGui();

        void renderControls();

        void renderGrid(const std::vector<models::Element> &elements, const int gridWidth, const int gridHeight);

        void finalizeRender();

        void setControlsStyle() const;

        static void removeControlsStyle();

        static void setGridStyles();

        static void removeGridStyles();

        ImVec4 calculateCellColor(const models::Element &element);

        static void renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color);

        void detectDrawing(int x, int y);

        void BresenhamLineAlgorithm(int currentX, int currentY);

        void applyElementParameters(int x, int y);

        void renderMaterialSelector();

        void renderTemperatureSelector();

        void renderGridSizeSelector();

        void renderControlsButtons();

        void updateElementMaterial(int x, int y) const;

        void updateElementInitialTemperatureCelsius(int x, int y) const;

        void updateElementSource(int x, int y) const;

        void renderSourceSelector();

        void renderViewSelector();

        void renderTimeStepInput();

        void renderTimeStepSlider();

        void startSimulation();

        void stopSimulation();

        void UI::renderBrushSizeSlider();

        double getCurrentElementTemperatureKelvin(const models::Element &element);

        enum ViewMode {
            VIEW_INITIAL_TEMPERATURE, VIEW_SOURCES, VIEW_MATERIAL, VIEW_VISUALIZATION
        };

        struct UIConfig {
            float controlWidth = 300.0f;
            ImVec4 windowBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            ImVec4 buttonColor = ImVec4(0.35f, 0.40f, 0.61f, 0.62f);
            ImVec4 buttonHoverColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
            ImVec4 buttonActiveColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
            ImVec2 buttonPadding = ImVec2(10, 10);
            float buttonRounding = 4.0f;
            float kelvinConstant = 273.15f;
        };
        UIConfig config;

        GLFWwindow *window;
        std::thread simulationThread;
        std::atomic_bool simulationRunning = false;
        std::atomic_bool simulationComplete = false;
        std::mutex simulationMutex;
        int gridWidth;
        int gridHeight;
        int currentViewTimeStep = 0;
        int brushSize = 1;
        std::vector<Eigen::VectorXf> simulationTemperatures;
        ViewMode currentViewMode = VIEW_INITIAL_TEMPERATURE;
        int selectedMaterialIndex = 0;
        float selectedTemperatureCelsius = 0;
        float selectedSource = 0;
        float timeStepSizeSeconds = 100000.0f;
        float targetDurationSeconds = 10000000.0f;
        bool mouseDown = false;
        std::pair<int, int> lastHoveredCell = {-1, -1};
        manager::MaterialManager &materialManager = manager::MaterialManager::getInstance();
        solver::Solver &solver1 = solver::Solver::getInstance();
    };

} // ui

#endif //HEATEQUATIONVISUALIZATION_UI_H
