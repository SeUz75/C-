#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <unistd.h>

#include "utilities.h"
#include "cube.h"
#include "background.h"

int main (int argc, char* argv[]) {
    std::cout << "Hello world \n";

    if (argc != 31) {
        std::cerr << "Enter valid number of arguments" << std::endl;
        return -1;
    }

    Utilities utilities;

    std::string output_file;
    int w,h,f,s,S, temp;
    uint8_t y,u,v,Y,U,V,a,b,c;

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

    // Minimal Y4M header must include
    // YUV4MPEG2 W<width> H<height> F<fps_num>:<fps_den> Ip A0:0 C<chroma>
    std::ofstream video_buffer(output_file, std::ios::out | std::ios::binary);
    video_buffer << "YUV4MPEG2 W" << w << " H" << h << " F" << f << ":1 C420\n";
    video_buffer << "FRAME\n";

    // shared pointers for cube, background
    auto frame_ptr = std::make_shared<Background>(Background(h,w,y,u,v));
    auto cube_ptr = std::make_shared<Cube>(Cube(s, S, Y, U, V));

    // std::cout << "Size of frame height -> " << frame_ptr->height << " sizeo of"
    //             "width " << frame_ptr->stride << " Size of Y's "
    //           << frame_ptr->Y_init.size() <<" value -> " << static_cast<int>(frame_ptr->Y_init[1])
    //           << std::endl << "size of u -> " << frame_ptr->U_init.size()
    //           << " value of u -> " << static_cast<int>(frame_ptr->U_init[1]) << std::endl
    //           << "size of v -> " << frame_ptr->V_init.size()
    //           << "value of v -> " << static_cast<int>(frame_ptr->U_init[1]) << std::endl;


    return 0;
}
