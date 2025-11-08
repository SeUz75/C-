#include <iostream>
#include "video_encoder.h"

Video_gen::Video_gen(std::string file_name, std::shared_ptr<Cube> cube, std::shared_ptr<Background> frame): finished(false) {
    file_ptr.open(file_name, std::ios::out | std::ios::binary);
    std::cout << "Constructor called !" << std::endl;
    if (!file_ptr.is_open()) {
        std::cout << "Could not open file exiting program.... " << std::endl;
        return;
    }

    cube_ptr = cube;
    frame_ptr = frame;
    total_size = (frame_ptr->stride_ * frame_ptr->height_) * 3 / 2;
    buffer_a = std::make_shared<std::vector<uint8_t>>(total_size);
    buffer_c = std::make_shared<std::vector<uint8_t>>(total_size);
    buffer_b = std::make_shared<std::vector<uint8_t>>(total_size);
    std::cout << "3 pointers poiting to vector created ! " << std::endl;
    {
        std::unique_lock<std::mutex> lock(draw_mtx);
        draw_queue.push(buffer_a);
        draw_queue.push(buffer_b);
        draw_queue.push(buffer_c);
        std::cout << "Pushed all pointer vectors into the queue  !" << std::endl;
    }
    std::cout << "Notifying write condition...." << std::endl;
    draw_cv.notify_one();

    // Threads start working
        // W thread will cond wait for the buffers to be pushed into the queue
    draw_thread = std::thread(&Video_gen::draw_frame, this);

    // Inside the w thread func we will wake up the g_thread to ge
    write_thread = std::thread(&Video_gen::write_file, this);
}


