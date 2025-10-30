#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include "utilities.h"

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
                std::cerr << "Typo mistake, entered char in width number" << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'h':
            h = std::strtoul(optarg, &possible_char, 10);
            if (*possible_char != '\0') {
                std::cerr << "Typo mistake, entered char in height number" << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'f':
            f = std::strtoul(optarg, &possible_char, 10);
            if ((f < 0) || (*possible_char != '\0')) {
                std::cerr << "Typo mistake, entered char in fps number" << *possible_char << std::endl;
                return -1;
            }
            break;
        case 's':
            s = std::strtoul(optarg, &possible_char, 10);
            if ((s%2 != 0) || (s > w) || (*possible_char != '\0')) {
                std::cerr << "Mistake at square size, must be even, and fit in the resolution "<< std::endl;
                return -1;
            }
            break;
        case 'S':
            S = std::strtoul(optarg, &possible_char, 10);
            if ((S%2 != 0) || (S > w) || (*possible_char != '\0')) {
                std::cerr << "Mistake at square steps, must be even, and fit in the resolution "<< std::endl;
                return -1;
            }
            break;
        case 'y':
            temp = std::strtoul(optarg, &possible_char, 10);
            y = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number y value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'u':
            temp = std::strtoul(optarg, &possible_char, 10);
            u = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number u value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'v':
            temp = std::strtoul(optarg, &possible_char, 10);
            v = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number v value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'Y':
            temp = std::strtoul(optarg, &possible_char, 10);
            Y = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number Y value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'U':
            temp = std::strtoul(optarg, &possible_char, 10);
            U = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number U value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'V':
            temp = std::strtoul(optarg, &possible_char, 10);
            V = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number V value " << *possible_char << std::endl;
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
                std::cerr << "Miss typed char in number b value " << *possible_char << std::endl;
                return -1;
            }
            break;
        case 'c':
            temp = std::strtoul(optarg, &possible_char, 10);
            c = utilities.Clamping_YUV(temp);
            if (*possible_char != '\0') {
                std::cerr << "Miss typed char in number c value " << *possible_char << std::endl;
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
    
    if ( !((w == 1280 && h == 720) || (w == 1632 && h == 1224) || (w == 1920 && h == 1080)) ) {
        std::cerr << "Please pass one of these pairs of resolutions " <<std::endl << "1280 x 720" << std::endl <<
        "1632 x 1224" << std::endl << "1920 x 1080" << std::endl;
        return -1;
    }
    std::cout << (int)y << std::endl;
    std::cout << (int)u << std::endl;
    std::cout << (int)v << std::endl;
    // Minimal Y4M header must include
    // YUV4MPEG2 W<width> H<height> F<fps_num>:<fps_den> Ip A0:0 C<chroma>
    std::ofstream video_buffer(output_file, std::ios::out | std::ios::binary);
    video_buffer << "YUV4MPEG2 W" << w << " H" << h << " F" << f << ":1 C420\n";
    
    // // Filling background
    // std::vector<uint8_t> Y_buffer(w*h, y);
    // std::vector<uint8_t> U_buffer((w*h)/4, u);
    // std::vector<uint8_t> V_buffer((w*h)/4, v);


    // // Generating a cube 
    // for (size_t y = 0; y < s; y++) {
    //     for (size_t x = 0; x < s; x++) {
    //         Y_buffer[(y * w) + x] = Y;
    //     }
    // }


    // for (size_t y = 0; y < s/2; y++) {
    //     for (size_t x = 0; x < s/2; x++) {
    //         U_buffer[(y * (w/2)) + x] = U;
    //         V_buffer[(y * (w/2)) + x] = V;
    //     }
    // }

    
    for (size_t pos_y = 0; pos_y + s < h; pos_y+=S) {
        for (size_t pos_x = 0; pos_x + s < w; pos_x+=S) {
            std::vector<uint8_t> Y_buffer(w*h, y);
            std::vector<uint8_t> U_buffer((w*h)/4, u);
            std::vector<uint8_t> V_buffer((w*h)/4, v);

            for (size_t y = 0; y < s; y++) {
                for (size_t x = 0; x < s; x++) {
                    size_t y_index = (pos_y * + y) * w + (pos_x + x);
                    Y_buffer[y_index] = Y;
                }
            }


            // Draw corresponding UV area (subsampled)
            size_t pos_y_uv = pos_y / 2;
            size_t pos_x_uv = pos_x / 2;
            size_t s_uv = s / 2;

            for (size_t j = 0; j < s_uv; j++) {
                for (size_t i = 0; i < s_uv; i++) {
                    size_t uv_index = (pos_y_uv + j) * (w/2) + (pos_x_uv + i);
                    U_buffer[uv_index] = U;
                    V_buffer[uv_index] = V;
                }
            }


            video_buffer  << "FRAME\n";
            video_buffer.write(reinterpret_cast<char*>(Y_buffer.data()), Y_buffer.size());
            video_buffer.write(reinterpret_cast<char*>(U_buffer.data()), U_buffer.size());
            video_buffer.write(reinterpret_cast<char*>(V_buffer.data()), V_buffer.size());
        }
    }


    return 0;
}