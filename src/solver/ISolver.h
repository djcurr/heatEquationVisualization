//
// Created by djcur on 4/24/2024.
//

#ifndef ISOLVER_H
#define ISOLVER_H
#include <vector>
#include <Eigen>
#include "../models/Element.h"
namespace solver {
    class ISolver {
    public:
        virtual ~ISolver() = default;

        virtual std::vector<Eigen::VectorXd> performSimulation(int numTimesteps, int timeStep) = 0;
        virtual void updateGridSize(int newWidth, int newHeight) = 0;
        [[nodiscard]] virtual std::vector<models::Element> getElements() const = 0;
    };
}
#endif //ISOLVER_H
