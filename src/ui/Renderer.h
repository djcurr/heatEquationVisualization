//
// Created by djcur on 4/18/2024.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <vector>

#include "WindowManager.h"

namespace models {
    class Element;
}

namespace ui {

    class Renderer {
    public:
        explicit Renderer(GLFWwindow* window);
        ~Renderer();

        static void initializeImGui();
        void finalizeRender();

        void render(const std::vector<models::Element> &elements);

    private:

        GLFWwindow* window;


    };

} // ui

#endif //RENDERER_H
