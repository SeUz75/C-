#pragma once

#include <stdint.h>
#include <vector>

class Cube {
    public :
        Cube(int size_cube, int step_cube, uint8_t Y_init_value_,
                uint8_t U_init_value_, uint8_t V_init_value_);
        int get_size();
        int get_step();
        uint8_t get_Y();
        uint8_t get_U();
        uint8_t get_V();

     private :
        int size_;
        int step_;
        uint8_t Y_init_;
        uint8_t U_init_;
        uint8_t V_init_;
};