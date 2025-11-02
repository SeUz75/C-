#include "cube.h"

Cube::Cube(int cube_size, int step_size, uint8_t Y, uint8_t U, uint8_t V) {
    size_ = cube_size;
    step_ = step_size;
    Y_init_ = Y;
    U_init_ = U;
    V_init_ = V;
}

int Cube::get_size() {
    return size_;
}

int Cube::get_step() {
    return step_;
}

uint8_t Cube::get_Y() {
    return Y_init_;
}

uint8_t Cube::get_U() {
    return U_init_;
}

uint8_t Cube::get_V() {
    return V_init_;
}
