//
// Created by djcur on 4/10/2024.
//

#include <stdexcept>
#include "Grid.h"

namespace models {
    Grid::Grid(int width, int height) {
        if (width <= 0) {
            throw std::runtime_error("Width must be greater than 0.");
        } else {
            this->width = width;
        }
        
        if (height <= 0) {
            throw std::runtime_error("Height must be greater than 0.");
        } else {
            this->height = height;
        }
        initializeGrid();
    }
    
    void Grid::initializeGrid() {
        std::vector<models::Node> nodes;
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

    void Grid::setMaterial(int x, int y, models::Material material) {
        elements[x + width * y].setMaterial(material);
    }

    void Grid::setInitialTemperature(int x, int y, float temperature) {
        elements[x + width * y].setInitialHeat(temperature);
    }

    void Grid::setElementSource(int x, int y, float source) {
        elements[x + width * y].setExternalHeat(source);
    }
} // models