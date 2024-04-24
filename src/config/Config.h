//
// Created by djcur on 4/19/2024.
//

#ifndef CONFIG_H
#define CONFIG_H
#include <imgui.h>
#include <Eigen>
#include "../events/EventTypes.h"
#include "../ui/ColorMap.h"


enum class ColorMaps;

namespace config {
    class Config {
    public:
        struct UIConfig {
            float controlWidth = 600.0f;
            ImVec4 windowBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            ImVec4 buttonColor = ImVec4(0.35f, 0.40f, 0.61f, 0.62f);
            ImVec4 buttonHoverColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
            ImVec4 buttonActiveColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
            ImVec2 buttonPadding = ImVec2(10, 10);
            ImVec2 windowPadding = ImVec2(0.0f, 0.0f);
            ImVec2 itemSpacing = ImVec2(0.0f, 0.0f);
            float buttonRounding = 4.0f;
            float kelvinConstant = 273.15f;

            int gridWidth = 150;
            int gridHeight = 150;
            ViewMode view_mode = ViewMode::VIEW_INITIAL_TEMPERATURE;
            int brushSize = 3;
            Eigen::VectorXd simulationTemperature = Eigen::VectorXd::Zero(0);
            int timeStepSizeSeconds = 100000;
            int targetDurationSeconds = 1000000;
            std::string materialName = "Copper";
            int maxGridHeight = 500;
            int maxGridWidth = 500;
            float fontSize = 20.0f;
            ColorMaps colorMap = ColorMaps::PLASMA;
        };

        static UIConfig config;
    };
}
#endif //CONFIG_H
