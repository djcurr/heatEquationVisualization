//
// Created by djcur on 4/20/2024.
//

#include "StyleManager.h"

#include <variant>

namespace ui {
    void StyleManager::applyStyle(const std::vector<std::pair<ImGuiCol, ImVec4>>& colors,
                           const std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2>>>& vars) {
        for (auto& color : colors) {
            ImGui::PushStyleColor(color.first, color.second);
        }
        for (auto& var : vars) {
            if (std::holds_alternative<float>(var.second)) {
                ImGui::PushStyleVar(var.first, std::get<float>(var.second));
            } else if (std::holds_alternative<ImVec2>(var.second)) {
                ImGui::PushStyleVar(var.first, std::get<ImVec2>(var.second));
            }
        }

    }

    void StyleManager::removeStyle(int colorCount, int varCount) {
        ImGui::PopStyleVar(varCount);
        ImGui::PopStyleColor(colorCount);
    };
} // ui