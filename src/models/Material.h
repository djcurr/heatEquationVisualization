//
// Created by djcur on 4/11/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_MATERIAL_H
#define HEATEQUATIONVISUALIZATION_MATERIAL_H

#include <cassert>

namespace models {

    /**
     * @struct Color
     * @brief Represents a color using RGB components.
     *
     * Each component of the color (red, green, blue) is represented as a float,
     * typically ranging from 0.0 to 1.0.
     */
    struct Color {
        float red; ///< Red component of the color.
        float green; ///< Green component of the color.
        float blue; ///< Blue component of the color.

        /**
         * @brief Constructs a Color with specified red, green, and blue values.
         * @param r The red component (default 0.0).
         * @param g The green component (default 0.0).
         * @param b The blue component (default 0.0).
         *
         * Initializes the color with the specified RGB values.
         */
        explicit Color(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f)
            : red(r), green(g), blue(b) {}
    };

    /**
     * @class Material
     * @brief Represents a material with physical properties and an associated color.
     *
     * A material is defined by its density, thermal conductivity, specific heat,
     * and color. The class ensures that the physical properties are positive values
     * using assertions.
     */
    class Material {
    public:
        /**
         * @brief Constructs a Material with specified properties.
         * @param density The density of the material (kg/m^3).
         * @param thermalConductivity The thermal conductivity of the material (W/m·K).
         * @param specificHeat The specific heat capacity of the material (J/kg·K).
         * @param color The visual color representation of the material.
         *
         * Initializes the material with the provided physical properties and color.
         * All physical properties must be positive; assertions ensure this during development.
         */
        Material(const double density, const double thermalConductivity, const double specificHeat, const Color color)
            : density(density), thermalConductivity(thermalConductivity), specificHeat(specificHeat), color(color) {
            assert(density > 0 && thermalConductivity > 0 && specificHeat > 0);
        }

        /**
         * @brief Default constructor for Material.
         *
         * Initializes the material with zero values for all physical properties and default color.
         */
        Material() : density(0), thermalConductivity(0), specificHeat(0), color() {}

        /**
         * @brief Gets the density of the material.
         * @return The density of the material in kilograms per cubic meter.
         */
        [[nodiscard]] double getDensity() const { return density; }

        /**
         * @brief Gets the thermal conductivity of the material.
         * @return The thermal conductivity of the material in watts per meter kelvin.
         */
        [[nodiscard]] double getThermalConductivity() const { return thermalConductivity; }

        /**
         * @brief Gets the specific heat of the material.
         * @return The specific heat of the material in joules per kilogram kelvin.
         */
        [[nodiscard]] double getSpecificHeat() const { return specificHeat; }

        /**
         * @brief Gets the color of the material.
         * @return The color associated with the material.
         */
        [[nodiscard]] Color getColor() const { return color; }

    private:
        double density; ///< Density of the material (kg/m^3).
        double thermalConductivity; ///< Thermal conductivity of the material (W/m·K).
        double specificHeat; ///< Specific heat capacity of the material (J/kg·K).
        Color color; ///< Visual color representation of the material.
    };

}

#endif //HEATEQUATIONVISUALIZATION_MATERIAL_H
