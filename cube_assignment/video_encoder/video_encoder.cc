#include <iostream>

#include "video_encoder.h"

    Video_gen::Video_gen(std::string file_name, std::shared_ptr<Cube> cube, std::shared_ptr<Background> frame): ready_to_right(false) {
        file_ptr.open(file_name, std::ios::out | std::ios::binary);
        cube_ptr = cube;
        frame_ptr = frame;
        if (!file_ptr.is_open()) {
            std::cout << "Could not open file exiting program.... " << std::endl;
            return;
        }
        w_thread = std::thread(&Video_gen::write_frame, this);
        g_thread = std::thread(&Video_gen::write_file, this);
    }

void Video_gen::write_frame() {
    int direction = RIGHT;
    // UPWARD MOVEMENT ---------------------------------------------------
    int iterator = 0;
    int last_pos_x = 0;
    int last_pos_y = 0;

    std::vector<uint8_t> Y_buffer;
    std::vector<uint8_t> U_buffer;
    std::vector<uint8_t> V_buffer;
    std::vector<uint8_t> frame_buffer;

    file_ptr << "YUV4MPEG2 W" << frame_ptr->stride_ << " H" << frame_ptr->height_ << " F" << frame_ptr->frame_rate << ":1 C420\n";


    for (int pos_y = 0; pos_y + cube_ptr->size_ < frame_ptr->height_; pos_y += cube_ptr->step_) {
        if (pos_y + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->height_) {
            std::cout << "pos_y -> " << pos_y << std::endl;
            last_pos_y = pos_y + (cube_ptr->size_ - 1);
        }
        for (int pos_x = 0; pos_x + cube_ptr->size_ <= frame_ptr->stride_; pos_x += cube_ptr->step_) {
            std::fill(Y_buffer.begin(), Y_buffer.end(), frame_ptr->Y_init_);
            std::fill(U_buffer.begin(), U_buffer.end(), frame_ptr->U_init_);
            std::fill(V_buffer.begin(), V_buffer.end(), frame_ptr->V_init_);
            if (pos_x + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->stride_) {
                // -1 because in the draw loops we start from 0
                last_pos_x = pos_x + (cube_ptr->size_ - 1);
            }

            if (direction == RIGHT){
                // right movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        Y_buffer[((pos_y + y) * frame_ptr->stride_) + (pos_x + x)] = cube_ptr->Y_init_;
                    }
                }
                // Chroma
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        U_buffer[((pos_y/2 + y) * (frame_ptr->stride_/2)) + ((pos_x/2) + x)] = cube_ptr->U_init_;
                        V_buffer[((pos_y/2 + y) * (frame_ptr->stride_/2)) + ((pos_x/2) + x)] = cube_ptr->U_init_;
                    }
                }
            } else if (direction == LEFT) {
                // left movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        Y_buffer[((pos_y + y) * frame_ptr->stride_) + (last_pos_x - (x + pos_x))] = cube_ptr->Y_init_;
                    }
                }
                // Chroma
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        U_buffer[((pos_y/2 + y) * (frame_ptr->stride_/2)) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->U_init_;
                        V_buffer[((pos_y/2 + y) * (frame_ptr->stride_/2)) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->V_init_;
                    }
                }
            }
            std::vector<uint8_t> frame_buffer;
            frame_buffer.reserve(Y_buffer.size() + U_buffer.size() + V_buffer.size());

            frame_buffer.insert(frame_buffer.end(), Y_buffer.begin(), Y_buffer.end());
            frame_buffer.insert(frame_buffer.end(), U_buffer.begin(), U_buffer.end());
            frame_buffer.insert(frame_buffer.end(), V_buffer.begin(), V_buffer.end());

            {
                std::unique_lock<std::mutex> lock(mtx);
                queue_frame.push(std::move(frame_buffer));
            }
            cv.notify_one();
        }
        cube_ptr->Y_init_ += cube_ptr->Y_inc_;
        cube_ptr->U_init_ += cube_ptr->U_inc_;
        cube_ptr->V_init_ += cube_ptr->V_inc_;
        direction *= -1;
        iterator++;
    }
    std::cout << "Iteration of downwards -> " << iterator << std::endl;

    std::cout << "Last position of y is -> "<< last_pos_y << std::endl;
    // // Upwards
    iterator = 0;
    std::cout << "Last position of y is -> "<< last_pos_y << std::endl;

    // DOWNWARD MOVEMENT ---------------------------------------------------

    // last_pos_y = pos_y + (s - 1);
    last_pos_y = frame_ptr->height_ - last_pos_y;
    std::cout << " last_pos/2 -> " << last_pos_y/2 << std::endl;

    for (int pos_y = frame_ptr->height_; pos_y >= 0 + cube_ptr->size_; pos_y -= cube_ptr->step_) {
        for (int pos_x = 0; pos_x + cube_ptr->size_ <= frame_ptr->stride_; pos_x += cube_ptr->step_) {
            std::fill(Y_buffer.begin(), Y_buffer.end(), frame_ptr->Y_init_);
            std::fill(U_buffer.begin(), U_buffer.end(), frame_ptr->U_init_);
            std::fill(V_buffer.begin(), V_buffer.end(), frame_ptr->V_init_);
            if (pos_x + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->stride_) {
                // -1 because in the draw loops we start from 0
                last_pos_x = pos_x + (cube_ptr->size_ - 1);
            }

            if (direction == RIGHT){
                // right movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        Y_buffer[((pos_y - last_pos_y - y) * frame_ptr->stride_) + (pos_x + x)] = cube_ptr->Y_init_;
                    }
                }
                // // Chroma
                for (int y = 1; y <= cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        U_buffer[((pos_y/2) - (last_pos_y/2) - y) * (frame_ptr->stride_/2) + ((pos_x/2) + x)] = cube_ptr->U_init_;
                        V_buffer[((pos_y/2) - (last_pos_y/2) - y) * (frame_ptr->stride_/2) + ((pos_x/2) + x)] = cube_ptr->V_init_;
                    }
                }
            } else if (direction == LEFT) {
                // left movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        Y_buffer[((pos_y - last_pos_y -y) * frame_ptr->stride_) + (last_pos_x - (x + pos_x))] = cube_ptr->Y_init_;
                    }
                }
                // // Chroma
                for (int y = 1; y <= cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        U_buffer[(pos_y/2 - last_pos_y/2 - y) * (frame_ptr->stride_/2) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->U_init_;
                        V_buffer[(pos_y/2 - last_pos_y/2 - y) * (frame_ptr->stride_/2) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->V_init_;
                    }
                }
            }
            std::vector<uint8_t> frame_buffer;
            frame_buffer.reserve(Y_buffer.size() + U_buffer.size() + V_buffer.size());

            frame_buffer.insert(frame_buffer.end(), Y_buffer.begin(), Y_buffer.end());
            frame_buffer.insert(frame_buffer.end(), U_buffer.begin(), U_buffer.end());
            frame_buffer.insert(frame_buffer.end(), V_buffer.begin(), V_buffer.end());

            {
                std::unique_lock<std::mutex> lock(mtx);
                queue_frame.push(std::move(frame_buffer));
            }
            cv.notify_one();
            //    file_ptr << "FRAME\n";
            //    file_ptr.write(reinterpret_cast<char*>(Y_buffer.data()), Y_buffer.size());
            //    file_ptr.write(reinterpret_cast<char*>(U_buffer.data()), U_buffer.size());
            //    file_ptr.write(reinterpret_cast<char*>(V_buffer.data()), V_buffer.size());
        }
            cube_ptr->Y_init_ += cube_ptr->Y_inc_;
            cube_ptr->U_init_ += cube_ptr->U_inc_;
            cube_ptr->V_init_ += cube_ptr->V_inc_;
            direction = direction * (-1);
            iterator++;
    }


}



void Video_gen::write_file() {
    while (true) {
        std::vector<uint8_t> frame_buffer;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] {
                return !queue_frame.empty();  // wake up when data or done
            });

            if (queue_frame.empty()) break;  // producer finished and no data left

            frame_buffer = std::move(queue_frame.front());
            queue_frame.pop();
        }

        // write outside the lock
        file_ptr << "FRAME\n";
        file_ptr.write(reinterpret_cast<const char*>(frame_buffer.data()), frame_buffer.size());
    }
}



Video_gen::~Video_gen() {

    cv.notify_all();

    if (w_thread.joinable() && g_thread.joinable()) {
        w_thread.join();
        g_thread.join();
    }

}