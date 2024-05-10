//
// Created by djcur on 4/19/2024.
//

#ifndef CONTROLSRENDERER_H
#define CONTROLSRENDERER_H

#include <vector>

#include "ColorMap.h"
#include "../config/Config.h"
#include "IRenderComponent.h"
#include "WindowManager.h"
#include "../events/Broker.h"
#include "../events/ISubscriber.h"

namespace ui {
    /**
     * @class ControlsRenderer
     * @brief Renders UI controls and handles user interactions for simulation settings.
     *
     * This class is responsible for rendering user interface elements and responding to user inputs,
     * updating simulation parameters based on user interaction. It subscribes to system-wide events
     * to update its state accordingly.
     */
    class ControlsRenderer final : public IRenderComponent, public events::ISubscriber {
    public:
        /**
         * @brief Constructs a ControlsRenderer attached to a specific GLFW window.
         * @param window Pointer to the GLFWwindow where the UI controls will be rendered.
         */
        explicit ControlsRenderer(GLFWwindow *window) : window(window) {
        }

        /**
         * @brief Render the controls to the associated GLFW window.
         */
        void render() override;

        static void setWindow();

       private:
        GLFWwindow *window; ///< The window where controls are rendered.

        // State variables
        int currentViewTimeStep = 0; ///< Current timestep displayed in the UI.
        float selectedTemperatureCelsius = 0; ///< Currently selected temperature in Celsius for input.
        int gridWidth = config::Config::config.gridWidth; ///< Current grid width from the configuration.
        int gridHeight = config::Config::config.gridHeight; ///< Current grid height from the configuration.
        float selectedSource = 0; ///< Currently selected source intensity for input.
        int selectedMaterialIndex = 0; ///< Index of the currently selected material.
        int brushSize = config::Config::config.brushSize; ///< Brush size for painting on the grid.
        int timeStepSizeSeconds = config::Config::config.timeStepSizeSeconds; ///< Time step size in seconds.
        /// Target duration of the simulation in seconds.
        int targetDurationSeconds = config::Config::config.targetDurationSeconds;
        bool simulationComplete = false; ///< Flag indicating whether the simulation has completed.
        bool simulationRunning = false; ///< Flag indicating whether the simulation is currently running.
        /// Event broker for publishing and subscribing to events.
        events::Broker &broker = events::Broker::getInstance();
        int timeStepsCompleted = 0; ///< Count of completed timesteps in the current or last simulation.
        ColorMaps currentColorMap = config::Config::config.colorMap; ///< Currently selected color map for rendering.
        ViewMode currentView = config::Config::config.view_mode; ///< Currently selected view mode.


        /// List of colors used for different UI elements.
        std::vector<std::pair<ImGuiCol, ImVec4> > controlColors{
            {ImGuiCol_WindowBg, config::Config::config.windowBackgroundColor},
            {ImGuiCol_Button, config::Config::config.buttonColor},
            {ImGuiCol_ButtonHovered, config::Config::config.buttonHoverColor},
            {ImGuiCol_ButtonActive, config::Config::config.buttonActiveColor}
        };
        std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2> > > controlVars{
            {ImGuiStyleVar_FramePadding, config::Config::config.buttonPadding},
            {ImGuiStyleVar_FrameRounding, config::Config::config.buttonRounding}
        };

        int maxTimeSteps = 1; ///< Max steps allowed in the slider.

        /**
         * @brief Renders all control elements associated with simulation settings.
         */
        void renderControls();

        /**
         * @brief Renders buttons used for control actions like starting or stopping simulations.
         */
        void renderControlsButtons();

        /**
         * @brief Changes the current view mode based on user interaction.
         * @param view The new view mode to set.
         */
        void changeView(ViewMode view);

        /**
         * @brief Renders a view selector for switching between different simulation visualizations.
         */
        void renderViewSelector();

