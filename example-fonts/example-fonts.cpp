#include "pico/stdlib.h"
#include "../bare-metal-display/BareMetalDisplay.h"
#include "../bare-metal-display/fonts/dejavu-sans-bold-16/DejaVuSansBold16.h"
#include "../bare-metal-display/fonts/basic-8x8/Basic8x8.h"
#include "../bare-metal-display/drivers/GC9A01_Driver.h"

int main() {
    GC9A01_Driver driver;
    BareMetalDisplay rgbDisplay(&driver);
    DejaVuSansBold16 dejaVuSansBold16;
    Basic8x8 basic8X8;
    stdio_init_all();
    /** Display init */
    rgbDisplay.init();
    while (true) {
        rgbDisplay.fillWithColor(BLUE);
        rgbDisplay.drawText(Point(30, 100), "dejaVuSansBold16", YELLOW, &dejaVuSansBold16);
        rgbDisplay.drawText(Point(30, 130), "basic mono 8x8 px", RED, &basic8X8);
        rgbDisplay.render();
        sleep_ms(2000);
    }
}
