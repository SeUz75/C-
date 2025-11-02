#pragma once

#include <stdint.h>
#include <vector>

class Cube {
    public :
        Cube(int size_cube, int step_cube, uint8_t Y_init_value_,
                uint8_t U_init_value_, uint8_t V_init_value_);

     private :
        int size_;
        int step_;
        std::vector<uint8_t> Y_init_value_;
        std::vector<uint8_t> U_init_value_;
        std::vector<uint8_t> V_init_value_;
};