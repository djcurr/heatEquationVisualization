//
// Created by djcur on 4/19/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

class Config {
public:
    struct UIConfig {
        float controlWidth = 300.0f;
        ImVec4 windowBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        ImVec4 buttonColor = ImVec4(0.35f, 0.40f, 0.61f, 0.62f);
        ImVec4 buttonHoverColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        ImVec4 buttonActiveColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
        ImVec2 buttonPadding = ImVec2(10, 10);
        float buttonRounding = 4.0f;
        float kelvinConstant = 273.15f;
    };
    static UIConfig config;
};

#endif //CONFIG_H
