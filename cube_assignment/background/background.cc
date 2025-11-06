#include "background.h"

Background::Background(int h, int w, int frame, uint8_t Y_background, uint8_t U_background,
                       uint8_t V_background){

        height_ = h;
        stride_ = w;
        frame_rate = frame;
        Y_init_ = Y_background;
        U_init_ = U_background;
        V_init_ = V_background;
    }
