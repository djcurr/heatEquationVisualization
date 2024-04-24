//
// Created by djcur on 4/20/2024.
//

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H
#include <imgui.h>
#include <variant>
#include <vector>

namespace ui {
    class StyleManager {
    public:
        static void applyStyle(const std::vector<std::pair<ImGuiCol, ImVec4> > &colors,
                               const std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2>>> &vars);

        static void removeStyle(int colorCount, int varCount);
    };
} // ui

#endif //STYLEMANAGER_H
