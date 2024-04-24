//
// Created by djcur on 4/18/2024.
//

#include "WindowManager.h"

#include <iostream>

namespace ui {
    WindowManager::WindowManager(int width, int height, const std::string& title)
    : window(nullptr) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW." << std::endl;
            return;
        }
        initializeWindow(width, height, title);
        updateViewport(width, height);
    }

    WindowManager::~WindowManager() {
        terminateWindow();
    }

    void WindowManager::initializeWindow(int width, int height, const std::string& title) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window." << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            glfwDestroyWindow(window);
            glfwTerminate();
            std::cerr << "Failed to initialize GLAD." << std::endl;
            throw std::runtime_error("Failed to initialize GLAD.");
        }
    }

    void WindowManager::terminateWindow() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }

    bool WindowManager::isInitialized() const {
        return window != nullptr;
    }

    bool WindowManager::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void WindowManager::processInput() const {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        glfwPollEvents();
    }

    void WindowManager::updateViewport(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
} // ui