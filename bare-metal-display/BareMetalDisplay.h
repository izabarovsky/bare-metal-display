//
// Created by brunnen-g on 2/10/23.
//

#ifndef RGB_DISPLAY_H
#define RGB_DISPLAY_H

#include <cstdint>
#include <vector>
#include "Color.h"
#include "Point.h"
#include "BaseFont.h"
#include "drivers/DisplayDriver.h"

/**
 * Base interface for display
 */
class BareMetalDisplay {
private:
    /**
     * Specific display driver
     */
    DisplayDriver *driver;
    /**
     * Display frame buffer
     * Can be transferred to real display with render() method only
     */
    uint16_t *buffer;
    /**
     * Just buffer size for check
     */
    uint32_t buffer_size;

    bool isPointBoundsValid(Point point);

    bool isXYBoundsValid(uint8_t x, uint8_t y);

    void setPixel(uint32_t x, uint32_t y, uint16_t color);

    void drawHLine(Point point1, Point point2, Color color);

    void drawVLine(Point point1, Point point2, Color color);

    void drawBresenhemsLine(Point point1, Point point2, Color color);

public:
    explicit BareMetalDisplay(DisplayDriver *driver);

    /**
     * Init display: must be called before any other method
     */
    void init();

    /**
     * Fill display buffer with color
     * @param color
     */
    void fillWithColor(Color color);

    /**
     * Render line to display buffer
     * @param point1
     * @param point2
     * @param color
     */
    void drawLine(Point point1, Point point2, Color color);

    /**
     * Render circle to display buffer
     * @param center
     * @param radius
     * @param color
     */
    void drawCircle(Point center, uint8_t radius, Color color);

    /**
     * Render image to display buffer
     * @param image - image, encoded in 16-bit 565 color values array
     * @param size - array size
     */
    void drawImage(const uint16_t *image, uint32_t size);

    /**
     * Render text to display buffer
     * @param start - start point
     * @param str - string
     * @param color - desired color
     * @param font - font
     */
    void drawText(Point start, const char *str, Color color, BaseFont *font);

    /**
     * Push display buffer to bare metal display
     */
    void render();

    virtual ~BareMetalDisplay();
};


#endif //RGB_DISPLAY_H
