#pragma once

#include <stdint.h>
#include <vector>

class Cube {
    public :
        Cube(int size_cube, int step_cube, uint8_t Y_init_value_,
                uint8_t U_init_value_, uint8_t V_init_value_, uint8_t
                y_inc, uint8_t u_inc, uint8_t v_inc);
        int size_;
        int step_;
        uint8_t Y_init_;
        uint8_t U_init_;
        uint8_t V_init_;
        uint8_t Y_inc_;
        uint8_t U_inc_;
        uint8_t V_inc_;
};