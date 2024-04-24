//
// Created by djcur on 4/23/2024.
//

#ifndef COLORMAP_H
#define COLORMAP_H
#include <imgui.h>
#include <string>

class ColorMap {
public:
    [[nodiscard]] virtual ImVec4 getColor(float value) const = 0;

    virtual ~ColorMap() = default;
};

class ViridisColorMap final : public ColorMap {
public:
    [[nodiscard]] ImVec4 getColor(float value) const override {
        // Simplified example RGB data for Viridis
        static const std::vector<std::tuple<float, float, float>> colors = {
            {0.267004, 0.004874, 0.329415},
            {0.229739, 0.322361, 0.545706},
            {0.127568, 0.566949, 0.550556},
            {0.369214, 0.788888, 0.382914},
            {0.993248, 0.906157, 0.143936}
        };
        int index = static_cast<int>(value * (colors.size() - 1));
        float fraction = (value * (colors.size() - 1)) - index;

        // Linear interpolation between the selected and next color
        auto [r1, g1, b1] = colors[index];
        auto [r2, g2, b2] = colors[std::min(index + 1, static_cast<int>(colors.size() - 1))];

        float red = r1 + fraction * (r2 - r1);
        float green = g1 + fraction * (g2 - g1);
        float blue = b1 + fraction * (b2 - b1);

        return ImVec4(red, green, blue, 1.0);
    }
};

class PlasmaColorMap final : public ColorMap {
public:
    [[nodiscard]] ImVec4 getColor(float value) const override {
        // Simplified example RGB data for Plasma
        static const std::vector<std::tuple<float, float, float>> colors = {
            {0.050383, 0.029803, 0.527975},
            {0.492388, 0.011943, 0.657209},
            {0.798216, 0.280197, 0.469538},
            {0.973381, 0.585395, 0.25154},
            {0.940015, 0.975158, 0.131326}
        };
        int index = static_cast<int>(value * (colors.size() - 1));
        float fraction = (value * (colors.size() - 1)) - index;

        // Linear interpolation between the selected and next color
        auto [r1, g1, b1] = colors[index];
        auto [r2, g2, b2] = colors[std::min(index + 1, static_cast<int>(colors.size() - 1))];

        float red = r1 + fraction * (r2 - r1);
        float green = g1 + fraction * (g2 - g1);
        float blue = b1 + fraction * (b2 - b1);

        return ImVec4(red, green, blue, 1.0);
    }
};

enum class ColorMaps {
    VIRIDIS,
    PLASMA
};

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
