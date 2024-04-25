//
// Created by djcur on 4/24/2024.
//

#include "WaterSolver.h"

namespace solver {
    void WaterSolver::computeDensities() {
        for (auto& pi : particles) {
            double density = 0;
            for (const auto& pj : particles) {
                double r = sqrt((pi.x - pj.x) * (pi.x - pj.x) + (pi.y - pj.y) * (pi.y - pj.y));
                density += pj.mass * kernel.W(r);
            }
            pi.density = density;
        }
    }

    void WaterSolver::computeForces() {
        for (auto& pi : particles) {
            double pressure_i = pi.density - 1000;  // Example: Simple EOS with reference density 1000 kg/m^3
            pi.ax = 0;
            pi.ay = 0;

            for (const auto& pj : particles) {
                double r = sqrt((pi.x - pj.x) * (pi.x - pj.x) + (pi.y - pj.y) * (pi.y - pj.y));
                if (r == 0) continue; // Skip self-interaction

                double pressure_j = pj.density - 1000;
                double m_over_rhoj = pj.mass / pj.density;

                // Compute pressure force contribution
                double W_grad = kernel.gradW(r);
                double pressure_term = m_over_rhoj * (pressure_i + pressure_j) / 2.0;
                pi.ax -= pressure_term * W_grad * ((pi.x - pj.x) / r);
                pi.ay -= pressure_term * W_grad * ((pi.y - pj.y) / r);

                // Compute viscous force contribution
                double visc = 0.1;  // Example viscosity coefficient
                double v_grad = kernel.gradW(r) * visc * m_over_rhoj;
                pi.ax += v_grad * (pj.vx - pi.vx);
                pi.ay += v_grad * (pj.vy - pi.vy);
            }

            // Apply gravity (example: gravitational acceleration in y-direction)
            pi.ay -= 9.81;
        }
    }
} // solver