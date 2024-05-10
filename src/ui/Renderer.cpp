//
// Created by djcur on 4/18/2024.
//

#include "Renderer.h"

#include <glfw3.h>
#include <imgui_impl_opengl3.h>

#if defined(_WIN32)
#include <Windows.h>
#endif

#include "GridRenderer.h"
#include "ControlsRenderer.h"
#include "../events/Broker.h"
#include "Roboto-Regular.h"

namespace ui {
    void Renderer::setupFont(ImGuiIO &io) {
        io.Fonts->Clear();

        const float fontSize = config::Config::config.fontSize;
        const float scaleFactor = getScaleFactor();
        ImFont *font = io.Fonts->AddFontFromMemoryTTF(assets_Roboto_Regular_ttf, sizeof(assets_Roboto_Regular_ttf),
                                                      fontSize * scaleFactor);
        if (font == nullptr) {
            throw std::runtime_error("Font not available");
        }

        io.Fonts->Build();
        io.FontDefault = font;
    }

    Renderer::Renderer(GLFWwindow *window) : window(window) {
        simulationController = std::make_shared<SimulationController>();
        std::shared_ptr<IRenderComponent> gridRenderer = std::make_shared<GridRenderer>(window);
        std::shared_ptr<IRenderComponent> controlsRenderer = std::make_shared<ControlsRenderer>(window);
        renderComponents.push_back(gridRenderer);
        renderComponents.push_back(controlsRenderer);

        broker.subscribe(gridRendererEvents, std::dynamic_pointer_cast<events::ISubscriber>(gridRenderer).get());
        broker.subscribe(controlsRendererEvents, std::dynamic_pointer_cast<events::ISubscriber>(controlsRenderer).get());
        broker.subscribe(simulationControllerEvents, std::dynamic_pointer_cast<events::ISubscriber>(simulationController).get());

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        setupFont(io);
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    Renderer::~Renderer() {
        broker.unsubscribe(simulationController.get());
        for (auto &component: renderComponents) {
            broker.unsubscribe(std::dynamic_pointer_cast<events::ISubscriber>(component).get());
        }
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Renderer::initializeImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Renderer::finalizeRender() const {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    void Renderer::render() const {
        initializeImGui();

        for (auto &component: renderComponents) {
            component->render();
        }

        finalizeRender();
    }

    double Renderer::getScaleFactor() {
#if defined(_WIN32)
        HWND hwnd = GetActiveWindow(); // Get a window handle somehow
        UINT dpi = GetDpiForWindow(hwnd);
        return dpi / 96.0; // Default DPI is 96
#elif defined(__APPLE__)
        return 1.0;
#else
        return 1.0;
#endif
    }
} // ui
