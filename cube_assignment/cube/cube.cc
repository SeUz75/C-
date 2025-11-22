#include "cube.h"

Cube::Cube(int cube_size, int step_size, uint8_t Y, uint8_t U, uint8_t V, uint8_t
                y_inc, uint8_t u_inc, uint8_t v_inc) {
    width_ = cube_size;
    step_ = step_size;
    y_ = Y;
    u_ = U;
    v_ = V;
    Y_inc_ = y_inc;
    U_inc_ = u_inc;
    V_inc_ = v_inc;
    pos_x = 0;
    pos_y = 0;
}
