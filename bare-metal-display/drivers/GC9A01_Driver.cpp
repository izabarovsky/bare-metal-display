//
// Created by brunnen-g on 4/3/23.
//

#include <cstdio>
#include "GC9A01_Driver.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

/** Constants */
const uint8_t GC9A01_CD_PIN = 18;
const uint8_t GC9A01_CS_PIN = 20;
const uint8_t GC9A01_RST_PIN = 19;

const uint8_t GC9A01_XSTART = 0;
const uint8_t GC9A01_YSTART = 0;

const uint8_t GC9A01_CMD_MODE = 0;
const uint8_t GC9A01_DATA_MODE = 1;

const uint8_t GC9A01_X_SIZE = 240;
const uint8_t GC9A01_Y_SIZE = 240;

/** Some display commands */
const uint8_t GC9A01_DISPON = 0x29;
const uint8_t GC9A01_CASET = 0x2A;
const uint8_t GC9A01_RASET = 0x2B;
const uint8_t GC9A01_RAMWR = 0x2C;
const uint8_t GC9A01_COLMOD = 0x3A;
const uint8_t GC9A01_MADCTL = 0x36;
const uint8_t GC9A01_DISFNCTRL = 0xB6;

/**
 * Init display:
 * - init SPI
 * - hardware reset
 * - send required commands and init data
 */
void GC9A01_Driver::init() {
    initSPI();
    hardReset();
    initSequence();
}

/**
 * Write frame buffer to display
 * @param buffer
 * @param length
 * @brief Used DMA to decrease write time. Display memory contains 16-bit per pixel,
 * so used 16-bit SPA mode
 */
