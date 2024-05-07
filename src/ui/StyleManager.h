//
// Created by djcur on 4/20/2024.
//

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <imgui.h>
#include <variant>
#include <vector>

namespace ui {
    /**
     * @class StyleManager
     * @brief Manages styling for ImGui elements.
     *
     * Provides static methods to apply and remove styles for ImGui elements, simplifying
     * the process of pushing and popping style configurations.
     */
    class StyleManager {
    public:
        /**
         * @brief Applies a collection of color and variable styles to ImGui.
         * @param colors Vector of pairs, each containing an ImGuiCol enum and an ImVec4 color value.
         * @param vars Vector of pairs, each containing an ImGuiStyleVar enum and a value which can be either float or ImVec2.
         *
         * This method pushes styles onto ImGui's style stack, allowing subsequent ImGui calls to use these styles.
         */
        static void applyStyle(const std::vector<std::pair<ImGuiCol, ImVec4>>& colors,
                               const std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2>>>& vars);

        /**
         * @brief Removes a specified number of color and variable styles from ImGui.
         * @param colorCount The number of color styles to pop.
         * @param varCount The number of variable styles to pop.
         *
         * This method pops styles from ImGui's style stack, reverting to the previous style settings.
         */
        static void removeStyle(int colorCount, int varCount);
    };
} // namespace ui

#endif //STYLEMANAGER_H
