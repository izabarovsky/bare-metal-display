
#ifndef BARE_METAL_DISPLAY_COLORUTIL_H
#define BARE_METAL_DISPLAY_COLORUTIL_H

#include <cstdint>

/**
 * Static utils for color convert
 */
class ColorUtil {
public:

    /**
     *
     * @param r - read
     * @param g - green
     * @param b - blue
     * @return - compressed 16-bit value, often used with TFT displays
     */
    static uint16_t rgbToColor565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

    /**
     *
     * @param color - wrapper for rgb values Color.h
     * @return - compressed 16-bit value, often used with TFT displays
     */
    static uint16_t rgbToColor565(Color color) {
        return ((color.getR() & 0xF8) << 8) | ((color.getG() & 0xFC) << 3) | (color.getB() >> 3);
    }
};

#endif //BARE_METAL_DISPLAY_COLORUTIL_H
