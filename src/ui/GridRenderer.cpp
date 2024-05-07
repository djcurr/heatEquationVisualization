//
// Created by djcur on 4/19/2024.
//

#include "GridRenderer.h"

#include "../config/Config.h"
#include "StyleManager.h"
#include "../events/Broker.h"

namespace ui {
    void GridRenderer::render() {
        processUpdates();
        setupWindow();
        applyStyles();
        renderGrid();
        resetMouseStateIfNeeded();
        finalizeRendering();
    }

    void GridRenderer::setupWindow() {
        ImVec2 gridSize = ImGui::GetIO().DisplaySize;
        ImVec2 windowSize(gridSize.x - config::Config::config.controlWidth, gridSize.y);
        ImGui::SetNextWindowSize(windowSize);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    }

    void GridRenderer::applyStyles() const {
        StyleManager::applyStyle(gridColors, gridVars);
        ImGui::Begin("Grid Visualization", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
    }

    void GridRenderer::renderGrid() {
        ImVec2 buttonSize = calculateButtonSize();
        ImVec2 cursorStart = ImGui::GetCursorScreenPos();

        if (!elements.empty()) {
            float currentY = cursorStart.y;
            for (int row = 0; row < gridHeight; ++row) {
                ImVec2 cellPos = cursorStart;
                cellPos.y = currentY;
                for (int col = 0; col < gridWidth; ++col) {
                    renderGridCellAtPosition(row, col, cellPos, buttonSize);
                    detectDrawing(row, col, cellPos, buttonSize);
                    maintainGridLine(col);
                    cellPos.x += buttonSize.x;
                }
                currentY += buttonSize.y; // Move to the next row
            }
        }
    }

    ImVec2 GridRenderer::calculateButtonSize() const {
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        return {availableSize.x / static_cast<float>(gridWidth), availableSize.y / static_cast<float>(gridHeight)};
    }

    void GridRenderer::renderGridCellAtPosition(int row, int col, const ImVec2 &cellPos,
                                                const ImVec2 &buttonSize) {
        ImVec4 color = calculateCellColor(elements[row * gridWidth + col]);
        renderGridCell(cellPos, buttonSize, color);
    }

    void GridRenderer::maintainGridLine(int col) const {
        if ((col + 1) % gridWidth != 0) {
            ImGui::SameLine();
        }
    }

    void GridRenderer::resetMouseStateIfNeeded() {
        if (!ImGui::IsMouseDown(0) && mouseDown) {
            mouseDown = false; // Reset on mouse up
        }
    }

    void GridRenderer::finalizeRendering() {
        batchApplyElementParameters();
        ImGui::End();
        StyleManager::removeStyle(static_cast<int>(gridColors.size()), static_cast<int>(gridVars.size()));
    }

    void GridRenderer::renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color) {
        ImGui::GetWindowDrawList()->AddRectFilled(cellPos, ImVec2(cellPos.x + cellSize.x, cellPos.y + cellSize.y),
                                                  ImGui::ColorConvertFloat4ToU32(color));
    }

    void GridRenderer::detectDrawing(int row, int col, const ImVec2 &cellPos, const ImVec2 &buttonSize) {
        ImVec2 mousePos = ImGui::GetMousePos();
        bool isWithinCell = (mousePos.x >= cellPos.x && mousePos.x <= (cellPos.x + buttonSize.x) &&
                             mousePos.y >= cellPos.y && mousePos.y <= (cellPos.y + buttonSize.y));

        if (isWithinCell) {
            if (ImGui::IsMouseDown(0)) {
                if (!mouseDown) {
                    mouseDown = true;
                    lastHoveredCell = {col, row};
                } else if (mouseDown) {
                    drawLineToLastHoveredCell(col, row);
                }
            } else if (!ImGui::IsMouseDown(0) && mouseDown) {
                mouseDown = false;
            }
        }
    }

    void GridRenderer::updateMouseState(bool isMouseDown, int x, int y) {
        if (isMouseDown) {
            mouseDown = true;
            lastHoveredCell = {x, y};
        } else {
            mouseDown = false; // Reset on mouse up
        }
    }

