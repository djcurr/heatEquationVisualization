//
// Created by djcur on 4/18/2024.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <vector>

#include "IRenderComponent.h"
#include "SimulationController.h"
#include "WindowManager.h"

namespace ui {
    class Renderer {
    public:
        explicit Renderer(GLFWwindow *window);

        ~Renderer();

        void render();

    private:
        static void initializeImGui();

        void finalizeRender();

        GLFWwindow *window;
        events::Broker &broker = events::Broker::getInstance();
        std::vector<std::shared_ptr<IRenderComponent> > renderComponents;
        std::shared_ptr<SimulationController> simulationController;
    };
} // ui

#endif //RENDERER_H
