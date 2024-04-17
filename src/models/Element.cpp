//
// Created by djcur on 4/11/2024.
//

#include "Element.h"

namespace models {
    void Element::setExternalHeat(double externalHeat) {
        Element::externalHeat = externalHeat;
    }

    void Element::setInitialHeat(double initialHeat) {
        Element::initialHeat = initialHeat;
    }

    void Element::setMaterial(const Material &material) {
        Element::material = material;
    }
} // models