#pragma once

#include <vector>

#include "stdint.h"

class Background {
    public :
        Background(int h, int w, int frame_rate, uint8_t Y, uint8_t U, uint8_t V);
        int height_;
        int stride_;
        int frame_rate;
        uint8_t Y_init_;
        uint8_t U_init_;
        uint8_t V_init_;
};
