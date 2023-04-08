//
// Created by brunnen-g on 3/21/23.
//

#include "DejaVuSansBold16.h"
#include "deajavu_sans_bold_16_varsize_raw_1bpp.h"

int32_t DejaVuSansBold16::drawChar(uint32_t x,
                                   uint32_t y,
                                   char16_t c,
                                   std::function<int8_t(uint32_t, uint32_t)> function) {
    font_table_t smbl_data;
    for (auto i: dejavusans_bold16_table) {
        if (i.symbol == c) {
            smbl_data = i;
            break;
        }
    }
    for (uint8_t h = 0; h < smbl_data.height; h++) {
        uint16_t data = dejavusans_bold16_bmp[smbl_data.bmp_index + h];
        for (uint8_t w = 0; w < smbl_data.width; w++) {
            if (data & 0x01) function(x + w, y + h);
            data = data >> 1;
        }
    }
    return x + smbl_data.width;
}
