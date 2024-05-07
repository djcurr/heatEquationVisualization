//
// Created by djcur on 4/19/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <imgui.h>
#include <Eigen/Eigen>
#include "../events/EventTypes.h"
#include "../ui/ColorMap.h"

namespace config {
    /**
     * @class Config
     * @brief Holds configuration settings for the application.
     *
     * This class contains nested structures that define various configuration settings,
     * including UI parameters and simulation parameters.
     */
    class Config {
    public:
        /**
         * @struct UIConfig
         * @brief Structure containing all user interface configuration settings.
         *
         * This structure defines the aesthetics and some functional aspects of the UI,
         * such as colors, spacings, and dimensions for various UI components.
         */
        struct UIConfig {
            float controlWidth = 700.0f; ///< Width of the UI controls pane in pixels
            ImVec4 windowBackgroundColor = ImVec4(0.15f, 0.15f, 0.20f, 1.0f); ///< Background color of the windows.
            ImVec4 buttonColor = ImVec4(0.2f, 0.4f, 0.8f, 1.0f); ///< Normal state button color.
            ImVec4 buttonHoverColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); ///< Hover state button color.
            ImVec4 buttonActiveColor = ImVec4(0.1f, 0.1f, 0.1f, 0.10f); ///< Active state button color.
            ImVec2 buttonPadding = ImVec2(10, 10); ///< Padding inside buttons.
            ImVec2 windowPadding = ImVec2(0.0f, 0.0f); ///< Padding inside windows.
            ImVec2 itemSpacing = ImVec2(0.0f, 0.0f); ///< Spacing between items within a window.
            float buttonRounding = 4.0f; ///< Rounding radius of button corners.
            float kelvinConstant = 273.15f; ///< Constant for converting Celsius to Kelvin.

            int gridWidth = 150; ///< Default width of the simulation grid.
            int gridHeight = 150; ///< Default height of the simulation grid.
            ViewMode view_mode = ViewMode::VIEW_INITIAL_TEMPERATURE; ///< Default view mode.
            int brushSize = 5; ///< Default brush size for interactive elements.
            int timeStepSizeSeconds = 100000; ///< Default timestep size in seconds.
            int targetDurationSeconds = 1000000; ///< Target duration of the simulation in seconds.
            std::string materialName = "Aluminum"; ///< Default material name.
            int maxGridHeight = 500; ///< Maximum grid height allowed.
            int maxGridWidth = 500; ///< Maximum grid width allowed.
            float fontSize = 16.0f; ///< Default font size for UI text.
            ColorMaps colorMap = ColorMaps::PLASMA; ///< Default color map for rendering.
        };

        static UIConfig config; ///< Static instance of UIConfig used throughout the application.
    };
}

#endif //CONFIG_H