        /**
         * @brief Renders a slider for selecting and displaying the current time step.
         */
        void renderTimeStepSlider();

        /**
         * @brief Renders selectors for adjusting the grid dimensions.
         */
        void renderGridSizeSelector();

        /**
         * @brief Renders settings for selecting color maps used in visualization.
         */
        void renderColorSettings();

        /**
         * @brief Renders a temperature selector for setting initial conditions.
         */
        void renderTemperatureSelector();

        /**
         * @brief Renders a source selector for setting heat sources in the simulation.
         */
        void renderSourceSelector();

        /**
         * @brief Renders a material selector for choosing materials used in the simulation.
         */
        void renderMaterialSelector();

        /**
         * @brief Renders a progress bar showing the completion status of the simulation.
         */
        void renderProgressBar() const;

        /**
         * @brief Renders a slider for selecting the brush size used in interactive elements.
         */
        void renderBrushSizeSlider();

        /**
         * @brief Renders an input field for setting the time step size.
         */
        void renderTimeStepInput();

        /**
         * @brief Handles the rendering of radio buttons and updates the view mode if changed.
         * @param label The label for the radio button.
         * @param targetView The view mode that the radio button corresponds to.
         * @param condition A condition under which the radio button is considered active.
         * @return True if the radio button is selected, False otherwise.
         */
        bool handleRadioButton(const std::string &label, ViewMode targetView, bool condition = true);

        /**
         * @brief Publishes an event if a value has changed.
         * @tparam T The type of the value being monitored.
         * @tparam Event The event type to publish if there is a change.
         * @tparam Args The types of additional arguments to pass to the event constructor.
         * @param oldValue Reference to the old value for comparison.
         * @param newValue The new value to update.
         * @param args Additional arguments for the event constructor.
         */
        template<class T, class Event, class... Args>
        void publishIfChanged(T &oldValue, T newValue, Args &&... args);

        /**
         * @brief Handles button press events and can change views or publish events based on conditions.
         * @tparam Event The type of event to publish on button press.
         * @param label The text label of the button.
         * @param size The size of the button.
         * @param checkView The view mode to check against before taking action.
         * @param changeToView The view mode to change to upon button press.
         * @param event The event to publish when the button is pressed.
         */
        template<typename Event>
        void handleButton(const std::string &label, const ImVec2 &size, ViewMode checkView,
                          ViewMode changeToView, Event event);

        /**
         * @brief Renders a standard input widget for various types of data.
         * @tparam T The type of data being input.
         * @tparam Event The type of event to trigger on change.
         * @tparam Args The types of additional arguments to pass to the event constructor.
         * @param label The label for the input widget.
         * @param value The current value (modifiable).
         * @param args Additional arguments for the event constructor.
         */
        template<typename T, typename Event, typename... Args>
        void renderStandardInput(const std::string &label, T &value, Args &&... args);

        /**
         * @brief Renders a slider input widget for numerical data.
         * @tparam T The type of data being adjusted.
         * @tparam Event The type of event to trigger on change.
         * @tparam Args The types of additional arguments to pass to the event constructor.
         * @param label The label for the slider.
         * @param value The current value (modifiable).
         * @param min The minimum value for the slider.
         * @param max The maximum value for the slider.
         * @param args Additional arguments for the event constructor.
         */
        template<typename T, typename Event, typename... Args>
        void renderSliderInput(const std::string &label, T &value, T min, T max, Args &&... args);

        /**
         * @brief Responds to events published by other components of the application.
         * @param event The event object shared from the publisher.
         */
        void onEvent(const std::shared_ptr<events::Event> &event) override;

        static bool inputHelper(const std::string &label, float *value);

        static bool inputHelper(const std::string &label, int *value);

        static bool inputHelper(const std::string &label, int *value, int min, int max);

        static bool inputHelper(const std::string &label, float *value, float min, float max);
    };
} // namespace ui

#endif //CONTROLSRENDERER_H
