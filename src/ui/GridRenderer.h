//
// Created by djcur on 4/19/2024.
//

#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H
#include <vector>

#include "IRenderComponent.h"
#include "WindowManager.h"
#include "../models/Element.h"

namespace ui {

    class GridRenderer : public IRenderComponent {
    public:
        GridRenderer(GLFWwindow* window, const std::vector<models::Element>& elements, int gridWidth, int gridHeight)
            : window(window), elements(elements), gridWidth(gridWidth), gridHeight(gridHeight) {}

        void render() override;

        void renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color);

        void detectDrawing(int y, int x);

        void BresenhamLineAlgorithm(int currentX, int currentY);

        void applyElementParameters(int x, int y);

        void setGridStyles();

        void removeGridStyles();

    private:
        GLFWwindow* window;
        const std::vector<models::Element>& elements;
        int gridWidth, gridHeight;
    };

} // ui

#endif //GRIDRENDERER_H
