//
// Created by djcur on 4/10/2024.
//
#include <iostream>
#include "ui/UIManager.h"

int main() {
    Eigen::setNbThreads(std::thread::hardware_concurrency() - 2);
    try {
        // Initialize UI Manager with window dimensions and title
        const ui::UIManager uiManager(1500, 1000, "Heat Equation Visualization");

        while (!uiManager.shouldClose()) {
            uiManager.update();
            uiManager.render();
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
