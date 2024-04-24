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

    class Element {
    public:

        Element(const std::array<Node, 4> nodes, bool isBoundary) : nodes(nodes),
                                                                             externalHeat(0),
                                                                             initialHeat(0),
                                                                             is_boundary(isBoundary) {
            material = manager::MaterialManager::getInstance().getDefaultMaterial();
        }

        const std::array<Node, 4> &getNodes() const { return nodes; }

        const double &getHeatSource() const { return externalHeat; }

        const Material &getMaterial() const { return material; }

        const double &getInitialHeatKelvin() const { return initialHeat; }

        const bool &isBoundary() const { return is_boundary; }

        void setExternalHeat(double externalHeat);

        void setInitialHeat(double initialHeat);

        void setMaterial(const Material &material);

    private:
        std::array<Node, 4> nodes;
        double externalHeat;
        double initialHeat;
        bool is_boundary;
        Material material;
    };

} // models

#endif //HEATEQUATIONVISUALIZATION_ELEMENT_H
