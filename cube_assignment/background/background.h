#pragma once

#include <vector>

#include "stdint.h";

class Background {
    public :
        Background(int h, int w, uint8_t Y, uint8_t U, uint8_t V);

    private :
        int height;
        int stride;
        uint8_t Y_init;
        uint8_t U_init;
        uint8_t V_init;
};
