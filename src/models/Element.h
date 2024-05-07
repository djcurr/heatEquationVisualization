//
// Created by djcur on 4/11/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_ELEMENT_H
#define HEATEQUATIONVISUALIZATION_ELEMENT_H

#include <array>
#include "Node.h"
#include "Material.h"
#include "../manager/MaterialManager.h"

namespace models {

    /**
     * @class Element
     * @brief Represents an element in the heat equation visualization, typically a part of a finite element mesh.
     *
     * Each element consists of nodes, a material, and parameters like heat source and boundary conditions.
     */
    class Element {
    public:

        /**
         * @brief Constructs an Element with specified nodes and boundary condition.
         * @param nodes Array of Node objects associated with this element.
         * @param isBoundary Boolean flag indicating whether this element is on the boundary of the domain.
         *
         * Initializes the element with default material from the MaterialManager and sets the initial
         * and external heat values to zero.
         */
        Element(const std::array<Node, 4> nodes, bool isBoundary) : nodes(nodes),
                                                                    externalHeat(0),
                                                                    initialHeat(0),
                                                                    is_boundary(isBoundary) {
            material = manager::MaterialManager::getInstance().getDefaultMaterial();
        }

        /**
         * @brief Gets the nodes that make up this element.
         * @return A constant reference to an array of Node objects.
         */
        [[nodiscard]] const std::array<Node, 4>& getNodes() const { return nodes; }

        /**
         * @brief Gets the external heat source affecting this element.
         * @return The external heat source value as a constant reference.
         */
        [[nodiscard]] const double& getHeatSource() const { return externalHeat; }

        /**
         * @brief Gets the material associated with this element.
         * @return The Material object used for this element.
         */
        [[nodiscard]] const Material& getMaterial() const { return material; }

        /**
         * @brief Gets the initial heat of this element in Kelvin.
         * @return The initial heat value as a constant reference.
         */
        [[nodiscard]] const double& getInitialHeatKelvin() const { return initialHeat; }

        /**
         * @brief Checks if this element is a boundary element.
         * @return True if this element is a boundary element, otherwise false.
         */
        [[nodiscard]] const bool& isBoundary() const { return is_boundary; }

        /**
         * @brief Sets the external heat source for this element.
         * @param externalHeat The new external heat source value.
         */
        void setExternalHeat(double externalHeat);

        /**
         * @brief Sets the initial heat of this element in Kelvin.
         * @param initialHeat The new initial heat value in Kelvin.
         */
        void setInitialHeat(double initialHeat);

        /**
         * @brief Sets the material for this element.
         * @param material The new Material object to be associated with this element.
         */
        void setMaterial(const Material& material);

    private:
        std::array<Node, 4> nodes; ///< Nodes that make up the element.
        double externalHeat; ///< External heat source affecting the element.
        double initialHeat; ///< Initial thermal energy of the element in Kelvin.
        bool is_boundary; ///< Indicates if the element is located at the boundary of the domain.
        Material material; ///< Material properties of the element.
    };

}

#endif //HEATEQUATIONVISUALIZATION_ELEMENT_H
