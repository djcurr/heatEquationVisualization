//
// Created by djcur on 4/19/2024.
//

#include "GridRenderer.h"

namespace ui {
    void GridRenderer::render() {
        ImVec2 gridSizeIm = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowSize(ImVec2(gridSizeIm.x - config.controlWidth, gridSizeIm.y));
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

        setGridStyles();

        ImGui::Begin("Grid Visualization", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoTitleBar);

        ImVec2 buttonSize = ImGui::GetContentRegionAvail();
        buttonSize.y /= static_cast<float>(gridHeight);
        buttonSize.x /= static_cast<float>(gridWidth);

        ImVec2 cursorStart = ImGui::GetCursorScreenPos();
        for (int i = 0; i < gridHeight; ++i) {
            for (int j = 0; j < gridWidth; ++j) {
                ImVec2 cellPos = ImVec2(cursorStart.x + j * buttonSize.x, cursorStart.y + i * buttonSize.y);
                ImVec4 color = calculateCellColor(elements[i * gridWidth + j]);
                renderGridCell(cellPos, buttonSize, color);
                detectDrawing(j, i);
                if ((j + 1) % static_cast<int>(gridWidth) != 0) ImGui::SameLine();
            }
        }

        if (!ImGui::IsMouseDown(0) && mouseDown) {
            mouseDown = false; // Reset on mouse up
        }

        ImGui::End();
        removeGridStyles();
    }

    void GridRenderer::renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color) {
        ImGui::GetWindowDrawList()->AddRectFilled(cellPos, ImVec2(cellPos.x + cellSize.x, cellPos.y + cellSize.y),
                                                  ImGui::ColorConvertFloat4ToU32(color));
        ImGui::InvisibleButton("cell", cellSize); // Make each grid cell clickable
    }

    void GridRenderer::detectDrawing(int y, int x) {
        if (ImGui::IsItemHovered()) {
            if (ImGui::IsMouseDown(0)) {
                mouseDown = true;
                lastHoveredCell = {x, y};
            }
            if (mouseDown) {
                BresenhamLineAlgorithm(x, y);
            }
        }
    }

    void GridRenderer::BresenhamLineAlgorithm(int currentX, int currentY) {
        int x1 = lastHoveredCell.first;
        int y1 = lastHoveredCell.second;
        int const dx = std::abs(currentX - x1);
        int const dy = -std::abs(currentY - y1);
        int sx = x1 < currentX ? 1 : -1;
        int sy = y1 < currentY ? 1 : -1;
        int err = dx + dy, e2;

        while (true) {
            applyElementParameters(x1, y1);  // Apply brush effect here
            if (x1 == currentX && y1 == currentY) break;
            e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x1 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y1 += sy;
            }
        }
        lastHoveredCell = {currentX, currentY};
    }

    void GridRenderer::applyElementParameters(int x, int y) {
        int startX = std::max(0, x - brushSize);
        int endX = std::min(gridWidth - 1, x + brushSize);
        int startY = std::max(0, y - brushSize);
        int endY = std::min(gridHeight - 1, y + brushSize);

        for (int i = startY; i <= endY; ++i) {
            for (int j = startX; j <= endX; ++j) {
                if (currentViewMode == VIEW_MATERIAL) {
                    updateElementMaterial(i, j);
                }
                if (currentViewMode == VIEW_INITIAL_TEMPERATURE) {
                    updateElementInitialTemperatureCelsius(i, j);
                }
                if (currentViewMode == VIEW_SOURCES) {
                    updateElementSource(i, j);
                }
            }
        }
    }

    void GridRenderer::setGridStyles() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    }

    void GridRenderer::removeGridStyles() {
        ImGui::PopStyleVar(2);
    }
} // ui