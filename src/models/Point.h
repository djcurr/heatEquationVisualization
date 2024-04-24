//
// Created by djcur on 4/22/2024.
//

#ifndef POINT_H
#define POINT_H

namespace models {
    class Point {
    public:
        int x;
        int y;

        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
}

template<>
struct std::hash<models::Point> {
    size_t operator()(const models::Point &p) const noexcept {
        // Combine hashes of x and y with a shift to reduce collision
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

#endif //POINT_H
