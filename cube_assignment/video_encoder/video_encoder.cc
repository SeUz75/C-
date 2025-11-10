#include <iostream>
#include "video_encoder.h"

Video_gen::Video_gen(std::string file_name, std::shared_ptr<Cube> cube, std::shared_ptr<Background> frame) {
            file_ptr.open(file_name, std::ios::out | std::ios::binary);
            std::cout << "Constructor called !" << std::endl;
            if (!file_ptr.is_open()) {
                std::cout << "Could not open file exiting program.... " << std::endl;
                return;
            }
            finished = false;
            cube_ptr = cube;
            frame_ptr = frame;
            total_size = (frame_ptr->stride_ * frame_ptr->height_) * 3 / 2;
            buffer_a = std::make_shared<std::vector<uint8_t>>(total_size);
            buffer_c = std::make_shared<std::vector<uint8_t>>(total_size);
            buffer_b = std::make_shared<std::vector<uint8_t>>(total_size);
            // W thread will cond wait for the buffers to be pushed into the queue
            draw_thread = std::thread(&Video_gen::draw_frame, this);

            // Inside the w thread func we will wake up the g_thread to ge
            write_thread = std::thread(&Video_gen::write_file, this);

            drawn_frames = 0;
            written_frames = 0;
}

void Video_gen::init (){
    std::cout << "3 pointers poiting to vector created ! " << std::endl;
    {
        std::unique_lock<std::mutex> lock(draw_mtx);
        draw_queue.push(buffer_a);
        draw_queue.push(buffer_b);
        draw_queue.push(buffer_c);
        std::cout << "Pushed all pointer vectors into the queue  !" << std::endl;
    }
    std::cout << "Notifying draw condition...." << std::endl;
    draw_cv.notify_one();
}



void Video_gen::run () {
    init();
    draw_thread.join();

    {
        std::unique_lock<std::mutex> lock(write_mtx);
        finished = true;
    }
    write_cv.notify_one();

    write_thread.join();
}


