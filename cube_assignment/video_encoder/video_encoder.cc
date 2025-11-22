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
            square_ptr = cube;
            frame_ptr = frame;
            total_size = (frame_ptr->width_ * frame_ptr->height_) * 3 / 2;
            buffer_a = std::make_shared<std::vector<uint8_t>>(total_size);
            buffer_c = std::make_shared<std::vector<uint8_t>>(total_size);
            buffer_b = std::make_shared<std::vector<uint8_t>>(total_size);
            // W thread will cond wait for the buffers to be pushed into the queue
            draw_thread = std::thread(&Video_gen::ThreadDraw, this);

            // Inside the w thread func we will wake up the g_thread to ge
            write_thread = std::thread(&Video_gen::ThreadWrite, this);

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


void Video_gen::ThreadDraw() {
    file_ptr << "YUV4MPEG2 W" << frame_ptr->width_
                << " H" << frame_ptr->height_
                << " F" << frame_ptr->fps
                << ":1 C420\n";

    std::shared_ptr<std::vector<uint8_t>> currentFrame;

    // downwards
    do {
        if (square_ptr->pos_x == 0) {
            // Right direction
            do {
                // Waiting till I have available buffers
                {
                    std::unique_lock<std::mutex> lock(draw_mtx);
                    draw_cv.wait(lock, [&]() {
                        return !draw_queue.empty() || finished;
                    });

                    if (finished && draw_queue.empty()) {
                        return;
                    }
                    currentFrame = draw_queue.front();
                    draw_queue.pop();
                }

                frame_ptr->FillBackGround(currentFrame);
                frame_ptr->DrawSquare(currentFrame, square_ptr);

                // Notifying that a buffer is ready to be written on the file
                {
                    std::unique_lock<std::mutex> lock(write_mtx);
                    write_queue.push(currentFrame);
                }

                write_cv.notify_one();
                square_ptr->pos_x += square_ptr->step_;

            } while (square_ptr->pos_x + square_ptr->width_ <= frame_ptr->width_);
        } else if (square_ptr->pos_x + square_ptr->width_ >= frame_ptr->width_) {
            // Left direction
            do {
                // Waiting till I have available buffers
                {
                    std::unique_lock<std::mutex> lock(draw_mtx);
                    draw_cv.wait(lock, [&]() {
                        return !draw_queue.empty() || finished;
                    });

                    if (finished && draw_queue.empty()) {
                        return;
                    }
                    currentFrame = draw_queue.front();
                    draw_queue.pop();
                }

                square_ptr->pos_x -= square_ptr->step_;

                frame_ptr->FillBackGround(currentFrame);
                frame_ptr->DrawSquare(currentFrame, square_ptr);

                // Notifying that a buffer is ready to be written on the file
                {
                    std::unique_lock<std::mutex> lock(write_mtx);
                    write_queue.push(currentFrame);
                }
                write_cv.notify_one();

            } while (square_ptr->pos_x > 0);
        }

        square_ptr->pos_y += square_ptr->step_;  // step of cube which goes down
        square_ptr->y_ += square_ptr->Y_inc_;
        square_ptr->u_ += square_ptr->U_inc_;
        square_ptr->v_ += square_ptr->V_inc_;

    } while (square_ptr->pos_y + square_ptr->width_ <= frame_ptr->height_);

    square_ptr->y_ += square_ptr->Y_inc_;
    square_ptr->u_ += square_ptr->U_inc_;
    square_ptr->v_ += square_ptr->V_inc_;

    // Upwards
    do {
        square_ptr->pos_y -= square_ptr->step_;
        if (square_ptr->pos_x == 0) {
            // Right direction
            do {
                // Waiting till I have available buffers
                {
                    std::unique_lock<std::mutex> lock(draw_mtx);
                    draw_cv.wait(lock, [&]() {
                        return !draw_queue.empty() || finished;
                    });

                    if (finished && draw_queue.empty()) {
                        return;
                    }
                    currentFrame = draw_queue.front();
                    draw_queue.pop();
                }

                frame_ptr->FillBackGround(currentFrame);
                frame_ptr->DrawSquare(currentFrame, square_ptr);

                // Notifying that a buffer is ready to be written on the file
                {
                    std::unique_lock<std::mutex> lock(write_mtx);
                    write_queue.push(currentFrame);
                }
                write_cv.notify_one();

                square_ptr->pos_x += square_ptr->step_;
            } while (square_ptr->pos_x + square_ptr->width_ <= frame_ptr->width_);
        } else if (square_ptr->pos_x + square_ptr->width_ >= frame_ptr->width_) {
            // Left direction
            do {
                // Waiting till I have available buffers
                {
                    std::unique_lock<std::mutex> lock(draw_mtx);
                    draw_cv.wait(lock, [&]() {
                        return !draw_queue.empty() || finished;
                    });

                    if (finished && draw_queue.empty()) {
                        return;
                    }
                    currentFrame = draw_queue.front();
                    draw_queue.pop();
                }

                square_ptr->pos_x -= square_ptr->step_;

                frame_ptr->FillBackGround(currentFrame);
                frame_ptr->DrawSquare(currentFrame, square_ptr);

                // Notifying that a buffer is ready to be written on the file
                {
                    std::unique_lock<std::mutex> lock(write_mtx);
                    write_queue.push(currentFrame);
                }
                write_cv.notify_one();

            } while (square_ptr->pos_x > 0);
        }
        square_ptr->y_ += square_ptr->Y_inc_;
        square_ptr->u_ += square_ptr->U_inc_;
        square_ptr->v_ += square_ptr->V_inc_;

    } while (square_ptr->pos_y > 0);
}


void Video_gen::ThreadWrite() {

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
