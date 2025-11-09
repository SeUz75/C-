#pragma once

#include <vector>
#include <stdint.h>
#include <thread>
#include <condition_variable>
#include <bits/stdc++.h>
#include <queue>

#include "cube.h"
#include "background.h"


#define RIGHT 1
#define LEFT -1

class Video_gen {
    public :
        Video_gen(std::string file_name, std::shared_ptr<Cube> cube, std::shared_ptr<Background> frame);
        void run();
        ~Video_gen();

    private :
        void init();
        int drawn_frames;
        int written_frames;
        void draw_frame();
        void write_file();
        std::shared_ptr<Cube> cube_ptr;
        std::shared_ptr<Background> frame_ptr;
        std::ofstream file_ptr;
        std::queue<std::shared_ptr<std::vector<uint8_t>>> draw_queue;
        std::queue<std::shared_ptr<std::vector<uint8_t>>> write_queue;
        std::shared_ptr<std::vector<uint8_t>> buffer_a;
        std::shared_ptr<std::vector<uint8_t>> buffer_b;
        std::shared_ptr<std::vector<uint8_t>> buffer_c;
        int total_size;
        std::thread draw_thread;
        std::thread write_thread;
        std::mutex draw_mtx;
        std::mutex write_mtx;
        std::condition_variable draw_cv;
        std::condition_variable write_cv;
        bool finished;
};