void Video_gen::draw_frame() {

    file_ptr << "YUV4MPEG2 W" << frame_ptr->stride_ << " H" << frame_ptr->height_ << " F" << frame_ptr->frame_rate_ << ":1 C420\n";

    // UPWARD MOVEMENT ---------------------------------------------------
    int last_pos_x = 0;
    int last_pos_y = 0;

    int y_size = frame_ptr->stride_ * frame_ptr->height_;
    int u_size = frame_ptr->stride_ * frame_ptr->height_ / 4;

    std::shared_ptr<std::vector<uint8_t>> buffer;


      // Lambda function to fill the background of the image
    auto fill_background = [&] (std::shared_ptr<std::vector<uint8_t>> buffer) {
        std::fill(buffer->begin(), buffer->begin() + y_size, frame_ptr->Y_init_);
        std::fill(buffer->begin() + y_size, buffer->begin() + y_size + u_size, frame_ptr->U_init_);
        std::fill(buffer->begin() + y_size + u_size, buffer->end(), frame_ptr->V_init_);
    };

    int direction = RIGHT;

    for (int pos_y = 0; pos_y + cube_ptr->size_ < frame_ptr->height_; pos_y += cube_ptr->step_) {
        if (pos_y + cube_ptr->size_ + cube_ptr->step_ > frame_ptr->height_ ) {
            last_pos_y = pos_y + (cube_ptr->size_ - 1);
        }

        for (int pos_x = 0; pos_x + cube_ptr->size_ < frame_ptr->stride_; pos_x += cube_ptr->step_) {
            // Waiting till I have available buffers
            {
                std::unique_lock<std::mutex> lock(draw_mtx);
                draw_cv.wait(lock, [&]() { return !draw_queue.empty() || finished; });

                if (finished && draw_queue.empty()) {
                    return;
                }
                buffer = draw_queue.front();
                draw_queue.pop();
            }
            fill_background(buffer);


            if (direction == RIGHT) {
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*buffer)[((pos_y + y) * frame_ptr->stride_) + (pos_x + x)] = cube_ptr->Y_init_;
                    }
                }

                // Chroma (U and V planes) - YUV420 is subsampled 2x2
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        int uv_index = ((pos_y/2 + y) * (frame_ptr->stride_/2)) + (pos_x/2 + x);
                        (*buffer)[y_size + uv_index] = cube_ptr->U_init_;                      // U plane
                        (*buffer)[y_size + u_size + uv_index] = cube_ptr->V_init_;             // V plane
                    }
                }

            } else if (direction == LEFT) {
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*buffer)[((pos_y + y) * frame_ptr->stride_) + (last_pos_x - (x + pos_x))] = cube_ptr->Y_init_;
                    }
                }

                // Chroma (U and V planes)
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        int uv_index = ((pos_y/2 + y) * (frame_ptr->stride_/2)) + (last_pos_x/2 - (x + pos_x/2));
                        (*buffer)[y_size + uv_index] = cube_ptr->U_init_;                      // U plane
                        (*buffer)[y_size + u_size + uv_index] = cube_ptr->V_init_;             // V plane
                    }
                }
            }
            // Notifying that a buffer is ready to be written on the file
            {
                std::unique_lock<std::mutex> lock(write_mtx);
                write_queue.push(buffer);
            }

            if (pos_x + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->stride_) {
                last_pos_x = pos_x + (cube_ptr->size_ - 1);
            }
            write_cv.notify_one();
            drawn_frames++;
            // file_ptr << "FRAME\n";
            // file_ptr.write(reinterpret_cast<char*>(buffer->data()), buffer->size());
        }
        if ( pos_y + cube_ptr->size_ + cube_ptr->step_ > frame_ptr->height_ ) {
            cube_ptr->Y_init_ = cube_ptr->Y_init_ + (2 * cube_ptr->Y_inc_);
            cube_ptr->U_init_ = cube_ptr->U_init_ + (2 * cube_ptr->U_inc_);
            cube_ptr->V_init_ = cube_ptr->V_init_ + (2 * cube_ptr->V_inc_);
        } else {
            cube_ptr->Y_init_ = cube_ptr->Y_init_ + cube_ptr->Y_inc_;
            cube_ptr->U_init_ = cube_ptr->U_init_ + cube_ptr->U_inc_;
            cube_ptr->V_init_ = cube_ptr->V_init_ + cube_ptr->V_inc_;
        }
        direction *= -1;
    }

    // Upward movement !
    for (int pos_y = 0; pos_y + cube_ptr->size_ < frame_ptr->height_; pos_y += cube_ptr->step_) {
        for (int pos_x = 0; pos_x + cube_ptr->size_ < frame_ptr->stride_; pos_x += cube_ptr->step_) {

            // Waiting till I have available buffers
            {
                std::unique_lock<std::mutex> lock(draw_mtx);
                draw_cv.wait(lock, [&]() { return !draw_queue.empty() || finished; });

                if (finished && draw_queue.empty()) {
                    return;
                }

                buffer = draw_queue.front();
                draw_queue.pop();
            }

            fill_background(buffer);


            if (direction == RIGHT) {
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*buffer)[((last_pos_y - (pos_y + y)) * frame_ptr->stride_) + (pos_x + x)] = cube_ptr->Y_init_;
                    }
                }

                // Chroma (U and V planes) - CORRECTED
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        int uv_index = ((last_pos_y/2 - (pos_y/2 + y)) * (frame_ptr->stride_/2)) + (pos_x/2 + x);
                        (*buffer)[y_size + uv_index] = cube_ptr->U_init_;
                        (*buffer)[y_size + u_size + uv_index] = cube_ptr->V_init_;
                    }
                }

            } else if (direction == LEFT) {
                for (int y = 0; y < cube_ptr->size_; y++) {
                    for (int x = 0; x < cube_ptr->size_; x++) {
                        (*buffer)[((last_pos_y - (pos_y + y)) * frame_ptr->stride_) + (last_pos_x - (x + pos_x))] = cube_ptr->Y_init_;
                    }
                }

                // Chroma (U and V planes) - CORRECTED
                for (int y = 0; y < cube_ptr->size_/2; y++) {
                    for (int x = 0; x < cube_ptr->size_/2; x++) {
                        int uv_index = ((last_pos_y/2 - (pos_y/2 + y)) * (frame_ptr->stride_/2)) + (last_pos_x/2 - (x + pos_x/2));
                        (*buffer)[y_size + uv_index] = cube_ptr->U_init_;
                        (*buffer)[y_size + u_size + uv_index] = cube_ptr->V_init_;
                    }
                }
            }

            {
                std::unique_lock<std::mutex> lock(write_mtx);
                write_queue.push(buffer);
            }
            drawn_frames++;
            if (pos_x + cube_ptr->size_ + cube_ptr->step_ >= frame_ptr->stride_) {
                last_pos_x = pos_x + (cube_ptr->size_ - 1);
            }
            write_cv.notify_one();
            // file_ptr << "FRAME\n";
            // file_ptr.write(reinterpret_cast<char*>(buffer->data()), buffer->size());
        }
        cube_ptr->Y_init_ = cube_ptr->Y_init_ + cube_ptr->Y_inc_;
        cube_ptr->U_init_ = cube_ptr->U_init_ + cube_ptr->U_inc_;
        cube_ptr->V_init_ = cube_ptr->V_init_ + cube_ptr->V_inc_;
        direction *= -1;
    }
}


void Video_gen::write_file() {

    while (true) {
        std::shared_ptr<std::vector<uint8_t>> buffer;
        {
            std::unique_lock<std::mutex> lock(write_mtx);
            write_cv.wait(lock, [&]() { return !write_queue.empty() || finished; });

            // If finished and no more buffers to write, exit
            if (finished && write_queue.empty()) {
                std::cout << "Write thread finishing..." << std::endl;
                return;
            }

            // If we have a buffer, process it
            if (!write_queue.empty()) {
                buffer = write_queue.front();
                write_queue.pop();
            } else {
                continue;  // Spurious wakeup, go back to waiting
            }
        }

        // Write to file (outside the lock!)
        file_ptr << "FRAME\n";
        file_ptr.write(reinterpret_cast<char*>(buffer->data()), buffer->size());
        written_frames++;

        // Return buffer to draw queue
        {
            std::unique_lock<std::mutex> lock(draw_mtx);
            draw_queue.push(buffer);
        }
        draw_cv.notify_one();
    }
}


    Video_gen::~Video_gen() {
            // Signal threads to finish
            {
                std::unique_lock<std::mutex> lock(draw_mtx);
                finished = true;
            }
            draw_cv.notify_all();
            write_cv.notify_all();

            // Wait for threads to complete
            if (draw_thread.joinable()) {
                draw_thread.join();
            }
            if (write_thread.joinable()) {
                write_thread.join();
            }

            // Close file
            if (file_ptr.is_open()) {
                file_ptr.close();
            }

            std::cout << "Generator destroyed, threads joined" << std::endl;
            std::cout << "Drawn frames are -> " << drawn_frames << std::endl;
            std::cout << "Written frames are -> " << written_frames << std::endl;
    }
