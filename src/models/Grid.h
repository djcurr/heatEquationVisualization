//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_GRID_H
#define HEATEQUATIONVISUALIZATION_GRID_H

#include <vector>
#include "Element.h"

namespace models {

    /**
     * @class Grid
     * @brief Represents a computational grid in the heat equation visualization.
     *
     * This class manages a grid of elements, each with its own nodes and material properties.
     * It supports operations like resizing the grid, setting materials, and initializing temperatures and heat sources.
     */
    class Grid {
    public:
        /**
         * @brief Constructor for the Grid class.
         * @param width The width of the grid, in terms of the number of elements.
         * @param height The height of the grid, in terms of the number of elements.
         *
         * Throws a runtime_error if either width or height is non-positive.
         */
        Grid(int width, int height);

        /**
         * @brief Get the elements of the grid.
         * @return A pointer to the vector of grid elements.
         *
         * This provides read-only access to the grid's elements.
         */
        [[nodiscard]] const std::vector<Element> *getElements() const { return &elements; }

        /**
         * @brief Get the width of the grid.
         * @return The number of elements across the width of the grid.
         */
        [[nodiscard]] int getWidth() const { return width; }

        /**
         * @brief Get the height of the grid.
         * @return The number of elements across the height of the grid.
         */
        [[nodiscard]] int getHeight() const { return height; }

        /**
         * @brief Set the material of an element in the grid.
         * @param x The x-coordinate of the element within the grid.
         * @param y The y-coordinate of the element within the grid.
         * @param material The material to assign to the element.
         */
        void setMaterial(int x, int y, const Material &material);

        /**
         * @brief Set the initial temperature of an element in the grid.
         * @param x The x-coordinate of the element.
         * @param y The y-coordinate of the element.
         * @param temperature The initial temperature to set for the element, in Kelvin.
         */
        void setInitialTemperature(int x, int y, float temperature);

        /**
         * @brief Set the heat source of an element in the grid.
         * @param x The x-coordinate of the element.
         * @param y The y-coordinate of the element.
         * @param source The heat source intensity to set for the element.
         */
        void setElementSource(int x, int y, float source);

        /**
         * @brief Resize the grid to specified dimensions.
         * @param newWidth The new width of the grid, in terms of the number of elements.
         * @param newHeight The new height of the grid, in terms of the number of elements.
         *
         * Resizes the grid and reinitializes its elements. Old elements are mapped to new ones based on proportional scaling.
         */
        void resizeGrid(int newWidth, int newHeight);

    private:
        std::vector<Element> elements; ///< Vector containing all the elements of the grid.
        int width; ///< Width of the grid, measured in elements.
        int height; ///< Height of the grid, measured in elements.

        /**
         * @brief Initialize the grid with default elements.
         *
         * Populates the grid with elements based on its current width and height. Each element is initialized with nodes and potentially set as a boundary based on its position.
         */
        void initializeGrid();
    };

}

#endif //HEATEQUATIONVISUALIZATION_GRID_H
