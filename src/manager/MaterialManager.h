//
// Created by djcur on 4/13/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_MATERIALMANAGER_H
#define HEATEQUATIONVISUALIZATION_MATERIALMANAGER_H

#include <string>
#include <map>
#include <vector>
#include "../models/Material.h"

namespace manager {

    class MaterialManager {
    public:
        // Singleton access
        static MaterialManager& getInstance() {
            static MaterialManager instance;
            return instance;
        }

        MaterialManager(const MaterialManager&) = delete;
        void operator=(const MaterialManager&) = delete;

        void addMaterial(const std::string &name, const models::Material &material);

        const models::Material &getMaterial(const std::string &name) const;

        const models::Material &getDefaultMaterial() const;

        [[nodiscard]] std::vector<std::string> getAllMaterialNames() const;

    private:
        MaterialManager() {
            initializeDefaults();
        }

        std::map<std::string, models::Material> materials;

        void initializeDefaults();
    };

} // manager

#endif //HEATEQUATIONVISUALIZATION_MATERIALMANAGER_H
