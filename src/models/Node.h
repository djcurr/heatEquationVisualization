//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_NODE_H
#define HEATEQUATIONVISUALIZATION_NODE_H

namespace models {

    class Node {
    public:
        Node() : globalId(-1) {};

        void setGlobalId(int globalid) { this->globalId = globalid; }

        const int getGlobalId() const { return globalId; }

    private:
        int globalId;
    };

} // models

#endif //HEATEQUATIONVISUALIZATION_NODE_H
