//
// Created by brunnen-g on 3/29/23.
//

#include "Basic8x8.h"
#include "font8x8_basic.h"

int32_t Basic8x8::drawChar(uint32_t x, uint32_t y, char16_t c, std::function<int8_t(uint32_t, uint32_t)> function) {
    const uint8_t FONT_WIDTH = 8;
    const uint8_t FONT_HEIGHT = 8;
    for (uint8_t h = 0; h < FONT_HEIGHT; h++) {
        uint8_t byte = font_8x8[c][h];
        for (uint8_t w = 0; w < FONT_WIDTH; w++) {
            if (byte & 0x01) {
                function(x + w, y + h);
            }
            byte = byte >> 1;
        }
    }
    return x + FONT_WIDTH;
}