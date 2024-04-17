//
// Created by djcur on 4/10/2024.
//
#include <iostream>
#include "ui/UI.h"
#include "solver/Solver.h"

int main() {
    ui::UI &ui1 = ui::UI::getInstance();
    solver::Solver &solver1 = solver::Solver::getInstance();

    // Check if the UI was initialized properly
    if (!ui1.isInitialized()) {
        std::cerr << "UI initialization failed." << std::endl;
        return -1;
    }

    while (!ui1.shouldClose()) {
        ui1.updateSolverGrid();
        ui1.processInput();
        ui1.render(*solver1.getElements(), solver1.getGridWidth(), solver1.getGridHeight());
    }

    return 0;
}

