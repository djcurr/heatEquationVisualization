//
// Created by djcur on 4/10/2024.
//

#ifndef HEATEQUATIONVISUALIZATION_NODE_H
#define HEATEQUATIONVISUALIZATION_NODE_H

namespace models {

    /**
     * @class Node
     * @brief Represents a node within a grid for numerical simulations.
     *
     * This class defines a single node point in a computational grid used in numerical simulations,
     * such as finite element analysis in heat equation visualization. Each node holds a unique global identifier
     * which is essential for mapping and solving purposes.
     */
    class Node {
    public:
        /**
         * @brief Constructs a Node with a default global ID of -1.
         *
         * A default global ID of -1 indicates that the node has not yet been assigned a valid identifier
         * within the grid structure.
         */
        Node() : globalId(-1) {};

        /**
         * @brief Sets the global ID of the node.
         * @param globalId The global ID to assign to the node.
         *
         * The global ID is typically assigned based on the node's position within the grid and is used to
         * uniquely identify and reference the node in computational algorithms.
         */
        void setGlobalId(const int globalId) { this->globalId = globalId; }

        /**
         * @brief Gets the global ID of the node.
         * @return The global ID of the node.
         *
         * If the global ID is -1, it indicates that the node has not been assigned a valid ID.
         */
        [[nodiscard]] int getGlobalId() const { return globalId; }

    private:
        int globalId; ///< The global identifier for the node, used in grid mapping and analysis.
    };

}

#endif //HEATEQUATIONVISUALIZATION_NODE_H
