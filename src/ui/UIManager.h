//
// Created by djcur on 4/18/2024.
//

#ifndef UIMANAGER_H
#define UIMANAGER_H
#include "Renderer.h"

namespace ui {
    /**
        * @class UIManager
        * @brief Manages the main user interface components and interactions.
        *
        * This class is responsible for managing all UI-related activities, including window management
        * and rendering operations. It integrates the functionality provided by WindowManager and Renderer
        * to facilitate the UI operations in a centralized manner.
        */
    class UIManager {
    public:
        /**
         * @brief Constructs a UIManager with initial window dimensions and title.
         * @param width Initial width of the window.
         * @param height Initial height of the window.
         * @param title The title of the window.
         *
         * Initializes the window manager and renderer, setting up the primary window with the specified
         * dimensions and title.
         */
        UIManager(int width, int height, const std::string &title);

        /**
         * @brief Performs necessary updates to the UI components.
         *
         * This method should be called regularly and will handle all updates necessary for the UI,
         * including processing window events and other interactions.
         */
        void update() const;

        /**
         * @brief Renders the UI components.
         *
         * This method handles the rendering of all managed UI components. It delegates the rendering tasks
         * to the Renderer class.
         */
        void render() const;

        /**
         * @brief Checks if the window should be closed.
         * @return True if the window should close, otherwise false.
         *
         * Queries the window manager to determine if the window close event has been triggered,
         * such as by the user clicking the window's close button.
         */
        [[nodiscard]] bool shouldClose() const { return windowManager->shouldClose(); }

    private:
        std::unique_ptr<WindowManager> windowManager; ///< Manages the windowing system interactions.
        std::unique_ptr<Renderer> renderer; ///< Handles rendering of all UI components.
    };
} // ui

#endif //UIMANAGER_H