    void GridRenderer::drawLineToLastHoveredCell(int currentX, int currentY) {
        std::vector<models::Point> linePoints = BresenhamLineAlgorithm(currentX, currentY);
        batchedPoints.insert(batchedPoints.end(), linePoints.begin(), linePoints.end());
        lastHoveredCell = {currentX, currentY};
    }

    std::vector<models::Point> GridRenderer::BresenhamLineAlgorithm(int currentX, int currentY) const {
        int x1 = lastHoveredCell.first;
        int y1 = lastHoveredCell.second;
        const int dx = std::abs(currentX - x1);
        const int dy = std::abs(currentY - y1);
        const int sx = x1 < currentX ? 1 : -1;
        const int sy = y1 < currentY ? 1 : -1;
        int err = dx - dy;

        std::vector<models::Point> linePoints;
        while (true) {
            linePoints.push_back({x1, y1}); // Collect each point
            if (x1 == currentX && y1 == currentY) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }

        return linePoints;
    }

    void GridRenderer::batchApplyElementParameters() {
        if (batchedPoints.empty()) return;

        // Combine points affected by brush size before applying them
        std::vector<models::Point> affectedPoints = combineAffectedPoints(batchedPoints);

        // Publish the event based on the current view mode
        switch (currentViewMode) {
            case ViewMode::VIEW_MATERIAL:
                broker.publish<events::ApplyMaterial>(events::ApplyMaterial(std::move(affectedPoints)));
                break;
            case ViewMode::VIEW_INITIAL_TEMPERATURE:
                broker.publish<events::ApplyInitialTemperature>(
                    events::ApplyInitialTemperature(std::move(affectedPoints)));
                break;
            case ViewMode::VIEW_SOURCES:
                broker.publish<events::ApplySource>(events::ApplySource(std::move(affectedPoints)));
                break;
            default:
                break; // Optionally handle unsupported view modes
        }

        batchedPoints.clear(); // Clear the batch for the next set of updates
    }

    std::vector<models::Point> GridRenderer::combineAffectedPoints(const std::vector<models::Point> &points) const {
        std::unordered_set<models::Point> uniquePoints; // Use a hash to avoid duplicates
        for (const auto &[x, y]: points) {
            // Loop through a square that bounds the circle
            const int startX = std::max(0, x - brushSize);
            const int endX = std::min(gridWidth - 1, x + brushSize);
            const int startY = std::max(0, y - brushSize);
            const int endY = std::min(gridHeight - 1, y + brushSize);

            for (int ix = startX; ix <= endX; ++ix) {
                for (int iy = startY; iy <= endY; ++iy) {
                    // Check if the point (ix, iy) is within the circle centered at (x, y)
                    if ((ix - x) * (ix - x) + (iy - y) * (iy - y) <= brushSize * brushSize) {
                        uniquePoints.insert(models::Point{ix, iy});
                    }
                }
            }
        }

        return {uniquePoints.begin(), uniquePoints.end()};
    }

    ImVec4 GridRenderer::calculateCellColor(const models::Element &element) {
        double heat;
        double normalizedHeat;
        models::Color color;

        switch (currentViewMode) {
            case ViewMode::VIEW_INITIAL_TEMPERATURE:
                heat = element.getInitialHeatKelvin();
                normalizedHeat = normalize(heat, minTemp, maxTemp);
                return currentColorMap->getColor(normalizedHeat);

            case ViewMode::VIEW_SOURCES:
                heat = element.getHeatSource();
                normalizedHeat = normalize(heat, minSource, maxSource);
                return currentColorMap->getColor(normalizedHeat);

            case ViewMode::VIEW_MATERIAL:
                color = element.getMaterial().getColor();
                return {color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, 1.0};

            case ViewMode::VIEW_VISUALIZATION:
                heat = getCurrentElementTemperatureKelvin(element);
                normalizedHeat = normalize(heat, minSimTemp, maxSimTemp);
                return currentColorMap->getColor(normalizedHeat);

            default:
                return {1.0f, 0.0f, 0.0f, 1.0};
        }
    }

