//
// Created by brunnen-g on 4/3/23.
//

#ifndef BARE_METAL_DISPLAY_GC9A01_DRIVER_H
#define BARE_METAL_DISPLAY_GC9A01_DRIVER_H

#include "DisplayDriver.h"

class GC9A01_Driver : public DisplayDriver {
public:
    void init() override;

    void write(uint16_t *buffer, uint32_t length) override;

    uint32_t getXsize() override;

    uint32_t getYsize() override;

private:
    void initSPI();

    void hardReset();

    void initSequence();

    void command(uint8_t cmd);

    void data(uint8_t data);

    void setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
};

#endif //BARE_METAL_DISPLAY_GC9A01_DRIVER_H
