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

    /**
     * @class MaterialManager
     * @brief Manages a collection of materials used in the heat equation visualization.
     *
     * This class is implemented as a singleton to ensure that there is a single centralized
     * instance managing all material data across the application. It provides methods to add,
     * retrieve, and list materials.
     */
    class MaterialManager {
    public:
        /**
         * @brief Singleton access to MaterialManager instance.
         * @return Reference to the singleton instance of MaterialManager.
         *
         * Ensures that there is only one instance of the MaterialManager throughout the application,
         * providing a global point of access.
         */
        static MaterialManager& getInstance() {
            static MaterialManager instance;
            return instance;
        }

        /**
         * @brief Adds a material to the manager.
         * @param name The name of the material.
         * @param material The material object to be added.
         *
         * Stores the material in the manager under the provided name. If the name already exists,
         * the material associated with that name is overwritten.
         */
        void addMaterial(const std::string &name, const models::Material &material);

        /**
         * @brief Retrieves a material by its name.
         * @param name The name of the material to retrieve.
         * @return Constant reference to the material object.
         * @throw std::out_of_range If the material name does not exist.
         *
         * Provides read-only access to the material associated with the specified name.
         */
        [[nodiscard]] const models::Material &getMaterial(const std::string &name) const;

        /**
         * @brief Retrieves the default material.
         * @return Constant reference to the default material object.
         *
         * This method is useful for obtaining a reference material when no specific material
         * has been requested.
         */
        [[nodiscard]] const models::Material &getDefaultMaterial() const;

        /**
         * @brief Gets all material names stored in the manager.
         * @return Vector of strings containing all the material names.
         *
         * This method allows for retrieving a list of all material names, which can be useful for
         * UI displays or logging purposes.
         */
        [[nodiscard]] std::vector<std::string> getAllMaterialNames() const;

    private:
        /**
         * @brief Private constructor to enforce the singleton pattern.
         *
         * Initializes the manager and loads default materials.
         */
        MaterialManager() {
            initializeDefaults();
        }

        std::map<std::string, models::Material> materials; ///< Map of material names to Material objects.

        /**
         * @brief Initializes the manager with default materials.
         *
         * This method is called by the constructor to populate the manager with an initial set of materials,
         * ensuring that there are always some materials available for use.
         */
        void initializeDefaults();
    };
}

#endif //HEATEQUATIONVISUALIZATION_MATERIALMANAGER_H
