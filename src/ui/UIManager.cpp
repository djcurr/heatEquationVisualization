//
// Created by djcur on 4/18/2024.
//

#include "UIManager.h"

namespace ui {
    UIManager::UIManager(int width, int height, const std::string &title) {
        windowManager = std::make_unique<WindowManager>(width, height, title);
        renderer = std::make_unique<Renderer>(windowManager->getWindow());
    }

    void UIManager::update() const {
        if (windowManager->isInitialized()) {
            windowManager->processInput();
        }
    }

    void UIManager::render() const {
        if (windowManager->isInitialized() && !windowManager->shouldClose()) {
            renderer->render();
        }
    }
} // ui
