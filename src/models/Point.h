//
// Created by djcur on 4/22/2024.
//

#ifndef POINT_H
#define POINT_H

#include <functional> // Needed for std::hash

namespace models {
    /**
     * @class Point
     * @brief Represents a two-dimensional point with integer coordinates.
     *
     * This class provides a simple structure for representing points in a two-dimensional space,
     * which includes utilities for equality comparison to facilitate its use in data structures that
     * require element comparison.
     */
    class Point {
    public:
        int x; ///< The x-coordinate of the point.
        int y; ///< The y-coordinate of the point.

        /**
         * @brief Checks if two points are equal.
         * @param other Another point to compare with this point.
         * @return True if both x and y coordinates are the same, false otherwise.
         *
         * This operator enables Point objects to be directly compared, which is essential for use
         * in associative containers and for general equality checks.
         */
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
}

namespace std {
    /**
     * @brief Specialization of std::hash for the Point class to enable its use as keys in unordered data structures.
     */
    template<>
    struct hash<models::Point> {
        /**
         * @brief Generates a hash value for a Point object.
         * @param p The Point object to generate a hash for.
         * @return A hash value computed for the Point object.
         *
         * This function combines the hash of the x and y coordinates by shifting and bitwise operations,
         * aiming to minimize collisions in hash tables.
         */
        size_t operator()(const models::Point& p) const noexcept {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };
}

#endif //POINT_H
