//
// Created by brunnen-g on 2/9/23.
//

#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

/**
 * Class-wrapper for RGB-color
 */
class Color {
private:
    uint8_t r;
    uint8_t g;
    uint8_t b;
public:
    Color(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    uint8_t getR() const {
        return r;
    }

    uint8_t getG() const {
        return g;
    }

    uint8_t getB() const {
        return b;
    }

    static const Color RED;
    static const Color YELLOW;
    static const Color BLUE;
    static const Color WHITE;
    static const Color GREEN;
    static const Color BLACK;

};

const Color RED(255, 0, 0);
const Color YELLOW(0xFF, 0xFF, 0);
const Color BLUE(0, 0, 0xFF);
const Color WHITE(0xFF, 0xFF, 0xFF);
const Color GREEN(0x00, 0xFF, 0x00);
const Color BLACK(0, 0, 0);

#endif //COLOR_H
