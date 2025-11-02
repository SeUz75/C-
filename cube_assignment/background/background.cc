#include "background.h"

Background::Background(int h, int w, uint8_t Y_background, uint8_t U_background,
                       uint8_t V_background){

        height_ = h;
        stride_ = w;
        Y_init_ = Y_background;
        U_init_ = U_background;
        V_init_ = V_background;
    }

int Background::get_height() {
    return height_;
}

int Background::get_width() {
    return stride_;
}

uint8_t Background::get_Y() {
    return Y_init_;
}

uint8_t Background::get_U() {
    return U_init_;
}
uint8_t Background::get_V() {
    return V_init_;
}