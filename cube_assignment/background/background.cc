#include "background.h"

Background::Background(int h, int w, uint8_t Y_background, uint8_t U_background,
                       uint8_t V_background){

        height = h;
        stride = w;
        Y_init = Y_background;
        U_init = U_background;
        V_init = V_background;
    }