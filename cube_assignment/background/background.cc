#include "background.h"

Background::Background(int h, int w, int frame, uint8_t Y_background, uint8_t U_background,
                       uint8_t V_background){

    height_ = h;
    width_ = w;
    fps = frame;
    y_ = Y_background;
    u_ = U_background;
    v_ = V_background;
}


uint64_t Background::GetYSize() {
    return width_ * height_;
}

uint64_t Background::GetUSize() {
    return (width_ * height_) / 4;
}

void Background::FillBackGround(std::shared_ptr<std::vector<uint8_t>> &curre_buffer) {
        std::fill(curre_buffer->begin(), curre_buffer->begin() + GetYSize(), y_);
        std::fill(curre_buffer->begin() + GetYSize(), curre_buffer->begin() + GetYSize() + GetUSize(),
        u_);
        std::fill(curre_buffer->begin() + GetYSize() + GetUSize(), curre_buffer->end(),
        v_);
}

void Background::DrawSquare(std::shared_ptr<std::vector<uint8_t>> &currentFrame, std::shared_ptr<Cube> &square_ptr) {
    uint64_t y_size = GetYSize();
    uint64_t u_size = GetUSize();

    for (size_t y = 0; y < square_ptr->width_ / 2; y++) {
        for (size_t x = 0; x < square_ptr->width_ / 2; x++) {
            int index =
                (square_ptr->pos_y / 2 + y) * width_ / 2 +
                (square_ptr->pos_x / 2 + x);
            (*currentFrame)[y_size + index] = square_ptr->u_;
            (*currentFrame)[y_size + u_size + index] = square_ptr->v_;
        }
    }
}
