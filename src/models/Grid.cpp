//
// Created by djcur on 4/10/2024.
//

#include <stdexcept>
#include <cmath>
#include "Grid.h"

namespace models {
    Grid::Grid(int width, int height) {
        if (width <= 0) {
            throw std::runtime_error("Width must be greater than 0.");
        }
        this->width = width;

        
        if (height <= 0) {
            throw std::runtime_error("Height must be greater than 0.");
        }
        this->height = height;
        initializeGrid();
    }
    
    void Grid::initializeGrid() {
        std::vector<Node> nodes;
        nodes.resize((width + 1) * (height + 1));
        for (int i = 0; i <= height; ++i) {
            for (int j = 0; j <= width; ++j) {
                int nodeId = i * (width + 1) + j;
                nodes[nodeId].setGlobalId(nodeId);
            }
        }

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::array<Node, 4> elementNodes{
                        nodes[i * (width + 1) + j],       // Top-left
                        nodes[i * (width + 1) + j + 1],   // Top-right
                        nodes[(i + 1) * (width + 1) + j], // Bottom-left
                        nodes[(i + 1) * (width + 1) + j + 1]  // Bottom-right
                };
                bool isBoundary = (i == 0 || j == 0 || i == height - 1 || j == width - 1);
                elements.emplace_back(elementNodes, isBoundary);
            }
        }
    }

    void Grid::setMaterial(int x, int y, const Material &material) {
        elements[x + width * y].setMaterial(material);
    }

    void Grid::setInitialTemperature(int x, int y, float temperature) {
        elements[x + width * y].setInitialHeat(temperature);
    }

    void Grid::setElementSource(int x, int y, float source) {
        elements[x + width * y].setExternalHeat(source);
    }

    void Grid::resizeGrid(int newWidth, int newHeight) {
        if (newWidth <= 0 || newHeight <= 0) {
            throw std::runtime_error("New dimensions must be greater than 0.");
        }

        // Store old elements
        std::vector<Element> oldElements = std::move(elements);
        int oldWidth = width;
        int oldHeight = height;

        // Update dimensions
        width = newWidth;
        height = newHeight;

        // Reinitialize the grid with new dimensions
        initializeGrid();

        // Calculate scaling factors for each dimension
        float scaleX = static_cast<float>(oldWidth) / static_cast<float>(newWidth);
        float scaleY = static_cast<float>(oldHeight) / static_cast<float>(newHeight);

        // Map old element properties to the new elements
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                // Calculate the corresponding old element indices
                int oldX = static_cast<int>(std::round(static_cast<float>(j) * scaleX));
                int oldY = static_cast<int>(std::round(static_cast<float>(i) * scaleY));

                // Ensure the indices are within the bounds of the old grid
                oldX = std::min(oldX, oldWidth - 1);
                oldY = std::min(oldY, oldHeight - 1);

                // Transfer properties
                Element& newElement = elements[i * width + j];
                const Element& oldElement = oldElements[oldY * oldWidth + oldX];
                newElement.setMaterial(oldElement.getMaterial());
                newElement.setInitialHeat(oldElement.getInitialHeatKelvin());
                newElement.setExternalHeat(oldElement.getHeatSource());
            }
        }
    }
} // models