//
// Created by djcur on 4/19/2024.
//

#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H
#include <memory>
#include <variant>
#include <vector>

#include "IRenderComponent.h"
#include "WindowManager.h"
#include "../config/Config.h"
#include "../events/Broker.h"
#include "../events/Event.h"
#include "../events/ISubscriber.h"
#include "../models/Element.h"
#include "../events/EventTypes.h"

namespace ui {
    class GridRenderer final : public IRenderComponent, public events::ISubscriber {
    public:
        explicit GridRenderer(GLFWwindow *window) : window(window),
            mouseDown(false) {
            setCurrentColorMap(config::Config::config.colorMap);
            broker.publish<events::GridHeightChangeEvent>(events::GridHeightChangeEvent(config::Config::config.gridHeight));
        }

        void render() override;

    private:
        GLFWwindow *window;
        std::pair<int, int> lastHoveredCell;
        bool mouseDown;

        std::queue<std::function<void()>> updates;
        std::vector<models::Point> batchedPoints;

        std::vector<models::Element> elements;
        int gridWidth = config::Config::config.gridWidth;
        int gridHeight = config::Config::config.gridHeight;
        ViewMode currentViewMode = config::Config::config.view_mode;
        int brushSize = config::Config::config.brushSize;
        Eigen::VectorXd simulationTemperature = config::Config::config.simulationTemperature;
        std::unique_ptr<ColorMap> currentColorMap;
        float minSource = 0;
        float maxSource = 0;
        float minTemp = 0;
        float maxTemp = 0;
        float minSimTemp = 0;
        float maxSimTemp = 0;

        events::Broker &broker = events::Broker::getInstance();
        std::mutex updateMutex;

        void onEvent(const std::shared_ptr<events::Event> &event) override;

        void processUpdates();

        static void setupWindow();

        void applyStyles() const;

        void renderGrid();

        [[nodiscard]] ImVec2 calculateButtonSize() const;

        void renderGridCellAtPosition(int row, int col, const ImVec2 &cursorStart, const ImVec2 &buttonSize) const;

        void maintainGridLine(int col) const;

        void resetMouseStateIfNeeded();

        void finalizeRendering();

        static void renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color);

        void detectDrawing(int y, int x);

        void batchApplyElementParameters();

        std::vector<models::Point> combineAffectedPoints(const std::vector<models::Point> &points) const;

        void updateMouseState(bool isMouseDown, int x, int y);

        void drawLineToLastHoveredCell(int currentX, int currentY);

        std::vector<models::Point> BresenhamLineAlgorithm(int currentX, int currentY) const;

        void applyElementParameters(int x, int y);

        ImVec4 calculateCellColor(const models::Element &element) const;

        float normalize(float value, float min, float max) const;

        [[nodiscard]] double getCurrentElementTemperatureKelvin(const models::Element &element) const;

        void setCurrentColorMap(ColorMaps colorMap);

        std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2> > > gridVars{
            {ImGuiStyleVar_WindowPadding, config::Config::config.windowPadding},
            {ImGuiStyleVar_ItemSpacing, config::Config::config.itemSpacing}
        };

        std::vector<std::pair<ImGuiCol, ImVec4> > gridColors{};
    };
} // ui

#endif //GRIDRENDERER_H
