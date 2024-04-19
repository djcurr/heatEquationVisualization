//
// Created by djcur on 4/11/2024.
//

#include <iostream>
#include "UI.h"

namespace ui {
    UI::UI(int windowWidth, int windowHeight, const std::string &title, int gridWidth, int gridHeight) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW." << std::endl;
            window = nullptr;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window." << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD." << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            window = nullptr;
            return;
        }
        // Initialize Dear ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        this->gridWidth = gridWidth;
        this->gridHeight = gridHeight;
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glViewport(0, 0, windowWidth, windowHeight);
    }

    UI::~UI() {
        if (window != nullptr) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    bool UI::isInitialized() const {
        return window != nullptr;
    }

    bool UI::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void UI::processInput() {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void UI::render(const std::vector<models::Element> &elements, const int gridWidth, const int gridHeight) {
        initializeImGui();

        renderControls();
        renderGrid(elements, gridWidth, gridHeight);
        finalizeRender();
    }

    void UI::finalizeRender() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    void UI::renderGrid(const std::vector<models::Element> &elements, const int gridWidth, const int gridHeight) {
        ImVec2 gridSizeIm = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowSize(ImVec2(gridSizeIm.x - config.controlWidth, gridSizeIm.y));
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

        setGridStyles();

        ImGui::Begin("Grid Visualization", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoTitleBar);

        ImVec2 buttonSize = ImGui::GetContentRegionAvail();
        buttonSize.y /= static_cast<float>(gridHeight);
        buttonSize.x /= static_cast<float>(gridWidth);

        ImVec2 cursorStart = ImGui::GetCursorScreenPos();
        for (int i = 0; i < gridHeight; ++i) {
            for (int j = 0; j < gridWidth; ++j) {
                ImVec2 cellPos = ImVec2(cursorStart.x + j * buttonSize.x, cursorStart.y + i * buttonSize.y);
                ImVec4 color = calculateCellColor(elements[i * gridWidth + j]);
                renderGridCell(cellPos, buttonSize, color);
                detectDrawing(j, i);
                if ((j + 1) % static_cast<int>(gridWidth) != 0) ImGui::SameLine();
            }
        }

        if (!ImGui::IsMouseDown(0) && mouseDown) {
            mouseDown = false; // Reset on mouse up
        }

        ImGui::End();
        removeGridStyles();
    }

    void UI::detectDrawing(int y, int x) {
        if (ImGui::IsItemHovered()) {
            if (ImGui::IsMouseDown(0)) {
                mouseDown = true;
                lastHoveredCell = {x, y};
            }
            if (mouseDown) {
                BresenhamLineAlgorithm(x, y);
            }
        }
    }

    void UI::BresenhamLineAlgorithm(int currentX, int currentY) {
        int x1 = lastHoveredCell.first;
        int y1 = lastHoveredCell.second;
        int const dx = std::abs(currentX - x1);
        int const dy = -std::abs(currentY - y1);
        int sx = x1 < currentX ? 1 : -1;
        int sy = y1 < currentY ? 1 : -1;
        int err = dx + dy, e2;

        while (true) {
            applyElementParameters(x1, y1);  // Apply brush effect here
            if (x1 == currentX && y1 == currentY) break;
            e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x1 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y1 += sy;
            }
        }
        lastHoveredCell = {currentX, currentY};
    }


    void UI::applyElementParameters(int x, int y) {
        int startX = std::max(0, x - brushSize);
        int endX = std::min(gridWidth - 1, x + brushSize);
        int startY = std::max(0, y - brushSize);
        int endY = std::min(gridHeight - 1, y + brushSize);

        for (int i = startY; i <= endY; ++i) {
            for (int j = startX; j <= endX; ++j) {
                if (currentViewMode == VIEW_MATERIAL) {
                    updateElementMaterial(i, j);
                }
                if (currentViewMode == VIEW_INITIAL_TEMPERATURE) {
                    updateElementInitialTemperatureCelsius(i, j);
                }
                if (currentViewMode == VIEW_SOURCES) {
                    updateElementSource(i, j);
                }
            }
        }
    }

    void UI::updateElementMaterial(const int x, const int y) const {
        solver1.setMaterial(x, y,
                            materialManager.getMaterial(materialManager.getAllMaterialNames()[selectedMaterialIndex]));
    }

    void UI::updateElementInitialTemperatureCelsius(const int x, const int y) const {
        solver1.setInitialTemperatureKelvin(x, y, selectedTemperatureCelsius);
    }

    void UI::updateElementSource(const int x, const int y) const {
        solver1.setElementSource(x, y, selectedSource);
    }

    ImVec4 UI::calculateCellColor(const models::Element &element) {
        float heat = 0.0;
        float normalizedHeat = 0.0; // Declare it once, outside the switch statement
        auto color = models::Color();

        switch (currentViewMode) {
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

    void UI::renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color) {
        ImGui::GetWindowDrawList()->AddRectFilled(cellPos, ImVec2(cellPos.x + cellSize.x, cellPos.y + cellSize.y),
                                                  ImGui::ColorConvertFloat4ToU32(color));
        ImGui::InvisibleButton("cell", cellSize);
    }

    void UI::removeGridStyles() { ImGui::PopStyleVar(2); }

    void UI::setGridStyles() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    }

    void UI::renderControls() {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - config.controlWidth, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(config.controlWidth, ImGui::GetIO().DisplaySize.y));

        setControlsStyle();

        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        renderControlsButtons();

        renderViewSelector();

        renderTimeStepInput();

        renderGridSizeSelector();

        renderBrushSizeSlider();

        ImGui::End();

        removeControlsStyle();
    }

    void UI::renderTimeStepInput() {
        if (ImGui::InputFloat("Timestep Size (seconds)", &timeStepSizeSeconds)) {
        }
        if (ImGui::InputFloat("Time Duration (seconds)", &targetDurationSeconds)) {
        }
    }

    void UI::renderTimeStepSlider() {
        if (ImGui::SliderInt("View Time Step", &currentViewTimeStep, 0, static_cast<int>(targetDurationSeconds / timeStepSizeSeconds) - 1)) {}
    }

    void UI::renderControlsButtons() {
        if (ImGui::Button("Process", ImVec2(-1, 0))) {
            if (currentViewMode == VIEW_VISUALIZATION) {
                currentViewMode = VIEW_INITIAL_TEMPERATURE;
            }
            startSimulation();
        }
        if (ImGui::Button("Reset Simulation", ImVec2(-1, 0))) {
            if (currentViewMode == VIEW_VISUALIZATION) {
                currentViewMode = VIEW_INITIAL_TEMPERATURE;
            }
            stopSimulation();
            solver1.updateGridSize(gridWidth, gridHeight);
        }
    }

    void UI::renderViewSelector() {
        ImGui::Text("View Options");
        if (ImGui::RadioButton("Initial Temperature", currentViewMode == VIEW_INITIAL_TEMPERATURE)) {
            currentViewMode = VIEW_INITIAL_TEMPERATURE;
        }
        renderTemperatureSelector();
        if (ImGui::RadioButton("Source", currentViewMode == VIEW_SOURCES)) {
            currentViewMode = VIEW_SOURCES;
        }
        renderSourceSelector();
        if (ImGui::RadioButton("Material", currentViewMode == VIEW_MATERIAL)) {
            currentViewMode = VIEW_MATERIAL;
        }
        renderMaterialSelector();
        if (simulationComplete.load()) {
            if (ImGui::RadioButton("Visualization", currentViewMode == VIEW_VISUALIZATION)) {
                currentViewMode = VIEW_VISUALIZATION;
            }
            if (currentViewMode == VIEW_VISUALIZATION) {
                renderTimeStepSlider();
            }
        }
    }

    void UI::renderGridSizeSelector() {
        ImGui::SliderInt("Grid Width", &gridWidth, 1, 200); // Min = 1, Max = 100
        ImGui::SliderInt("Grid Height", &gridHeight, 1, 200); // Min = 1, Max = 100
    }

    void UI::renderTemperatureSelector() {
        if (currentViewMode ==
            VIEW_INITIAL_TEMPERATURE) {
            // Only enable the text box if the temperature checkbox is checked
            ImGui::InputFloat("Temperature (°C)", &selectedTemperatureCelsius);
        }
    }

    void UI::renderBrushSizeSlider() {
        if (ImGui::SliderInt("Brush Size", &brushSize, 1, 10)) {}
    }

    void UI::renderSourceSelector() {
        if (currentViewMode == VIEW_SOURCES) {
            // Only enable the text box if the temperature checkbox is checked
            ImGui::InputFloat("Source (Watts)", &selectedSource);
        }
    }

    void UI::renderMaterialSelector() {
        if (currentViewMode == VIEW_MATERIAL) {
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

    void UI::removeControlsStyle() {
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
    }

    void UI::setControlsStyle() const {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, config.windowBackgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Button, config.buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.buttonHoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.buttonActiveColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config.buttonPadding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.buttonRounding);
    }

    void UI::updateSolverGrid() const {
        static int currentWidth = gridWidth;
        static int currentHeight = gridHeight;

        if (currentWidth != gridWidth || currentHeight != gridHeight) {
            solver1.updateGridSize(gridWidth, gridHeight);
            currentWidth = gridWidth;
            currentHeight = gridHeight;
        }
    }

    void UI::startSimulation() {
        if (simulationRunning) return; // Prevent redundant starts
        if (simulationThread.joinable()) {
            simulationThread.join(); // Ensure the simulation thread finishes cleanly
        }
        simulationRunning.store(true);
        simulationComplete.store(false);
        simulationThread = std::thread([this]() {
            const auto temp = solver1.performSimulation(
                static_cast<int>(targetDurationSeconds / timeStepSizeSeconds), timeStepSizeSeconds);
            std::lock_guard<std::mutex> lock(simulationMutex);
            simulationTemperatures = temp;
            simulationComplete.store(true);
            simulationRunning.store(false);
        });
    }

    void UI::stopSimulation() {
        simulationRunning.store(false);
        if (simulationThread.joinable()) {
            simulationThread.join(); // Ensure the simulation thread finishes cleanly
        }
    }

    double UI::getCurrentElementTemperatureKelvin(const models::Element &element) {
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

    void UI::initializeImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UI::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        const solver::Solver &solver1 = solver::Solver::getInstance();
        ui::UI::getInstance().render(*solver1.getElements(), solver1.getGridWidth(), solver1.getGridHeight());
        glfwPollEvents();
    }

} // ui
