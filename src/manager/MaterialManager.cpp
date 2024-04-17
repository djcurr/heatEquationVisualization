//
// Created by djcur on 4/13/2024.
//

#include <vector>
#include "MaterialManager.h"

namespace manager {
    // Methods to add and get materials
    void MaterialManager::addMaterial(const std::string& name, const models::Material& material) {
        materials[name] = material;
    }

    const models::Material& MaterialManager::getMaterial(const std::string& name) const {
        auto it = materials.find(name);
        assert(it != materials.end());
        return it->second;
    }

    const models::Material& MaterialManager::getDefaultMaterial() const {
        auto it = materials.find("Copper");
        assert(it != materials.end());
        return it->second;
    }

    // Initialize with default materials
    void MaterialManager::initializeDefaults() {
        materials["Steel"] = models::Material(7850, 50, 500, models::Color(113, 121, 126));
        materials["Aluminum"] = models::Material(2700, 237, 897, models::Color(132, 135, 137));
        materials["Copper"] = models::Material(8960, 401, 385, models::Color(184, 115, 51));
    }

    std::vector<std::string> MaterialManager::getAllMaterialNames() const {
        std::vector<std::string> names;
        for (const auto& pair : materials) {
            names.push_back(pair.first);
        }
        return names;
    }
} // manager