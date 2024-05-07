//
// Created by djcur on 4/18/2024.
//

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "imgui_impl_glfw.h"
#include <string>

namespace ui {
    /**
     * @class WindowManager
     * @brief Manages the application's window, including its creation, event handling, and destruction.
     *
     * This class is responsible for all interactions with the GLFW windowing library. It handles
     * window creation, processing input, and integrating ImGui with GLFW and OpenGL.
     */
    class WindowManager {
    public:
        /**
         * @brief Constructs a WindowManager with specified dimensions and title.
         * @param width Width of the window.
         * @param height Height of the window.
         * @param title Title of the window.
         *
         * This constructor initializes the window with the given dimensions and title. It sets up
         * necessary configurations for GLFW and ImGui.
         */
        WindowManager(int width, int height, const std::string &title);

        /**
         * @brief Destructor that cleans up the window and related resources.
         *
         * Ensures that GLFW and ImGui are properly terminated and all associated resources are released.
         */
        ~WindowManager();

        /**
         * @brief Checks if the window has been successfully initialized.
         * @return True if the window is initialized, false otherwise.
         */
        [[nodiscard]] bool isInitialized() const;

        /**
         * @brief Checks if the window should close, e.g., if the close button was pressed.
         * @return True if the window should close, otherwise false.
         */
        [[nodiscard]] bool shouldClose() const;

        /**
         * @brief Processes input events for the window.
         *
         * Handles all GLFW input events and updates ImGui state accordingly.
         */
        void processInput() const;

        /**
         * @brief Retrieves the GLFW window handle.
         * @return Pointer to the GLFW window.
         */
        [[nodiscard]] GLFWwindow* getWindow() const { return window; }

    private:
        GLFWwindow *window; ///< Pointer to the GLFW window managed by this class.

        /**
         * @brief Helper function to initialize the GLFW window.
         * @param width Width of the window.
         * @param height Height of the window.
         * @param title Title of the window.
         *
         * Sets up GLFW, configures the window, and initializes ImGui contexts.
         */
        void initializeWindow(int width, int height, const std::string &title);

        /**
         * @brief Cleans up the window and associated resources.
         *
         * Properly terminates GLFW and ImGui upon window closure or destruction of this class.
         */
        void terminateWindow();

        /**
         * @brief Callback function to update the viewport when the framebuffer is resized.
         * @param window GLFW window that received the event.
         * @param width New width of the framebuffer.
         * @param height New height of the framebuffer.
         *
         * Ensures the OpenGL viewport matches the new window dimensions.
         */
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        /**
         * @brief Static method to handle viewport updates during window resize events.
         * @param width New width of the window.
         * @param height New height of the window.
         *
         * Adjusts the OpenGL viewport settings to match the new window size.
         */
        static void updateViewport(int width, int height);
    };
} // namespace ui

#endif //WINDOWMANAGER_H
