//
// Created by djcur on 4/24/2024.
//

#ifndef SMOOTHINGKERNEL_H
#define SMOOTHINGKERNEL_H

namespace solver {
    class SmoothingKernel {
    public:
        double h;

        explicit SmoothingKernel(double h) : h(h) {
        }

        [[nodiscard]] double W(double r) const;

        [[nodiscard]] double gradW(double r) const;
    };
} // solver

#endif //SMOOTHINGKERNEL_H
