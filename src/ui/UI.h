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
        getInstance(int windowWidth = 1000, int windowHeight = 800, int gridWidth = 10, int gridHeight = 10) {
            static UI instance(windowWidth, windowHeight, "Heat Transfer Simulation Test", gridWidth, gridHeight);
            return instance;
        }

        UI(UI const &) = delete;

        void operator=(UI const &) = delete;

        bool isInitialized() const;

        bool shouldClose() const;

        GLFWwindow *getWindow() const;

        void processInput();

        void updateSolverGrid();

        void render(const std::vector<models::Element> &elements, const int gridWidth, const int gridHeight);

    private:
        UI(int windowWidth, int windowHeight, const std::string &title, int gridWidth, int gridHeight);

        ~UI();

        static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                                           GLsizei length, const char *message, const void *userParam);

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

        void initializeImGui();

        void renderControls();

        void renderGrid(const std::vector<models::Element> &elements, const int gridWidth, const int gridHeight);

        void finalizeRender();

        void setControlsStyle();

        void removeControlsStyle();

        void setGridStyles();

        void removeGridStyles();

        ImVec4 calculateCellColor(const models::Element &element) const;

        void renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color);

        void detectDrawing(int x, int y);

        void BresenhamLineAlgorithm(int currentX, int currentY);

        void applyElementParameters();

        void renderMaterialSelector();

        void renderTemperatureSelector();

        void renderGridSizeSelector();

        void renderControlsButtons();

        void updateElementMaterial(int i, int j);

        void updateElementInitialTemperatureCelsius(int x, int y);

        void updateElementSource(int x, int y);

        void renderSourceSelector();

        void renderViewSelector();

        void startSimulation();

        void stopSimulation();

        void renderTimeStepInput();

        void simulationLoop();

        void resetSimulationTimer();

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
        std::mutex mtx;
        int gridWidth;
        int gridHeight;
        ViewMode currentViewMode = VIEW_INITIAL_TEMPERATURE;
        int selectedMaterialIndex = 0;
        float selectedTemperatureCelsius = 0;
        float selectedSource = 0;
        std::atomic<float> timeStepSizeSeconds = 100000.0f;
        bool simulationRunning = false;
        float accumulatedTime = 0.0f;
        std::atomic<float> targetDurationSeconds = 0.0f;
        bool mouseDown = false;
        bool needsReset = true;
        std::pair<int, int> lastHoveredCell = {-1, -1};
        manager::MaterialManager &materialManager = manager::MaterialManager::getInstance();
        solver::Solver &solver1 = solver::Solver::getInstance();
    };

} // ui

#endif //HEATEQUATIONVISUALIZATION_UI_H