void Video_gen::draw_frame() {
    int direction = RIGHT;
    // UPWARD MOVEMENT ---------------------------------------------------
    int iterator = 0;
    int last_pos_x = 0;
    int last_pos_y = 0;

    int y_size = frame_ptr->stride_ * frame_ptr->height_;
    int u_size = frame_ptr->stride_ * frame_ptr->height_ / 4;

    // Lambda function to fill the background of the image
    auto fill_background = [&] (std::shared_ptr<std::vector<uint8_t>> buffer) {
        std::fill(buffer->begin(), buffer->begin() + y_size, frame_ptr->Y_init_);
        std::fill(buffer->begin() + y_size, buffer->begin() + y_size + u_size, frame_ptr->U_init_);
        std::fill(buffer->begin() + y_size + u_size, buffer->end(), frame_ptr->V_init_);
    };

    file_ptr << "YUV4MPEG2 W" << frame_ptr->stride_ << " H" << frame_ptr->height_ << " F" << frame_ptr->frame_rate << ":1 C420\n";

    std::shared_ptr<std::vector<uint8_t>> free_buffer; // This free_buff will host one of the 3 available buffers inside the write_queue;

    for (int pos_y = 0; pos_y + cube_ptr->size_ < frame_ptr->height_; pos_y += cube_ptr->step_) {
        if (pos_y + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->height_) {
            std::cout << "pos_y -> " << pos_y << std::endl;
            last_pos_y = pos_y + (cube_ptr->size_ - 1);
        }
        for (int pos_x = 0; pos_x + cube_ptr->size_ <= frame_ptr->stride_; pos_x += cube_ptr->step_) {

            if (pos_x + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->stride_) {
                // -1 because in the draw loops we start from 0
                last_pos_x = pos_x + (cube_ptr->size_ - 1);
            }

            {
                std::unique_lock<std::mutex> lock(draw_mtx);
                std::cout << "waiting for available buffers... write frame..." << std::endl;
                draw_cv.wait(lock, [&]{ return !draw_queue.empty(); });
                std::cout << "Buffer poped from write_queue ready to be written " << std::endl;
                free_buffer = std::move(draw_queue.front());
                draw_queue.pop();
            }

            // Fill background
            fill_background(free_buffer);

            std::cout << "Starting frame writing !" << std::endl;

            if (direction == RIGHT){
                // right movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*free_buffer)[((pos_y + y) * frame_ptr->stride_) + (pos_x + x)] = cube_ptr->Y_init_;
                    }
                }
                // Chroma
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        (*free_buffer)[((pos_y/2 + y) * (frame_ptr->stride_/2)) + ((pos_x/2) + x)] = cube_ptr->U_init_;
                        (*free_buffer)[((pos_y/2 + y) * (frame_ptr->stride_/2)) + ((pos_x/2) + x)] = cube_ptr->U_init_;
                    }
                }
            } else if (direction == LEFT) {
                // left movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*free_buffer)[((pos_y + y) * frame_ptr->stride_) + (last_pos_x - (x + pos_x))] = cube_ptr->Y_init_;
                    }
                }
                // Chroma
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        (*free_buffer)[((pos_y/2 + y) * (frame_ptr->stride_/2)) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->U_init_;
                        (*free_buffer)[((pos_y/2 + y) * (frame_ptr->stride_/2)) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->V_init_;
                    }
                }
            }

            {
                std::unique_lock<std::mutex> lock(write_mtx);
                write_queue.push(free_buffer);
            }
            write_cv.notify_one(); // After pushing the ready buffer into in the generate queue to generate the frame into the buffer
            iterator++;
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
            if (pos_x + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->stride_) {
                // -1 because in the draw loops we start from 0
                last_pos_x = pos_x + (cube_ptr->size_ - 1);
            }

            {
                std::unique_lock<std::mutex> lock(draw_mtx);
                draw_cv.wait(lock, [&]{ return !draw_queue.empty(); });
                free_buffer = std::move(draw_queue.front());
                draw_queue.pop();
            }

            // Fill background
            fill_background(free_buffer);

            if (direction == RIGHT){
                // right movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*free_buffer)[((pos_y - last_pos_y - y) * frame_ptr->stride_) + (pos_x + x)] = cube_ptr->Y_init_;
                    }
                }
                // // Chroma
                for (int y = 1; y <= cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        (*free_buffer)[((pos_y/2) - (last_pos_y/2) - y) * (frame_ptr->stride_/2) + ((pos_x/2) + x)] = cube_ptr->U_init_;
                        (*free_buffer)[((pos_y/2) - (last_pos_y/2) - y) * (frame_ptr->stride_/2) + ((pos_x/2) + x)] = cube_ptr->V_init_;
                    }
                }
            } else if (direction == LEFT) {
                // left movement
                // Luma
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*free_buffer)[((pos_y - last_pos_y -y) * frame_ptr->stride_) + (last_pos_x - (x + pos_x))] = cube_ptr->Y_init_;
                    }
                }
                // // Chroma
                for (int y = 1; y <= cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        (*free_buffer)[(pos_y/2 - last_pos_y/2 - y) * (frame_ptr->stride_/2) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->U_init_;
                        (*free_buffer)[(pos_y/2 - last_pos_y/2 - y) * (frame_ptr->stride_/2) + (last_pos_x/2 - (x + pos_x/2))] = cube_ptr->V_init_;
                    }
                }
            }

            {
                std::unique_lock<std::mutex> lock(write_mtx);
                write_queue.push(free_buffer);
            }
            write_cv.notify_one();
            iterator++;
        }
            cube_ptr->Y_init_ += cube_ptr->Y_inc_;
            cube_ptr->U_init_ += cube_ptr->U_inc_;
            cube_ptr->V_init_ += cube_ptr->V_inc_;
            direction = direction * (-1);
            iterator++;
    }
    finished = true;
    std::cout << "Drew iterations -> " << iterator << std::endl;
}



void Video_gen::write_file() {
    int iterator = 0;
    while (true) {
        std::shared_ptr<std::vector<uint8_t>> ready_buffer;
        {
            std::unique_lock<std::mutex> lock(write_mtx);
            write_cv.wait(lock, [&]{ return !write_queue.empty() ; });
            if (write_queue.empty()) {
                break;
            }
            ready_buffer = std::move(write_queue.front());
            write_queue.pop();
        }

        file_ptr << "FRAME\n";
        iterator++;
        file_ptr.write(reinterpret_cast<char*>(ready_buffer->data()), ready_buffer->size());

        {
            std::lock_guard<std::mutex> lk(draw_mtx);
            draw_queue.push(std::move(ready_buffer));
        }
        draw_cv.notify_one();

        if (finished == true) {
            break;
        }
    }
    std::cout << "Written frame -> " << iterator << std::endl;
}

Video_gen::~Video_gen() {

    finished = true;

    draw_cv.notify_all();

    write_cv.notify_all();

    if (draw_thread.joinable()) {
        draw_thread.join();
    }
    if (write_thread.joinable()) {
        write_thread.join();
    }
}
