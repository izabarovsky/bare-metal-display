/**
  * @file GC9A01.c
  *
 **/


/*********************
 *      INCLUDES
 *********************/
#include "GC9A01.h"
#include <cstdio>
#include <cstring>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

const uint GC9A01_CD_PIN = 18;
const uint GC9A01_CS_PIN = 20;
const uint GC9A01_RST_PIN = 19;

/*********************
 *      DEFINES
 *********************/
#ifndef GC9A01_XSTART
#define GC9A01_XSTART    0
#endif
#ifndef GC9A01_YSTART
#define GC9A01_YSTART    0
#endif

#define GC9A01_CMD_MODE        0
#define GC9A01_DATA_MODE    1

#define GC9A01_X_SIZE    240
#define GC9A01_Y_SIZE    240

/* GC9A01 Commands that we know of.  Limited documentation */
#define GC9A01_INVOFF        0x20
#define GC9A01_INVON        0x21
#define GC9A01_DISPON        0x29
#define GC9A01_CASET        0x2A
#define GC9A01_RASET        0x2B
#define GC9A01_RAMWR        0x2C
#define GC9A01_COLMOD        0x3A
#define GC9A01_MADCTL        0x36
#define GC9A01_MADCTL_MY    0x80
#define GC9A01_MADCTL_MX    0x40
#define GC9A01_MADCTL_MV    0x20
#define GC9A01_MADCTL_RGB    0x00
#define GC9A01_DISFNCTRL    0xB6

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void GC9A01_command(uint8_t cmd);

static void GC9A01_data(uint8_t data);

static void GC9A01_setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void GC9A01_write(uint16_t *buffer, uint32_t length) {
    gpio_put(GC9A01_CS_PIN, false);
    GC9A01_setAddr(0, 0, GC9A01_X_SIZE - 1, GC9A01_Y_SIZE - 1);
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
                          &buffer, // read address
                          length, // element count (each element is of size transfer_data_size)
                          true);
    dma_channel_wait_for_finish_blocking(dma_tx);
    gpio_put(GC9A01_CS_PIN, true);
    dma_channel_unclaim(dma_tx);
    spi_set_format(spi_default, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
}

/**
 * Write a command to the GC9A01
 * @param cmd the command
 */
static void GC9A01_command(uint8_t cmd) {
    gpio_put(GC9A01_CD_PIN, GC9A01_CMD_MODE);
    spi_write_blocking(spi_default, &cmd, 1);
}

/**
 * Write data to the GC9A01
 * @param data the data
 */
static void GC9A01_data(uint8_t data) {
    gpio_put(GC9A01_CD_PIN, GC9A01_DATA_MODE);
    spi_write_blocking(spi_default, &data, 1);
}

/**
 * Initialize the GC9A01
 */
