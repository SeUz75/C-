#pragma once

#include <vector>
#include <memory>
#include "stdint.h"
#include "cube.h"

class Background {
    public :
        Background(int h, int w, int frame_rate, uint8_t Y, uint8_t U, uint8_t V);
        void FillBackGround(std::shared_ptr<std::vector<uint8_t>> &curre_buffer);
        void DrawSquare (std::shared_ptr<std::vector<uint8_t>> &curre_buffer, std::shared_ptr<Cube> &cube_ptr);

        uint64_t GetYSize();
        uint64_t GetUSize();
        uint64_t height_;
        uint64_t width_;
        int fps;
        uint8_t y_;
        uint8_t u_;
        uint8_t v_;
};
