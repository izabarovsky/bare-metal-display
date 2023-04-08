#include "pico/stdlib.h"
#include "../bare-metal-display/BareMetalDisplay.h"
#include "../bare-metal-display/drivers/GC9A01_Driver.h"
#include "../bare-metal-display/images/blade.h"
#include "../bare-metal-display/images/eye.h"

int main() {
    GC9A01_Driver driver;
    BareMetalDisplay rgbDisplay(&driver);
    stdio_init_all();
    /** Display init */
    rgbDisplay.init();
    while (true) {
        rgbDisplay.drawImage(blade, 240*240);
        rgbDisplay.render();
        sleep_ms(2000);
        rgbDisplay.drawImage(eye, 240*240);
        rgbDisplay.render();
        sleep_ms(2000);
    }
}
