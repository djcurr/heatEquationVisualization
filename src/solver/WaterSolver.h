//
// Created by djcur on 4/24/2024.
//

#ifndef WATERSOLVER_H
#define WATERSOLVER_H
#include <vector>

#include "SmoothingKernel.h"
#include "../models/Particle.h"

namespace solver {

class WaterSolver {
public:
    WaterSolver(double kernelWidth, double timeStep) : kernel(kernelWidth), dt(timeStep) {}

    void addParticle(double x, double y, double mass) {
        particles.emplace_back(x, y, mass);
    }

    void computeDensities();

    void computeForces();

    void integrate() {
        // Integrate particle states
        for (auto& p : particles) {
            p.update(dt);
        }
    }

    void stepSimulation() {
        computeDensities();
        computeForces();
        integrate();
    }
private:
    std::vector<models::Particle> particles;
    SmoothingKernel kernel;
    double dt;
};

} // solver

#endif //WATERSOLVER_H
