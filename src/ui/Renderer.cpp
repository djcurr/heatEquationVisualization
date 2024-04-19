//
// Created by djcur on 4/18/2024.
//

#include "Renderer.h"

#include <mutex>

#include "../solver/Solver.h"

namespace ui {
    Renderer::Renderer(GLFWwindow *window) : window(window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    Renderer::~Renderer() {
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

    void Renderer::render(const std::vector<models::Element> &elements) {
        initializeImGui();
        renderControls();
        renderGrid(elements);
        finalizeRender();
    }
} // ui
