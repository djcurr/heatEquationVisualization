//
// Created by djcur on 4/19/2024.
//

#ifndef CONTROLSRENDERER_H
#define CONTROLSRENDERER_H
#include "IRenderComponent.h"
#include "WindowManager.h"

namespace ui {

    class ControlsRenderer : public IRenderComponent {
    public:
        ControlsRenderer(GLFWwindow* window) : window(window) {}

        void renderControls();

        void render() override;

        void renderControlsButtons();

        void renderViewSelector();

        void renderTimeStepSlider();

        void renderGridSizeSelector();

        void renderTemperatureSelector();

        void renderSourceSelector();

        void renderMaterialSelector();

        void renderBrushSizeSlider();

        void renderTimeStepInput();

        void setControlsStyle();

        void removeControlsStyle();

    private:
        GLFWwindow* window;
        enum ViewMode {
            VIEW_INITIAL_TEMPERATURE, VIEW_SOURCES, VIEW_MATERIAL, VIEW_VISUALIZATION
        };
        ViewMode currentView = VIEW_INITIAL_TEMPERATURE;
    };

} // ui

#endif //CONTROLSRENDERER_H
