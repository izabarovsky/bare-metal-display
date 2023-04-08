//
// Created by brunnen-g on 3/29/23.
//

#ifndef BASIC8X8_H
#define BASIC8X8_H

#include "../../BaseFont.h"

class Basic8x8 : public BaseFont {

public:
    int32_t drawChar(uint32_t x, uint32_t y, char16_t c, std::function<int8_t(uint32_t, uint32_t)> function) override;
};

#endif //BASIC8X8_H
