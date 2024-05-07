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
    /**
     * @class GridRenderer
     * @brief Renders the simulation grid and handles interactive elements within the grid.
     *
     * This class is responsible for drawing the grid used in simulations and for processing user
     * interactions such as drawing and modifying elements directly within the grid. It subscribes to
     * events to handle updates to grid elements dynamically during simulation.
     */
    class GridRenderer final : public IRenderComponent, public events::ISubscriber {
    public:
        /**
         * @brief Constructs a GridRenderer with a specified window.
         * @param window Pointer to the GLFW window where the grid will be rendered.
         */
        explicit GridRenderer(GLFWwindow *window) : window(window),
                                                    mouseDown(false) {
            setCurrentColorMap(config::Config::config.colorMap);
            broker.publish<events::GridHeightChangeEvent>(
                events::GridHeightChangeEvent(config::Config::config.gridHeight));
        }

        /**
         * @brief Renders the grid and handles user interactions.
         */
        void render() override;

    private:
        GLFWwindow *window; ///< The window where the grid is rendered.
        std::pair<int, int> lastHoveredCell; ///< Last cell hovered over by the mouse.
        bool mouseDown; ///< Flag indicating if the mouse button is pressed down.

        std::queue<std::function<void()> > updates; ///< Queue of updates to apply to the grid.
        std::vector<models::Point> batchedPoints; ///< Batch of points affected by the current drawing action.

        std::vector<models::Element> elements; ///< Elements that make up the grid.
        int gridWidth = config::Config::config.gridWidth; ///< Width of the grid.
        int gridHeight = config::Config::config.gridHeight; ///< Height of the grid.
        ViewMode currentViewMode = config::Config::config.view_mode; ///< Current view mode of the renderer.
        int brushSize = config::Config::config.brushSize; ///< Size of the brush used for drawing.
        Eigen::VectorXd simulationTemperature = Eigen::VectorXd::Zero(0); ///< Simulation temperatures.
        std::mutex simulationTemperaturesMutex;
        std::unique_ptr<ColorMap> currentColorMap; ///< Currently active color map.
        double minSource = 0; ///< Minimum source value for scaling.
        double maxSource = 0; ///< Maximum source value for scaling.
        double minTemp = 0; ///< Minimum temperature for scaling.
        double maxTemp = 0; ///< Maximum temperature for scaling.
        double minSimTemp = 0; ///< Minimum simulated temperature for scaling.
        double maxSimTemp = 0; ///< Maximum simulated temperature for scaling.

        events::Broker &broker = events::Broker::getInstance(); ///< Event broker for managing events.
        std::mutex updateMutex; ///< Mutex for thread-safe updates.

        /**
                * @brief Responds to events published by the event system.
                * @param event Shared pointer to the received event.
                */
        void onEvent(const std::shared_ptr<events::Event> &event) override;

        /**
         * @brief Processes pending updates in the update queue.
         */
        void processUpdates();

        /**
         * @brief Sets up window properties for rendering.
         */
        static void setupWindow();

        /**
         * @brief Applies styling to the ImGui elements being rendered.
         */
        void applyStyles() const;

        /**
         * @brief Renders the grid cells.
         */
        void renderGrid();

        /**
         * @brief Calculates the size of buttons used for grid cells based on window size and grid dimensions.
         * @return Size of the buttons.
         */
        [[nodiscard]] ImVec2 calculateButtonSize() const;

        /**
         * @brief Renders a single grid cell at the specified row and column.
         * @param row Row index of the cell.
         * @param col Column index of the cell.
         * @param cursorStart Starting position of the cursor for drawing.
         * @param buttonSize Size of each button representing grid cells.
         */
        void renderGridCellAtPosition(int row, int col, const ImVec2 &cursorStart, const ImVec2 &buttonSize);

        /**
         * @brief Maintains a visual line while interacting with the grid.
         * @param col Column index.
         */
        void maintainGridLine(int col) const;

        /**
         * @brief Resets the mouse state when necessary.
         */
        void resetMouseStateIfNeeded();

        /**
         * @brief Finalizes rendering operations.
         */
        void finalizeRendering();

        /**
         * @brief Renders an individual grid cell.
         * @param cellPos Position of the cell.
         * @param cellSize Size of the cell.
         * @param color Color of the cell.
         */
        static void renderGridCell(const ImVec2 &cellPos, const ImVec2 &cellSize, const ImVec4 &color);

        /**
         * @brief Detects drawing actions based on mouse movement.
         * @param y Y-coordinate.
         * @param x X-coordinate.
         */
        void detectDrawing(int row, int col, const ImVec2 &cellPos, const ImVec2 &buttonSize);

        /**
         * @brief Applies parameters to elements based on current brush and settings.
         */
        void batchApplyElementParameters();

        /**
         * @brief Combines affected points for batch processing.
         * @param points Points to combine.
         * @return Vector of combined points.
         */
        [[nodiscard]] std::vector<models::Point> combineAffectedPoints(const std::vector<models::Point> &points) const;

        /**
         * @brief Updates the state of the mouse based on interaction.
         * @param isMouseDown Whether the mouse button is down.
         * @param x X-coordinate.
         * @param y Y-coordinate.
         */
        void updateMouseState(bool isMouseDown, int x, int y);

        /**
         * @brief Draws a line to the last hovered cell.
         * @param currentX Current X-coordinate.
         * @param currentY Current Y-coordinate.
         */
        void drawLineToLastHoveredCell(int currentX, int currentY);

        /**
         * @brief Implements Bresenham's line algorithm for drawing lines.
         * @param currentX Current X-coordinate.
         * @param currentY Current Y-coordinate.
         * @return Vector of points representing the line.
         */
        [[nodiscard]] std::vector<models::Point> BresenhamLineAlgorithm(int currentX, int currentY) const;

        /**
         * @brief Calculates the color of a cell based on the current color map and element properties.
         * @param element Element for which to calculate the color.
         * @return Calculated color.
         */
        [[nodiscard]] ImVec4 calculateCellColor(const models::Element &element);

        /**
         * @brief Normalizes a value within a specified range.
         * @param value Value to normalize.
         * @param min Minimum of the range.
         * @param max Maximum of the range.
         * @return Normalized value.
         */
        [[nodiscard]] static double normalize(double value, double min, double max);

        /**
         * @brief Retrieves the current temperature of an element in Kelvin.
         * @param element Element to check.
         * @return Temperature in Kelvin.
         */
        [[nodiscard]] double getCurrentElementTemperatureKelvin(const models::Element &element);

        /**
         * @brief Sets the current color map based on the specified color map type.
         * @param colorMap Color map to set.
         */
        void setCurrentColorMap(ColorMaps colorMap);

        /**
          * @brief Style variables and values used for rendering the grid.
          */
        std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2> > > gridVars{
            {ImGuiStyleVar_WindowPadding, config::Config::config.windowPadding},
            {ImGuiStyleVar_ItemSpacing, config::Config::config.itemSpacing}
        };

        /**
         * @brief Color settings for different elements of the grid.
         */
        std::vector<std::pair<ImGuiCol, ImVec4> > gridColors{};
    };
} // ui

#endif //GRIDRENDERER_H
