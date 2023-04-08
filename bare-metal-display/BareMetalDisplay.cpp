//
// Created by brunnen-g on 2/10/23.
//

#include <cstdlib>
#include "BaseFont.h"
#include "BareMetalDisplay.h"
#include "ColorUtil.h"

BareMetalDisplay::BareMetalDisplay(DisplayDriver *driver) : driver(driver) {
    buffer_size = driver->getXsize() * driver->getYsize();
    buffer = new uint16_t[240 * 240];
}

BareMetalDisplay::~BareMetalDisplay() {
    delete[] buffer;
}

/**
 * Init display throw corresponded driver method
 */
void BareMetalDisplay::init() {
    driver->init();
}

/**
 * Simple fill display buffer with desired color
 * @param color
 */
void BareMetalDisplay::fillWithColor(Color color) {
    uint16_t color565 = ColorUtil::rgbToColor565(color);
    for (uint32_t i = 0; i < buffer_size - 1; i++) buffer[i] = color565;
}

/**
 *
 * @param point1
 * @param point2
 * @param color
 * @brief Check points valid or not
 * Then draw line with implemented internal methods: vertical, horizontal or another
 */
void BareMetalDisplay::drawLine(Point point1, Point point2, Color color) {
    if (!(isPointBoundsValid(point1) && isPointBoundsValid(point2))) return;
    if (point1.getY() == point2.getY()) drawHLine(point1, point2, color);
    else if (point1.getX() == point2.getX()) drawVLine(point1, point2, color);
    else drawBresenhemsLine(point1, point2, color);
}

/**
 * Draw circle with  Bresenhem's algorithm
 * @param center
 * @param radius
 * @param color
 */
void BareMetalDisplay::drawCircle(Point center, uint8_t radius, Color color) {
    uint8_t x0 = center.getX();
    uint8_t y0 = center.getY();
    uint16_t color565 = ColorUtil::rgbToColor565(color);
    int x = 0;
    auto y = (int8_t) radius;
    int16_t delta = 1 - 2 * radius;
    int8_t error = 0;
    while (y >= 0) {
        if (!isXYBoundsValid(x0 + x, y0 + y)) break;
        setPixel(x0 + x, y0 + y, color565);
        if (!isXYBoundsValid(x0 + x, y0 - y)) break;
        setPixel(x0 + x, y0 - y, color565);
        if (!isXYBoundsValid(x0 - x, y0 + y)) break;
        setPixel(x0 - x, y0 + y, color565);
        if (!isXYBoundsValid(x0 - x, y0 - y)) break;
        setPixel(x0 - x, y0 - y, color565);
        error = 2 * (delta + y) - 1;
        if (delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if (delta > 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

void BareMetalDisplay::drawText(Point start, const char *str, Color color, BaseFont *font) {
    uint8_t x = start.getX();
    uint8_t y = start.getY();
    uint16_t color565 = ColorUtil::rgbToColor565(color);
    std::function<int8_t(uint32_t, uint32_t)> pixelDrawer = [this, color565](uint32_t x, uint32_t y) {
        this->setPixel(x, y, color565);
        return 0;
    };
    while (*str) {
        x = font->drawChar(x, y, *str++, pixelDrawer);
    }
}

/**
 * Push buffer to bare-metal display
 * Using driver method
 */
void BareMetalDisplay::render() {
    driver->write(buffer, buffer_size);
}

void BareMetalDisplay::drawHLine(Point point1, Point point2, Color color) {
    uint8_t x1;
    uint8_t x2;
    uint8_t y = point1.getY();
    if (point1.getX() < point2.getX()) {
        x1 = point1.getX();
        x2 = point2.getX();
    } else {
        x1 = point2.getX();
        x2 = point1.getX();
    }
    uint16_t color565 = ColorUtil::rgbToColor565(color);
    uint32_t index = (y + 1) * driver->getXsize();
    while (x1 <= x2) {
        buffer[x1 + index] = color565;
        x1++;
    }
}

void BareMetalDisplay::drawVLine(Point point1, Point point2, Color color) {
    uint8_t x = point1.getX();
    uint8_t y1;
    uint8_t y2;
    if (point1.getY() < point2.getY()) {
        y1 = point1.getY();
        y2 = point2.getY();
    } else {
        y1 = point2.getY();
        y2 = point1.getY();
    }
    uint16_t color565 = ColorUtil::rgbToColor565(color);
    uint32_t index = (y1 * driver->getXsize()) + x;
    while (y1 <= y2) {
        buffer[index] = color565;
        y1++;
        index = (y1 * driver->getXsize()) + x;
    }
}

void BareMetalDisplay::drawBresenhemsLine(Point point1, Point point2, Color color) {
    uint8_t x1 = point1.getX();
    uint8_t x2 = point2.getX();
    uint8_t y1 = point1.getY();
    uint8_t y2 = point2.getY();
    uint16_t color565 = ColorUtil::rgbToColor565(color);
    uint8_t deltaX = abs(x2 - x1);
    uint8_t deltaY = abs(y2 - y1);
    int8_t signX = x1 < x2 ? 1 : -1;
    int8_t signY = y1 < y2 ? 1 : -1;
    uint8_t error = deltaX - deltaY;
    setPixel(x2, y2, color565);
    while (x1 != x2 || y1 != y2) {
        setPixel(x1, y1, color565);
        int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
        if (!isXYBoundsValid(x1, y1)) break;
    }

}

void BareMetalDisplay::setPixel(uint32_t x, uint32_t y, uint16_t color) {
    buffer[y * driver->getXsize() + x] = color;
}

bool BareMetalDisplay::isPointBoundsValid(Point point) {
    return isXYBoundsValid(point.getX(), point.getY());
}

bool BareMetalDisplay::isXYBoundsValid(uint8_t x, uint8_t y) {
    return !((x >= driver->getXsize()) || (y >= driver->getYsize()));
}

void BareMetalDisplay::drawImage(const uint16_t *image, uint32_t size) {
    if (size > buffer_size) return;
    for (uint32_t i = 0; i < size; i++) {
        buffer[i] = *image;
        image++;
    }
}
