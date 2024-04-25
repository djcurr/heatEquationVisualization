//
// Created by djcur on 4/24/2024.
//

#include "SmoothingKernel.h"

namespace solver {
    double SmoothingKernel::W(double r) const {
        // Cubic spline smoothing kernel
        double q = r / h;
        if (q <= 1.0) {
            if (q <= 0.5) {
                return (2.0/3.0 - q*q + 0.5*q*q*q) / h;
            } else {
                return (2.0/3.0 - 4.0/3.0*q + q*q - 0.25*q*q*q) / h;
            }
        }
        return 0.0;
    }

    double SmoothingKernel::gradW(double r) const {
        // Gradient of the cubic spline smoothing kernel
        double q = r / h;
        if (q <= 1.0) {
            double factor = 1.0 / (h * h * h);
            if (q <= 0.5) {
                return (3.0 * q - 2.0 * q * q) * factor;
            } else {
                return (-q + 2.0 * q * q - 0.5 * q * q * q) * factor;
            }
        }
        return 0.0;
    }
} // solver