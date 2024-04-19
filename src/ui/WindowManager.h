//
// Created by djcur on 4/18/2024.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#include <glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>

namespace ui {
    class WindowManager {
    public:
        WindowManager(int width, int height, const std::string& title);
        ~WindowManager();

        bool isInitialized() const;

        bool shouldClose() const;

        void processInput();

        static void updateViewport(int width, int height);

        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

    private:
        GLFWwindow *window;
        void initializeWindow(int width, int height, const std::string& title);
        void terminateWindow();
    };
} // ui

#endif //WINDOWMANAGER_H
