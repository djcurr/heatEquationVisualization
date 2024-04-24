//
// Created by djcur on 4/18/2024.
//

#ifndef UIMANAGER_H
#define UIMANAGER_H
#include "Renderer.h"

namespace ui {
    class UIManager {
    public:
        UIManager(int width, int height, const std::string &title);

        void update() const;

        void render() const;

        [[nodiscard]] bool shouldClose() const { return windowManager->shouldClose(); }

    private:
        std::unique_ptr<WindowManager> windowManager;
        std::unique_ptr<Renderer> renderer;
    };
} // ui

#endif //UIMANAGER_H
