//
// Created by brunnen-g on 4/3/23.
//

#ifndef BARE_METAL_DISPLAY_DRIVER_H
#define BARE_METAL_DISPLAY_DRIVER_H

#include <cstdint>

/**
 * Interface to display
 */
class DisplayDriver {
public:
    /**
     * Init display (init spi, i2c etc., send init instructions)
     */
    virtual void init() = 0;

    /**
     * Send frame buffer to display
     * @param buffer - buffer pointer
     * @param length - buffer size
     */
    virtual void write(uint16_t *buffer, uint32_t length) = 0;

    /**
     * Get x-axis display size
     * @return
     */
    virtual uint32_t getXsize() = 0;

    /**
     * Get y-axis display size
     * @return
     */
    virtual uint32_t getYsize() = 0;
};


#endif //BARE_METAL_DISPLAY_DRIVER_H
