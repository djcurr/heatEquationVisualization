//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_GRID_H
#define HEATEQUATIONVISUALIZATION_GRID_H

#include <vector>
#include "Node.h"
#include "Element.h"

namespace models {

    class Grid {
    public:
        Grid(int width, int height);

        const std::vector<Element> *getElements() const { return &elements; }

        const int getWidth() const { return width; }

        const int getHeight() const { return height; }

        void setMaterial(int x, int y, models::Material material);

        void setInitialTemperature(int x, int y, float temperature);

        void setElementSource(int x, int y, float source);

    private:
        std::vector<Element> elements;
        int width, height;

        void initializeGrid();
    };

} // models

#endif //HEATEQUATIONVISUALIZATION_GRID_H