void GC9A01_Driver::write(uint16_t *buffer, uint32_t length) {
    gpio_put(GC9A01_CS_PIN, false);
    setAddr(0, 0, GC9A01_X_SIZE - 1, GC9A01_Y_SIZE - 1);
    gpio_put(GC9A01_CS_PIN, true);
    spi_set_format(spi_default, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_put(GC9A01_CS_PIN, false);
    gpio_put(GC9A01_CD_PIN, GC9A01_DATA_MODE);
    const uint dma_tx = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_tx);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_dreq(&c, spi_get_dreq(spi_default, true));
    channel_config_set_write_increment(&c, false);
    channel_config_set_read_increment(&c, true);
    dma_channel_set_config(dma_tx, &c, false);
    dma_channel_configure(dma_tx, &c,
                          &spi_get_hw(spi_default)->dr, // write address
                          buffer, // read address
                          length, // element count (each element is of size transfer_data_size)
                          true);
    dma_channel_wait_for_finish_blocking(dma_tx);
    gpio_put(GC9A01_CS_PIN, true);
    dma_channel_unclaim(dma_tx);
    spi_set_format(spi_default, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

/**
 * Init SPI interface, pins configured throw constants
 */
void GC9A01_Driver::initSPI() {
    // Init DC pin
    gpio_init(GC9A01_CD_PIN);
    gpio_set_dir(GC9A01_CD_PIN, GPIO_OUT);
    // Init CS pin
    gpio_init(GC9A01_CS_PIN);
    gpio_set_dir(GC9A01_CS_PIN, GPIO_OUT);
    // Init RST pin
    gpio_init(GC9A01_RST_PIN);
    gpio_set_dir(GC9A01_RST_PIN, GPIO_OUT);
    // Init SPI
    spi_init(spi_default, 20 * 1000 * 1000);
    gpio_set_function(2, GPIO_FUNC_SPI);
    gpio_set_function(3, GPIO_FUNC_SPI);
}

/**
 * Reset display after power on
 */
void GC9A01_Driver::hardReset() {
    gpio_put(GC9A01_CS_PIN, false);
    gpio_put(GC9A01_RST_PIN, true);
    sleep_ms(50);
    gpio_put(GC9A01_RST_PIN, false);
    sleep_ms(50);
    gpio_put(GC9A01_RST_PIN, true);
    sleep_ms(50);
}

/**
 * Init display with required commands
 * Couldn't find full docs, so just get some useful code from internet
 * As result, It's looks like ugly magic
 */
void GC9A01_Driver::initSequence() {
    command(0xEF);
    command(0xEB);
    data(0x14);
    command(0xFE);  // Inter Register Enable1
    command(0xEF);  // Inter Register Enable2
    command(0xEB);
    data(0x14);
    command(0x84);
    data(0x40);
    command(0x85);
    data(0xFF);
    command(0x86);
    data(0xFF);
    command(0x87);
    data(0xFF);
    command(0x88);
    data(0x0A);
    command(0x89);
    data(0x21);
    command(0x8A);
    data(0x00);
    command(0x8B);
    data(0x80);
    command(0x8C);
    data(0x01);
    command(0x8D);
    data(0x01);
    command(0x8E);
    data(0xFF);
    command(0x8F);
    data(0xFF);
    command(GC9A01_DISFNCTRL); // Display Function Control
    data(0x00);
    data(0x00);
    command(GC9A01_MADCTL); // Memory Access Control
    data(0x48); // Set the display direction 0,1,2,3	four directions
    command(GC9A01_COLMOD); // COLMOD: Pixel Format Set
    data(0x05); // 16 Bits per pixel
    command(0x90);
    data(0x08);
    data(0x08);
    data(0x08);
    data(0x08);
    command(0xBD);
    data(0x06);
    command(0xBC);
    data(0x00);
    command(0xFF);
    data(0x60);
    data(0x01);
    data(0x04);
    command(0xC3); // Power Control 2
    data(0x13);
    command(0xC4); // Power Control 3
    data(0x13);
    command(0xC9); // Power Control 4
    data(0x22);
    command(0xBE);
    data(0x11);
    command(0xE1);
    data(0x10);
    data(0x0E);
    command(0xDF);
    data(0x21);
    data(0x0C);
    data(0x02);
    command(0xF0); // SET_GAMMA1
    data(0x45);
    data(0x09);
    data(0x08);
    data(0x08);
    data(0x26);
    data(0x2A);
    command(0xF1); // SET_GAMMA2
    data(0x43);
    data(0x70);
    data(0x72);
    data(0x36);
    data(0x37);
    data(0x6F);
    command(0xF2); // SET_GAMMA3
    data(0x45);
    data(0x09);
    data(0x08);
    data(0x08);
    data(0x26);
    data(0x2A);
    command(0xF3); // SET_GAMMA4
    data(0x43);
    data(0x70);
    data(0x72);
    data(0x36);
    data(0x37);
    data(0x6F);
    command(0xED);
    data(0x1B);
    data(0x0B);
    command(0xAE);
    data(0x77);
    command(0xCD);
    data(0x63);
    command(0x70);
    data(0x07);
    data(0x07);
    data(0x04);
    data(0x0E);
    data(0x0F);
    data(0x09);
    data(0x07);
    data(0x08);
    data(0x03);
    command(0xE8);
    data(0x34);
    command(0x62);
    data(0x18);
    data(0x0D);
    data(0x71);
    data(0xED);
    data(0x70);
    data(0x70);
    data(0x18);
    data(0x0F);
    data(0x71);
    data(0xEF);
    data(0x70);
    data(0x70);
    command(0x63);
    data(0x18);
    data(0x11);
    data(0x71);
    data(0xF1);
    data(0x70);
    data(0x70);
    data(0x18);
    data(0x13);
    data(0x71);
    data(0xF3);
    data(0x70);
    data(0x70);
    command(0x64);
    data(0x28);
    data(0x29);
    data(0xF1);
    data(0x01);
    data(0xF1);
    data(0x00);
    data(0x07);
    command(0x66);
    data(0x3C);
    data(0x00);
    data(0xCD);
    data(0x67);
    data(0x45);
    data(0x45);
    data(0x10);
    data(0x00);
    data(0x00);
    data(0x00);
    command(0x67);
    data(0x00);
    data(0x3C);
    data(0x00);
    data(0x00);
    data(0x00);
    data(0x01);
    data(0x54);
    data(0x10);
    data(0x32);
    data(0x98);
    command(0x74);
    data(0x10);
    data(0x85);
    data(0x80);
    data(0x00);
    data(0x00);
    data(0x4E);
    data(0x00);
    command(0x98);
    data(0x3E);
    data(0x07);
    command(0x35); // Tearing Effect Line ON
    command(0x21); // Display Inversion ON
    command(0x11); // Sleep Out Mode
    sleep_ms(120);
    command(GC9A01_DISPON); // Display ON
    sleep_ms(255);
}

/**
 * Send command byte
 * @param cmd
 */
void GC9A01_Driver::command(uint8_t cmd) {
    gpio_put(GC9A01_CD_PIN, GC9A01_CMD_MODE);
    spi_write_blocking(spi_default, &cmd, 1);
}

/**
 * Send data byte
 * @param data
 */
void GC9A01_Driver::data(uint8_t data) {
    gpio_put(GC9A01_CD_PIN, GC9A01_DATA_MODE);
    spi_write_blocking(spi_default, &data, 1);
}

/**
 * Set address
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 */
void GC9A01_Driver::setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint16_t x_start = x0 + GC9A01_XSTART, x_end = x1 + GC9A01_XSTART;
    uint16_t y_start = y0 + GC9A01_YSTART, y_end = y1 + GC9A01_YSTART;
    command(GC9A01_CASET); // Column addr set
    data(x_start >> 8);
    data(x_start & 0xFF);     // XSTART
    data(x_end >> 8);
    data(x_end & 0xFF);     // XEND
    command(GC9A01_RASET); // Row addr set
    data(y_start >> 8);
    data(y_start & 0xFF);     // YSTART
    data(y_end >> 8);
    data(y_end & 0xFF);     // YEND
    command(GC9A01_RAMWR);
}

uint32_t GC9A01_Driver::getXsize() {
    return GC9A01_X_SIZE;
}

uint32_t GC9A01_Driver::getYsize() {
    return GC9A01_Y_SIZE;
}
