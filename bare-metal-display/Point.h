//
// Created by brunnen-g on 2/9/23.
//

#ifndef POINT_H
#define POINT_H

#include <cstdint>

/**
 * Class-wrapper for point
 */
class Point {
private:
    uint8_t x;
    uint8_t y;

public:
    Point(uint8_t x, uint8_t y) {
        this->x = x;
        this->y = y;
    }

    uint8_t getX() const {
        return x;
    }

    uint8_t getY() const {
        return y;
    }
};


#endif //POINT_H