void GC9A01_init() {
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
    // Reset
    gpio_put(GC9A01_CS_PIN, false);
    gpio_put(GC9A01_RST_PIN, true);
    sleep_ms(50);
    gpio_put(GC9A01_RST_PIN, false);
    sleep_ms(50);
    gpio_put(GC9A01_RST_PIN, true);
    sleep_ms(50);
    // End Reset
    GC9A01_command(0xEF);
    GC9A01_command(0xEB);
    GC9A01_data(0x14);
    GC9A01_command(0xFE);  // Inter Register Enable1
    GC9A01_command(0xEF);  // Inter Register Enable2
    GC9A01_command(0xEB);
    GC9A01_data(0x14);
    GC9A01_command(0x84);
    GC9A01_data(0x40);
    GC9A01_command(0x85);
    GC9A01_data(0xFF);
    GC9A01_command(0x86);
    GC9A01_data(0xFF);
    GC9A01_command(0x87);
    GC9A01_data(0xFF);
    GC9A01_command(0x88);
    GC9A01_data(0x0A);
    GC9A01_command(0x89);
    GC9A01_data(0x21);
    GC9A01_command(0x8A);
    GC9A01_data(0x00);
    GC9A01_command(0x8B);
    GC9A01_data(0x80);
    GC9A01_command(0x8C);
    GC9A01_data(0x01);
    GC9A01_command(0x8D);
    GC9A01_data(0x01);
    GC9A01_command(0x8E);
    GC9A01_data(0xFF);
    GC9A01_command(0x8F);
    GC9A01_data(0xFF);
    GC9A01_command(GC9A01_DISFNCTRL); // Display Function Control
    GC9A01_data(0x00);
    GC9A01_data(0x00);
    GC9A01_command(GC9A01_MADCTL); // Memory Access Control
    GC9A01_data(0x48); // Set the display direction 0,1,2,3	four directions
    GC9A01_command(GC9A01_COLMOD); // COLMOD: Pixel Format Set
    GC9A01_data(0x05); // 16 Bits per pixel
    GC9A01_command(0x90);
    GC9A01_data(0x08);
    GC9A01_data(0x08);
    GC9A01_data(0x08);
    GC9A01_data(0x08);
    GC9A01_command(0xBD);
    GC9A01_data(0x06);
    GC9A01_command(0xBC);
    GC9A01_data(0x00);
    GC9A01_command(0xFF);
    GC9A01_data(0x60);
    GC9A01_data(0x01);
    GC9A01_data(0x04);
    GC9A01_command(0xC3); // Power Control 2
    GC9A01_data(0x13);
    GC9A01_command(0xC4); // Power Control 3
    GC9A01_data(0x13);
    GC9A01_command(0xC9); // Power Control 4
    GC9A01_data(0x22);
    GC9A01_command(0xBE);
    GC9A01_data(0x11);
    GC9A01_command(0xE1);
    GC9A01_data(0x10);
    GC9A01_data(0x0E);
    GC9A01_command(0xDF);
    GC9A01_data(0x21);
    GC9A01_data(0x0C);
    GC9A01_data(0x02);
    GC9A01_command(0xF0); // SET_GAMMA1
    GC9A01_data(0x45);
    GC9A01_data(0x09);
    GC9A01_data(0x08);
    GC9A01_data(0x08);
    GC9A01_data(0x26);
    GC9A01_data(0x2A);
    GC9A01_command(0xF1); // SET_GAMMA2
    GC9A01_data(0x43);
    GC9A01_data(0x70);
    GC9A01_data(0x72);
    GC9A01_data(0x36);
    GC9A01_data(0x37);
    GC9A01_data(0x6F);
    GC9A01_command(0xF2); // SET_GAMMA3
    GC9A01_data(0x45);
    GC9A01_data(0x09);
    GC9A01_data(0x08);
    GC9A01_data(0x08);
    GC9A01_data(0x26);
    GC9A01_data(0x2A);
    GC9A01_command(0xF3); // SET_GAMMA4
    GC9A01_data(0x43);
    GC9A01_data(0x70);
    GC9A01_data(0x72);
    GC9A01_data(0x36);
    GC9A01_data(0x37);
    GC9A01_data(0x6F);
    GC9A01_command(0xED);
    GC9A01_data(0x1B);
    GC9A01_data(0x0B);
    GC9A01_command(0xAE);
    GC9A01_data(0x77);
    GC9A01_command(0xCD);
    GC9A01_data(0x63);
    GC9A01_command(0x70);
    GC9A01_data(0x07);
    GC9A01_data(0x07);
    GC9A01_data(0x04);
    GC9A01_data(0x0E);
    GC9A01_data(0x0F);
    GC9A01_data(0x09);
    GC9A01_data(0x07);
    GC9A01_data(0x08);
    GC9A01_data(0x03);
    GC9A01_command(0xE8);
    GC9A01_data(0x34);
    GC9A01_command(0x62);
    GC9A01_data(0x18);
    GC9A01_data(0x0D);
    GC9A01_data(0x71);
    GC9A01_data(0xED);
    GC9A01_data(0x70);
    GC9A01_data(0x70);
    GC9A01_data(0x18);
    GC9A01_data(0x0F);
    GC9A01_data(0x71);
    GC9A01_data(0xEF);
    GC9A01_data(0x70);
    GC9A01_data(0x70);
    GC9A01_command(0x63);
    GC9A01_data(0x18);
    GC9A01_data(0x11);
    GC9A01_data(0x71);
    GC9A01_data(0xF1);
    GC9A01_data(0x70);
    GC9A01_data(0x70);
    GC9A01_data(0x18);
    GC9A01_data(0x13);
    GC9A01_data(0x71);
    GC9A01_data(0xF3);
    GC9A01_data(0x70);
    GC9A01_data(0x70);
    GC9A01_command(0x64);
    GC9A01_data(0x28);
    GC9A01_data(0x29);
    GC9A01_data(0xF1);
    GC9A01_data(0x01);
    GC9A01_data(0xF1);
    GC9A01_data(0x00);
    GC9A01_data(0x07);
    GC9A01_command(0x66);
    GC9A01_data(0x3C);
    GC9A01_data(0x00);
    GC9A01_data(0xCD);
    GC9A01_data(0x67);
    GC9A01_data(0x45);
    GC9A01_data(0x45);
    GC9A01_data(0x10);
    GC9A01_data(0x00);
    GC9A01_data(0x00);
    GC9A01_data(0x00);
    GC9A01_command(0x67);
    GC9A01_data(0x00);
    GC9A01_data(0x3C);
    GC9A01_data(0x00);
    GC9A01_data(0x00);
    GC9A01_data(0x00);
    GC9A01_data(0x01);
    GC9A01_data(0x54);
    GC9A01_data(0x10);
    GC9A01_data(0x32);
    GC9A01_data(0x98);
    GC9A01_command(0x74);
    GC9A01_data(0x10);
    GC9A01_data(0x85);
    GC9A01_data(0x80);
    GC9A01_data(0x00);
    GC9A01_data(0x00);
    GC9A01_data(0x4E);
    GC9A01_data(0x00);
    GC9A01_command(0x98);
    GC9A01_data(0x3E);
    GC9A01_data(0x07);
    GC9A01_command(0x35); // Tearing Effect Line ON
    GC9A01_command(0x21); // Display Inversion ON
    GC9A01_command(0x11); // Sleep Out Mode
    sleep_ms(120);
    GC9A01_command(GC9A01_DISPON); // Display ON
    sleep_ms(255);
}

static void GC9A01_setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint16_t x_start = x0 + GC9A01_XSTART, x_end = x1 + GC9A01_XSTART;
    uint16_t y_start = y0 + GC9A01_YSTART, y_end = y1 + GC9A01_YSTART;
    GC9A01_command(GC9A01_CASET); // Column addr set
    GC9A01_data(x_start >> 8);
    GC9A01_data(x_start & 0xFF);     // XSTART
    GC9A01_data(x_end >> 8);
    GC9A01_data(x_end & 0xFF);     // XEND
    GC9A01_command(GC9A01_RASET); // Row addr set
    GC9A01_data(y_start >> 8);
    GC9A01_data(y_start & 0xFF);     // YSTART
    GC9A01_data(y_end >> 8);
    GC9A01_data(y_end & 0xFF);     // YEND
    GC9A01_command(GC9A01_RAMWR);
}

