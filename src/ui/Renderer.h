//
// Created by djcur on 4/18/2024.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "IRenderComponent.h"
#include "SimulationController.h"
#include "WindowManager.h"
#include "../events/Broker.h"

namespace ui {
    /**
     * @class Renderer
     * @brief Manages and coordinates rendering of all UI components within the application.
     *
     * This class serves as the central rendering manager, handling the setup, execution,
     * and finalization of rendering operations for all registered IRenderComponent instances.
     * It interfaces with the window management system to ensure that rendering is synchronized
     * with the application's main event loop.
     */
    class Renderer {
    public:
        /**
         * @brief Constructs the Renderer with a specific window.
         * @param window The GLFWwindow where the rendering will be performed.
         *
         * This constructor initializes the rendering context for the specified window and sets up
         * necessary configurations.
         */
        explicit Renderer(GLFWwindow *window);

        /**
         * @brief Destructor that cleans up resources.
         *
         * Ensures that all rendering components are properly cleaned up and that the rendering
         * state is finalized before the renderer is destroyed.
         */
        ~Renderer();

        /**
         * @brief Executes the rendering process for all registered components.
         *
         * This method is called typically in the application's main loop, and it executes
         * the rendering operations for each component registered with the renderer.
         */
        void render() const;

        /**
         * @brief Determines the scale factor to use based on DPI scaling (Windows only)
         * @return The scale factor to use
         */
        static double getScaleFactor();

    private:
        /**
         * @brief Initializes the ImGui rendering context.
         *
         * This static method sets up ImGui specifically for use in the provided windowing
         * system, configuring global state as needed.
         */
        static void initializeImGui();

        /**
         * @brief Finalizes the rendering process.
         *
         * Cleans up rendering state after all components have been rendered, ensuring
         * the graphics context is left in a consistent state.
         */
        void finalizeRender() const;

        /**
         * @brief Loads the default font and applies DPI and size scaling.
         * @param io The ImGui IO instance
         */
        static void setupFont(ImGuiIO &io);


        GLFWwindow *window; ///< Pointer to the GLFW window used for rendering.
        /// Reference to the event broker for managing system-wide events.
        events::Broker &broker = events::Broker::getInstance();
        /// List of components that the renderer manages.
        std::vector<std::shared_ptr<IRenderComponent> > renderComponents;
        /// Controller for managing simulation state and logic.
        std::shared_ptr<SimulationController> simulationController;
        std::vector<EventType> simulationControllerEvents = {
            EventType::StartSimulation,
            EventType::ResetSimulation,
            EventType::GridWidthChange,
            EventType::GridHeightChange,
            EventType::ApplyMaterial,
            EventType::ApplyInitialTemperature,
            EventType::ApplySource,
            EventType::MaterialChangeEvent,
            EventType::TemperatureInputEvent,
            EventType::SourceChangeEvent,
            EventType::TimeDurationChangeEvent,
            EventType::TimeStepSizeChangeEvent,
            EventType::TimeStepChangeEvent
        }; ///< Events used by the simulation controller
        std::vector<EventType> gridRendererEvents = {
            EventType::BrushSizeChangeEvent,
            EventType::ViewChange,
            EventType::ElementsUpdate,
            EventType::SimulationTemperatureUpdate,
            EventType::ColorMapChange
        }; ///< Events used by the grid renderer
        std::vector<EventType> controlsRendererEvents = {
            EventType::SimulationCompleted,
            EventType::SimulationRunning,
            EventType::TimeStepChangeEvent,
            EventType::ResetSimulation,
            EventType::TimeStepCompleted,
            EventType::StartSimulation
        }; ///< Events used by the controls renderer
    };
} // namespace ui

#endif //RENDERER_H
