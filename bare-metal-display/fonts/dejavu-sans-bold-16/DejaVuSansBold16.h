//
// Created by brunnen-g on 3/21/23.
//

#ifndef DEJAVU_SANS_BOLD16_H
#define DEJAVU_SANS_BOLD16_H

#include <cstdint>
#include "../../BaseFont.h"

class DejaVuSansBold16 : public BaseFont {

public:
    int32_t drawChar(uint32_t x, uint32_t y, char16_t c, std::function<int8_t(uint32_t, uint32_t)> function);

};

#endif //DEJAVU_SANS_BOLD16_H
