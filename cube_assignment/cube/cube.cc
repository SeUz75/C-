#include "cube.h"

Cube::Cube(int cube_size, int step_size, uint8_t Y, uint8_t U, uint8_t V) {
    size_ = cube_size;
    step_ = step_size;

    Y_init_value_.assign(size_, Y);
    U_init_value_.assign((size_/4), U);
    V_init_value_.assign((size_/4), V);

}
