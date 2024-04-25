//
// Created by djcur on 4/24/2024.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>


namespace models {
    class Particle {
    public:
        double x, y; // Position
        double vx, vy; // Velocity
        double ax, ay; // Acceleration
        double density; // Density
        double pressure; // Pressure
        double mass; // Mass of the particle

        Particle(double x, double y, double mass)
            : x(x), y(y), vx(0), vy(0), ax(0), ay(0), density(0), pressure(0), mass(mass) {
        }

        void update(double dt) {
            // Update velocity and position based on current acceleration
            vx += ax * dt;
            vy += ay * dt;
            x += vx * dt;
            y += vy * dt;

            // Reset acceleration after each timestep
            ax = 0;
            ay = 0;
        }
    };
} // models

#endif //PARTICLE_H
