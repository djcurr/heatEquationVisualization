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
        Material::Material(double density, double thermalConductivity, double specificHeat, Color color)
                : density(density), thermalConductivity(thermalConductivity), specificHeat(specificHeat), color(color) {
            assert(density > 0 && thermalConductivity > 0 && specificHeat > 0);
        }

        Material::Material() : density(0), thermalConductivity(0), specificHeat(0), color() {}

        double getDensity() const { return density; }

        double getThermalConductivity() const { return thermalConductivity; }

        double getSpecificHeat() const { return specificHeat; }

        Color getColor() const { return color; }

    private:
        double density;
        double thermalConductivity;
        double specificHeat;
        Color color;
    };

} // models

#endif //HEATEQUATIONVISUALIZATION_MATERIAL_H