    double GridRenderer::normalize(double value, double min, double max) {
        if (max - min <= 0) {
            return 0;
        }
        return (value - min) / (max - min);
    }

    double GridRenderer::getCurrentElementTemperatureKelvin(const models::Element &element) {
        const std::array<models::Node, 4> nodes = element.getNodes();
        double totalTemperature = 0.0;
        int validNodes = 0;
        for (const auto &node: nodes) {
            int globalId = node.getGlobalId();
            {
                std::lock_guard lock(simulationTemperaturesMutex);
                if (globalId >= 0 && globalId < simulationTemperature.size()) {
                    totalTemperature += simulationTemperature.coeff(globalId);
                    ++validNodes;
                }
            }
        }

        if (validNodes == 0) {
            throw std::runtime_error("No valid nodes found for the given element.");
        }

        return totalTemperature / static_cast<double>(validNodes);
    }

    void GridRenderer::setCurrentColorMap(ColorMaps colorMap) {
        switch (colorMap) {
            case ColorMaps::PLASMA:
                currentColorMap = std::make_unique<PlasmaColorMap>();
                break;
            case ColorMaps::VIRIDIS:
                currentColorMap = std::make_unique<ViridisColorMap>();
                break;
            default:
                break;
        }
    }

    void GridRenderer::onEvent(const std::shared_ptr<events::Event> &event) {
        std::lock_guard<std::mutex> guard(updateMutex);
        updates.emplace([this, event]() {
            switch (event->getType()) {
                case EventType::BrushSizeChangeEvent:
                    brushSize = std::get<int>(event->getData(EventDataKey::BrushSize));
                    break;
                case EventType::ViewChange:
                    currentViewMode = std::get<ViewMode>(event->getData(EventDataKey::ViewMode));
                    break;
                case EventType::ElementsUpdate: {
                    elements = std::get<std::vector<models::Element> >(event->getData(EventDataKey::Elements));
                    int newGridWidth = std::get<int>(event->getData(EventDataKey::Width));
                    int newGridHeight = std::get<int>(event->getData(EventDataKey::Height));
                    if (newGridWidth != gridWidth || newGridHeight != gridHeight) {
                        gridWidth = newGridWidth;
                        gridHeight = newGridHeight;
                    }

                    if (!elements.empty()) {
                        double minInitialHeat = std::numeric_limits<double>::max();
                        double maxInitialHeat = std::numeric_limits<double>::lowest();
                        double minSourceHeat = std::numeric_limits<double>::max();
                        double maxSourceHeat = std::numeric_limits<double>::lowest();

                        for (const auto &element: elements) {
                            double initialHeat = element.getInitialHeatKelvin();
                            double sourceHeat = element.getHeatSource(); // Assuming there's a method getHeatSource()

                            if (initialHeat < minInitialHeat) minInitialHeat = initialHeat;
                            if (initialHeat > maxInitialHeat) maxInitialHeat = initialHeat;
                            if (sourceHeat < minSourceHeat) minSourceHeat = sourceHeat;
                            if (sourceHeat > maxSourceHeat) maxSourceHeat = sourceHeat;
                        }

                        minSource = minSourceHeat;
                        maxSource = maxSourceHeat;
                        minTemp = minInitialHeat;
                        maxTemp = maxInitialHeat;
                    }
                    break;
                }
                case EventType::SimulationTemperatureUpdate: {
                    std::lock_guard lock(simulationTemperaturesMutex);
                    simulationTemperature = std::get<Eigen::VectorXd>(
                        event->getData(EventDataKey::SimulationTemperatures));

                    if (simulationTemperature.size() > 0) {
                        minSimTemp = simulationTemperature.minCoeff();
                        maxSimTemp = simulationTemperature.maxCoeff();
                    }
                    break;
                }
                case EventType::ColorMapChange:
                    setCurrentColorMap(std::get<ColorMaps>(event->getData(EventDataKey::ColorMap)));
                    break;

                default:
                    break;
            }
        });
    }

    void GridRenderer::processUpdates() {
        std::lock_guard<std::mutex> guard(updateMutex);
        while (!updates.empty()) {
            updates.front()(); // Execute the update
            updates.pop();
        }
    }
} // ui
