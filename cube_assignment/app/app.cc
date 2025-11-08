#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <unistd.h>

#include "utilities.h"
#include "cube.h"
#include "background.h"
#include "video_encoder.h"

#define LEFT -1
#define RIGHT 1

int main (int argc, char* argv[]) {
    std::cout << "Hello world \n";

    if (argc != 31) {
        std::cerr << "Enter valid number of arguments" << std::endl;
        return -1;
    }

    Utilities utilities;

    std::string output_file;
    int w = 0, h = 0, f = 0, s = 0, S = 0, temp = 0;
    uint8_t y = 0, u = 0, v = 0, Y = 0, U = 0, V = 0, a = 0, b = 0, c = 0;

    char* possible_char;
    int opt;
    while ((opt = getopt(argc, argv, "o:w:h:f:s:S:y:u:v:Y:U:V:a:b:c:")) != -1){
        switch (opt)
        {
        case 'o':
            output_file = optarg;
            break;
        case 'w':
            w = std::strtoul(optarg, &possible_char, 10);
            if (*possible_char != '\0') {
                std::cerr << "Typo mistake, entered char in width number"
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'h':
            h = std::strtoul(optarg, &possible_char, 10);
            if (*possible_char != '\0') {
                std::cerr << "Typo mistake, entered char in height number"
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'f':
            f = std::strtoul(optarg, &possible_char, 10);
            if ((f < 0) || (*possible_char != '\0')) {
                std::cerr << "Typo mistake, entered char in fps number"
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 's':
            s = std::strtoul(optarg, &possible_char, 10);
            if ((s%2 != 0) || (s > w) || (*possible_char != '\0')) {
                std::cerr << "Mistake at square size, must be even,"
                             "and fit in the resolution "<< std::endl;
                return -1;
            }
            break;
        case 'S':
            S = std::strtoul(optarg, &possible_char, 10);
            if ((S%2 != 0) || (S > w) || (*possible_char != '\0')) {
                std::cerr << "Mistake at square steps, must be even, "
                             "and fit in the resolution "<< std::endl;
                return -1;
            }
            break;
        case 'y':
            temp = std::strtoul(optarg, &possible_char, 10);
            y = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number y value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'u':
            temp = std::strtoul(optarg, &possible_char, 10);
            u = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number u value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'v':
            temp = std::strtoul(optarg, &possible_char, 10);
            v = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number v value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'Y':
            temp = std::strtoul(optarg, &possible_char, 10);
            Y = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number Y value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'U':
            temp = std::strtoul(optarg, &possible_char, 10);
            U = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number U value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'V':
            temp = std::strtoul(optarg, &possible_char, 10);
            V = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number V value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'a':
            temp = std::strtoul(optarg, &possible_char, 10);
            a = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number a value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'b':
            temp = std::strtoul(optarg, &possible_char, 10);
            b = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number b value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'c':
            temp = std::strtoul(optarg, &possible_char, 10);
            c = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number c value "
                          << *possible_char << std::endl;
                return -1;
            }
            break;
        case '?' :
            std::cerr << "Uknown flag passed, exiting program..." << std::endl;
            return -1;
            break;
        default:
            break;
        }
    }

    if ( !((w == 1280 && h == 720) || (w == 1632 && h == 1224) ||
            (w == 1920 && h == 1080)) ) {
        std::cerr << "Please pass one of these pairs of resolutions "
                  <<std::endl << "1280 x 720" << std::endl
                  << "1632 x 1224" << std::endl << "1920 x 1080" << std::endl;
        return -1;
    }

    // shared pointers for cube, background
    auto frame_ptr = std::make_shared<Background>(h, w, f, y, u, v);
    auto cube_ptr = std::make_shared<Cube>(s, S, Y, U, V, a, b, c);

    try {
        auto gen_ptr = std::make_shared<Video_gen>(output_file, cube_ptr, frame_ptr);
        // let destructor join threads / flush output when gen_ptr goes out of scope
    } catch (const std::exception& ex) {
        std::cerr << "Exception constructing Video_gen: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
