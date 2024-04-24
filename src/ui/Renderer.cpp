//
// Created by djcur on 4/18/2024.
//

#include "Renderer.h"

#include <mutex>

#include "GridRenderer.h"
#include "ControlsRenderer.h"
#include "../events/Broker.h"

namespace ui {
    Renderer::Renderer(GLFWwindow *window) : window(window) {
        simulationController = std::make_shared<SimulationController>();
        renderComponents.push_back(std::make_shared<GridRenderer>(window));
        renderComponents.push_back(std::make_shared<ControlsRenderer>(window));

        // Subscribe the controller and renderers to the event broker
        for (auto &component: renderComponents) {
            broker.subscribe(std::dynamic_pointer_cast<events::ISubscriber>(component).get());
        }
        broker.subscribe(simulationController.get());

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        float fontSize = config::Config::config.fontSize;
        io.Fonts->AddFontDefault()->Scale = fontSize / 13.0f;
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

    void Renderer::finalizeRender() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    void Renderer::render() {
        initializeImGui();

        for (auto &component: renderComponents) {
            component->render();
        }

        finalizeRender();
    }
} // ui
