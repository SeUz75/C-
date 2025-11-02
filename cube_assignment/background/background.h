#pragma once

#include <vector>

#include "stdint.h"

class Background {
    public :
        Background(int h, int w, uint8_t Y, uint8_t U, uint8_t V);
        int get_height();
        int get_width();
        uint8_t get_Y();
        uint8_t get_U();
        uint8_t get_V();

    private :
        int height_;
        int stride_;
        uint8_t Y_init_;
        uint8_t U_init_;
        uint8_t V_init_;
};
