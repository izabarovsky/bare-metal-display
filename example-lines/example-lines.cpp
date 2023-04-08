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
        rgbDisplay.fillWithColor(BLUE);
        rgbDisplay.drawLine(Point(0, 120), Point(239, 120), RED);
        rgbDisplay.drawLine(Point(120, 0), Point(120, 239), YELLOW);
        rgbDisplay.drawLine(Point(0, 0), Point(239, 239), WHITE);
        rgbDisplay.drawLine(Point(0, 239), Point(239, 0), GREEN);
        rgbDisplay.render();
        sleep_ms(2000);
    }
}
