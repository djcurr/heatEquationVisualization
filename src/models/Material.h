//
// Created by djcur on 4/11/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_MATERIAL_H
#define HEATEQUATIONVISUALIZATION_MATERIAL_H

#include <cassert>

namespace models {

    struct Color {
        float red;
        float green;
        float blue;

        Color(float r = 0.0f, float g = 0.0f, float b = 0.0f)
                : red(r), green(g), blue(b) {
        }
    };

    class Material {
    public:
        Material(double density, double thermalConductivity, double specificHeat, Color color)
                : density(density), thermalConductivity(thermalConductivity), specificHeat(specificHeat), color(color) {
            assert(density > 0 && thermalConductivity > 0 && specificHeat > 0);
        }

        Material() : density(0), thermalConductivity(0), specificHeat(0), color() {}

        [[nodiscard]] double getDensity() const { return density; }

        [[nodiscard]] double getThermalConductivity() const { return thermalConductivity; }

        [[nodiscard]] double getSpecificHeat() const { return specificHeat; }

        [[nodiscard]] Color getColor() const { return color; }

    private:
        double density;
        double thermalConductivity;
        double specificHeat;
        Color color;
    };

} // models

#endif //HEATEQUATIONVISUALIZATION_MATERIAL_H
