//
// Created by djcur on 4/24/2024.
//

#ifndef PARTICLE_H
#define PARTICLE_H

namespace models {
    /**
     * @class Particle
     * @brief Represents a physical particle in simulations, encompassing properties such as position, velocity, and mass.
     *
     * This class is designed to simulate the dynamics of a particle within a physical system, accounting for its
     * kinematic properties (position and velocity) and dynamic properties (mass, density, and pressure).
     */
    class Particle {
    public:
        double x, y; ///< X and Y coordinates of the particle's position.
        double vx, vy; ///< X and Y components of the particle's velocity.
        double ax, ay; ///< X and Y components of the particle's acceleration.
        double density; ///< Density of the particle, affecting physical interactions like pressure.
        double pressure; ///< Pressure experienced by the particle, relevant in fluid dynamics.
        double mass; ///< Mass of the particle, influencing its inertial properties.

        /**
         * @brief Constructs a Particle with specified initial position and mass.
         * @param x The x-coordinate of the initial position.
         * @param y The y-coordinate of the initial position.
         * @param mass The mass of the particle.
         *
         * Initializes the particle with specified position and mass while setting all other dynamical properties (velocity, acceleration)
         * to zero. This constructor allows for the creation of a particle at a specific location with a defined mass.
         */
        Particle(const double x, const double y, const double mass)
            : x(x), y(y), vx(0), vy(0), ax(0), ay(0), density(0), pressure(0), mass(mass) {
        }

        /**
         * @brief Updates the particle's state based on its acceleration and the time step.
         * @param dt The time step size used for the update.
         *
         * This method advances the particle's position and velocity according to Newton's second law of motion.
         * Acceleration affects the velocity, which in turn affects the position. After updating the dynamics, the
         * acceleration is reset, assuming a constant force model where forces are recalculated at each time step.
         */
        void update(const double dt) {
            // Update velocity based on acceleration
            vx += ax * dt;
            vy += ay * dt;

            // Update position based on new velocity
            x += vx * dt;
            y += vy * dt;

            // Reset acceleration to zero after each update to prepare for the next force calculation
            ax = 0;
            ay = 0;
        }
    };
} // namespace models

#endif //PARTICLE_H
