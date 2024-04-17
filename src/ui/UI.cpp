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
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

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

    void UI::detectDrawing(int x, int y) {
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
        int x1 = lastHoveredCell.second;
        int y1 = lastHoveredCell.first;
        int const dx = std::abs(currentX - x1);
        int const dy = -std::abs(currentY - y1);
        int sx = x1 < currentX ? 1 : -1;
        int sy = y1 < currentY ? 1 : -1;
        int err = dx + dy, e2; // error value e_xy

        while (true) {
            applyElementParameters();
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

    void UI::applyElementParameters() {
        if (currentViewMode == VIEW_MATERIAL) { updateElementMaterial(lastHoveredCell.first, lastHoveredCell.second); }
        if (currentViewMode == VIEW_INITIAL_TEMPERATURE) {
            updateElementInitialTemperatureCelsius(lastHoveredCell.first, lastHoveredCell.second);
        }
        if (currentViewMode == VIEW_SOURCES) { updateElementSource(lastHoveredCell.first, lastHoveredCell.second); }
    }

    void UI::updateElementMaterial(int x, int y) {
        solver1.setMaterial(x, y,
                            materialManager.getMaterial(materialManager.getAllMaterialNames()[selectedMaterialIndex]));
        resetSimulationTimer();
    }

    void UI::updateElementInitialTemperatureCelsius(int x, int y) {
        solver1.setInitialTemperatureKelvin(x, y, selectedTemperatureCelsius);
        resetSimulationTimer();
    }

    void UI::updateElementSource(int x, int y) {
        solver1.setElementSource(x, y, selectedSource);
        resetSimulationTimer();
    }

    ImVec4 UI::calculateCellColor(const models::Element &element) const {
        float heat = 0.0;
        float normalizedHeat = 0.0; // Declare it once, outside the switch statement
        models::Color color = models::Color();

        switch (currentViewMode) {
            case VIEW_INITIAL_TEMPERATURE:
                heat = element.getInitialHeatKelvin();
                normalizedHeat = std::max(0.0f, std::min(1.0f, heat / 100));
                return ImVec4(normalizedHeat, 0.0, 1.0 - normalizedHeat, 1.0);

            case VIEW_SOURCES:
                heat = element.getHeatSource(); // Assuming getHeatSource() was a typo and should be getExternalHeat()
                normalizedHeat = std::max(0.0f, std::min(1.0f, heat / 100));
                return ImVec4(normalizedHeat, 0.0, 1.0 - normalizedHeat, 1.0);

            case VIEW_MATERIAL:
                color = element.getMaterial().getColor();
                return ImVec4(color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, 1.0);

            case VIEW_VISUALIZATION:
                heat = solver1.getCurrentElementTemperatureKelvin(element);
                normalizedHeat = std::max(0.0f, std::min(1.0f, heat / 100));
                return ImVec4(normalizedHeat, 0.0, 1.0 - normalizedHeat, 1.0);
            default:
                return ImVec4(1.0f, 0.0f, 0.0f, 1.0);
        }
    }

    void UI::simulationLoop() {
        float accumulatedTime = 0.0f;

        while (simulationRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep to reduce CPU usage
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (needsReset) {
                    accumulatedTime = 0.0f;
                    needsReset = false;
                    continue;
                }
            }

            float currentStepSize = timeStepSizeSeconds.load();
            accumulatedTime += currentStepSize;
            solver1.solveTimeStep1(currentStepSize);

            if (accumulatedTime >= targetDurationSeconds.load()) {
                break;
            }
        }

//        stopSimulation();
    }

    void UI::stopSimulation() {
        simulationRunning = false;
        if (simulationThread.joinable()) {
            simulationThread.join(); // Wait for the thread to finish
        }
    }

    void UI::startSimulation() {
        if (simulationRunning) return; // Prevent redundant starts
        simulationRunning = true;
        if (simulationThread.joinable()) {
            simulationThread.join(); // Ensure any previous thread is finished
        }
        simulationThread = std::thread(&UI::simulationLoop, this);
    }

    void UI::resetSimulationTimer() {
        std::lock_guard<std::mutex> lock(mtx);
        needsReset = true; // Explicitly request a reset, which will zero out accumulatedTime
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

        ImGui::End();

        removeControlsStyle();
    }

    void UI::renderTimeStepInput() {
        // Temporary float variables
        float tempTimeStepSize = timeStepSizeSeconds.load();
        float tempTargetDuration = targetDurationSeconds.load();

        // Input fields for temporary floats
        if (ImGui::InputFloat("Timestep Size (seconds)", &tempTimeStepSize)) {
            // Update atomic variable if input changes
            timeStepSizeSeconds.store(tempTimeStepSize);
        }
        if (ImGui::InputFloat("Time Duration (seconds)", &tempTargetDuration)) {
            // Update atomic variable if input changes
            targetDurationSeconds.store(tempTargetDuration);
        }
    }

    void UI::renderControlsButtons() {
        if (ImGui::Button("Begin Visualization", ImVec2(-1, 0))) {
            currentViewMode = VIEW_VISUALIZATION;
            startSimulation();
        }
        if (ImGui::Button("Stop Visualization")) {
            stopSimulation();
        }
        if (ImGui::Button("Reset Simulation")) {
            solver1.updateGridSize(gridWidth, gridHeight);
            resetSimulationTimer();
        }
        if (ImGui::Button("Step")) {
            if (!simulationRunning) {
                solver1.solveTimeStep1(timeStepSizeSeconds.load());
                accumulatedTime += timeStepSizeSeconds.load();
            }
        }
    }

    void UI::renderViewSelector() {
        ImGui::Text("View Options");
        if (ImGui::RadioButton("Initial Temperature", currentViewMode == VIEW_INITIAL_TEMPERATURE)) {
            currentViewMode = VIEW_INITIAL_TEMPERATURE;
            stopSimulation();
        }
        renderTemperatureSelector();
        if (ImGui::RadioButton("Source", currentViewMode == VIEW_SOURCES)) {
            currentViewMode = VIEW_SOURCES;
            stopSimulation();
        }
        renderSourceSelector();
        if (ImGui::RadioButton("Material", currentViewMode == VIEW_MATERIAL)) {
            currentViewMode = VIEW_MATERIAL;
            stopSimulation();
        }
        renderMaterialSelector();
        if (ImGui::RadioButton("Visualization", currentViewMode == VIEW_VISUALIZATION)) {
            currentViewMode = VIEW_VISUALIZATION;
        }
    }

    void UI::renderGridSizeSelector() {
        ImGui::SliderInt("Grid Width", &gridWidth, 1, 100);  // Min = 1, Max = 100
        ImGui::SliderInt("Grid Height", &gridHeight, 1, 100); // Min = 1, Max = 100

    }

    void UI::renderTemperatureSelector() {
        if (currentViewMode ==
            VIEW_INITIAL_TEMPERATURE) { // Only enable the text box if the temperature checkbox is checked
            ImGui::InputFloat("Temperature (°C)", &selectedTemperatureCelsius);
        }
    }

    void UI::renderSourceSelector() {
        if (currentViewMode == VIEW_SOURCES) { // Only enable the text box if the temperature checkbox is checked
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

    void UI::setControlsStyle() {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, config.windowBackgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Button, config.buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.buttonHoverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.buttonActiveColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config.buttonPadding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.buttonRounding);
    }

    void UI::initializeImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UI::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        solver::Solver &solver1 = solver::Solver::getInstance();
        ui::UI::getInstance().render(*solver1.getElements(), solver1.getGridWidth(), solver1.getGridHeight());
        glfwPollEvents();
    }

    void UI::updateSolverGrid() {
        static int currentWidth = gridWidth;
        static int currentHeight = gridHeight;

        if (currentWidth != gridWidth || currentHeight != gridHeight) {
            solver1.updateGridSize(gridWidth, gridHeight);
            resetSimulationTimer();
            currentWidth = gridWidth;
            currentHeight = gridHeight;
        }
    }

    void APIENTRY UI::glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
                                    GLsizei length, const char *message, const void *userParam) {
        // Ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cerr << "---------------" << std::endl;
        std::cerr << "Debug message (" << id << "): " << message << std::endl;

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                std::cerr << "Source: API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cerr << "Source: Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cerr << "Source: Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cerr << "Source: Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cerr << "Source: Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cerr << "Source: Other";
                break;
        }
        std::cerr << std::endl;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                std::cerr << "Type: Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cerr << "Type: Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cerr << "Type: Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cerr << "Type: Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cerr << "Type: Performance";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cerr << "Type: Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cerr << "Type: Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cerr << "Type: Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cerr << "Type: Other";
                break;
        }
        std::cerr << std::endl;

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cerr << "Severity: high";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cerr << "Severity: medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cerr << "Severity: low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cerr << "Severity: notification";
                break;
        }
        std::cerr << std::endl;
        std::cerr << std::endl;
    }
} // ui