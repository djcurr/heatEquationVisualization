//
// Created by djcur on 4/23/2024.
//

#ifndef COLORMAP_H
#define COLORMAP_H
#include <imgui.h>
#include <string>
#include <cmath>
#include <tuple>

/**
 * @class ColorMap
 * @brief Abstract base class for color maps that convert a scalar value to a color.
 *
 * This class defines the interface for color maps used in data visualization, where scalar values are
 * represented as colors.
 */
class ColorMap {
public:
    /**
     * @brief Virtual destructor for the ColorMap class.
     */
    virtual ~ColorMap() = default;

    /**
     * @brief Retrieves the color corresponding to a given value.
     * @param value A floating-point value typically between 0 and 1, representing a position within the color map.
     * @return The color as an ImVec4 (RGBA) corresponding to the input value.
     */
    [[nodiscard]] virtual ImVec4 getColor(double value) const = 0;
};

/**
 * @class ViridisColorMap
 * @brief A specific implementation of ColorMap that uses the Viridis color scheme.
 *
 * Viridis is a perceptually-uniform color map, which is better suited for accurately reading quantitative data visually.
 */
class ViridisColorMap final : public ColorMap {
public:
    ViridisColorMap() {
        precomputeColors();
    }

    /**
     * @brief Gets the color from the Viridis color map corresponding to the given value.
     * @param value A normalized value between 0 and 1, where 0 typically represents the start of the color map.
     * @return The color as an ImVec4 object.
     */
    [[nodiscard]] ImVec4 getColor(double value) const override {
        int index = static_cast<int>(std::round(value * (static_cast<int>(precomputedColors.size()) - 1)));
        return precomputedColors[index];
    }

private:
    std::vector<ImVec4> precomputedColors; ///< Compute a gradient of colors to use

    void precomputeColors() {
        static const std::vector<std::tuple<double, double, double>> baseColors = {
            {0.267004, 0.004874, 0.329415},
            {0.229739, 0.322361, 0.545706},
            {0.127568, 0.566949, 0.550556},
            {0.369214, 0.788888, 0.382914},
            {0.993248, 0.906157, 0.143936}
        };
        int numSteps = 256; // Increase for finer gradients
        precomputedColors.resize(numSteps);

        for (int i = 0; i < numSteps; ++i) {
            float value = static_cast<float>(i) / static_cast<float>(numSteps - 1);
            int index = static_cast<int>(value * (static_cast<float>(baseColors.size()) - 1));
            float fraction = (value * (static_cast<float>(baseColors.size()) - 1)) - static_cast<float>(index);
            auto [r1, g1, b1] = baseColors[index];
            auto [r2, g2, b2] = baseColors[std::min(index + 1, static_cast<int>(baseColors.size() - 1))];

            float red = r1 + fraction * (r2 - r1);
            float green = g1 + fraction * (g2 - g1);
            float blue = b1 + fraction * (b2 - b1);
            precomputedColors[i] = {red, green, blue, 1.0};
        }
    }
};

/**
 * @class PlasmaColorMap
 * @brief A specific implementation of ColorMap that uses the Plasma color scheme.
 *
 * Plasma is a color map that ranges from blue to red, offering good perceptual uniformity and high contrast.
 */
class PlasmaColorMap final : public ColorMap {
public:
    PlasmaColorMap() {
        precomputeColors();
    }
    /**
     * @brief Gets the color from the Plasma color map corresponding to the given value.
     * @param value A normalized value between 0 and 1, where 0 typically represents the start of the color map.
     * @return The color as an ImVec4 object.
     */
    [[nodiscard]] ImVec4 getColor(double value) const override {
        int index = static_cast<int>(std::round(value * (static_cast<int>(precomputedColors.size()) - 1)));
        return precomputedColors[index];
    }

private:
    std::vector<ImVec4> precomputedColors; ///< Compute a gradient of colors to use

    void precomputeColors() {
        static const std::vector<std::tuple<double, double, double>> baseColors = {
            {0.050383, 0.029803, 0.527975},
            {0.492388, 0.011943, 0.657209},
            {0.798216, 0.280197, 0.469538},
            {0.973381, 0.585395, 0.25154},
            {0.940015, 0.975158, 0.131326}
        };
        int numSteps = 256; // Increase for finer gradients
        precomputedColors.resize(numSteps);

        for (int i = 0; i < numSteps; ++i) {
            float value = static_cast<float>(i) / static_cast<float>(numSteps - 1);
            int index = static_cast<int>(value * (static_cast<float>(baseColors.size()) - 1));
            float fraction = (value * (static_cast<float>(baseColors.size()) - 1)) - static_cast<float>(index);
            auto [r1, g1, b1] = baseColors[index];
            auto [r2, g2, b2] = baseColors[std::min(index + 1, static_cast<int>(baseColors.size() - 1))];

            float red = r1 + fraction * (r2 - r1);
            float green = g1 + fraction * (g2 - g1);
            float blue = b1 + fraction * (b2 - b1);
            precomputedColors[i] = {red, green, blue, 1.0};
        }
    }
};

/**
 * @enum ColorMaps
 * @brief Enumerates the available color map types.
 */
enum class ColorMaps {
    VIRIDIS, ///< Represents the Viridis color map.
    PLASMA ///< Represents the Plasma color map.
};

/**
 * @brief Converts a ColorMaps enum value to its corresponding name string.
 * @param c The ColorMaps enum value.
 * @return A string representing the name of the color map.
 */
std::string inline getColorMapName(ColorMaps c) {
    switch (c) {
        case ColorMaps::VIRIDIS:
            return "Viridis";
        case ColorMaps::PLASMA:
            return "Plasma";
    }
    return "Unknown";
}
#endif //COLORMAP_H
