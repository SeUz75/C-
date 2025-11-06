#pragma once

#include <vector>
#include <stdint.h>
#include <thread>
#include <condition_variable>
#include <bits/stdc++.h>

#include "cube.h"
#include "background.h"


#define RIGHT 1
#define LEFT -1

class Video_gen {
    public :
        Video_gen(std::string file_name, std::shared_ptr<Cube> cube, std::shared_ptr<Background> frame);
        ~Video_gen();

    private :
        void write_frame();
        void write_file();
        std::shared_ptr<Cube> cube_ptr;
        std::shared_ptr<Background> frame_ptr;
        std::ofstream file_ptr;
        std::queue<std::vector<uint8_t>> queue_frame;
        std::thread w_thread;
        std::thread g_thread;
        std::mutex mtx;
        std::condition_variable cv;
        bool ready_to_right;
};