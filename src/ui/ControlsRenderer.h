//
// Created by djcur on 4/19/2024.
//

#ifndef CONTROLSRENDERER_H
#define CONTROLSRENDERER_H
#include <vector>

#include "ColorMap.h"
#include "../config/Config.h"
#include "IRenderComponent.h"
#include "WindowManager.h"
#include "../events/Broker.h"
#include "../events/ISubscriber.h"

namespace ui {
    class ControlsRenderer final : public IRenderComponent, public events::ISubscriber {
    public:
        explicit ControlsRenderer(GLFWwindow *window) : window(window) {
        }


        void render() override;

    private:
        GLFWwindow *window;
        int currentViewTimeStep = 0;
        float selectedTemperatureCelsius = 0;
        int gridWidth = config::Config::config.gridWidth;
        int gridHeight = config::Config::config.gridHeight;
        float selectedSource = 0;
        int selectedMaterialIndex = 0;
        int brushSize = config::Config::config.brushSize;
        int timeStepSizeSeconds = config::Config::config.timeStepSizeSeconds;
        int targetDurationSeconds = config::Config::config.targetDurationSeconds;
        bool simulationComplete = false;
        bool simulationRunning = false;
        events::Broker &broker = events::Broker::getInstance();
        int timeStepsCompleted = 0;
        ColorMaps currentColorMap = config::Config::config.colorMap;

        ViewMode currentView = config::Config::config.view_mode;

        std::vector<std::pair<ImGuiCol, ImVec4> > controlColors{
            {ImGuiCol_WindowBg, config::Config::config.windowBackgroundColor},
            {ImGuiCol_Button, config::Config::config.buttonColor},
            {ImGuiCol_ButtonHovered, config::Config::config.buttonHoverColor},
            {ImGuiCol_ButtonActive, config::Config::config.buttonActiveColor}
        };
        std::vector<std::pair<ImGuiStyleVar, std::variant<float, ImVec2> > > controlVars{
            {ImGuiStyleVar_FramePadding, config::Config::config.buttonPadding},
            {ImGuiStyleVar_FrameRounding, config::Config::config.buttonRounding}
        };
        int maxTimeSteps = 1;

        static void setWindow();

        void renderControls();

        void renderControlsButtons();

        void changeView(ViewMode view);

        void renderViewSelector();

        void renderTimeStepSlider();

        void renderGridSizeSelector();

        void renderColorSettings();

        void renderTemperatureSelector();

        void renderSourceSelector();

        void renderMaterialSelector();

        void renderProgressBar() const;

        void renderBrushSizeSlider();

        void renderTimeStepInput();

        bool handleRadioButton(const std::string &label, ViewMode targetView, bool condition = true);

        template<class T, class Event, class... Args>
        void publishIfChanged(T &oldValue, T newValue, Args &&... args);

        template<typename Event>
        void handleButton(const std::string &label, const ImVec2 &size, ViewMode checkView,
                                            ViewMode changeToView, Event event);

        template<typename T, typename Event, typename... Args>
        void renderStandardInput(const std::string &label, T &value, Args &&... args);

        template<typename T, typename Event, typename... Args>
        void renderSliderInput(const std::string &label, T &value, T min, T max, Args &&... args);

        void onEvent(const std::shared_ptr<events::Event> &event) override;
    };
} // ui

#endif //CONTROLSRENDERER_H
