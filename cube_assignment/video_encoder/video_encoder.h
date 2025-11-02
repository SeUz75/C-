#pragma once

#include <vector>
#include <stdint.h>

class Video_gen {
    public :
        Video_gen();
    private :
        std::vector<uint8_t> Y_buffer;
        std::vector<uint8_t> U_buffer;
        std::vector<uint8_t> V_buffer;
};