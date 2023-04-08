#include "pico/stdlib.h"
#include "../bare-metal-display/BareMetalDisplay.h"
#include "../bare-metal-display/drivers/GC9A01_Driver.h"

int main() {
    GC9A01_Driver driver;
    BareMetalDisplay rgbDisplay(&driver);
    stdio_init_all();
    /** Display init */
    rgbDisplay.init();
    while (true) {
        rgbDisplay.fillWithColor(BLACK);
        rgbDisplay.drawCircle(Point(120, 120), 50, RED);
        rgbDisplay.drawCircle(Point(200, 200), 10, GREEN);
        rgbDisplay.render();
        sleep_ms(2000);
    }
}
