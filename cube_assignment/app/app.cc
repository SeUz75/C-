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

    // std::cout << " y = " << static_cast<int>(y) <<std::endl;
    // std::cout << " u = " << static_cast<int>(u) <<std::endl;
    // std::cout << " v = " << static_cast<int>(v) <<std::endl;
    // std::cout << " Y = " << static_cast<int>(Y) <<std::endl;
    // std::cout << " U = " << static_cast<int>(U) <<std::endl;
    // std::cout << " V = " << static_cast<int>(V) <<std::endl;
    // std::cout << " a = " << static_cast<int>(a) <<std::endl;
    // std::cout << " b = " << static_cast<int>(b) <<std::endl;
    // std::cout << " c = " << static_cast<int>(c) <<std::endl;
    std::ofstream video_buffer(output_file, std::ios::out | std::ios::binary);

    video_buffer << "YUVMPEG2 W " << (int)w << " H" << (int)h << " F" <<f <<"Ip A0:0 C420\nFRAME\n";

    std::vector<uint8_t> Y_value(w * h);
    std::vector<uint8_t> U_value(w/2 * h/2);
    std::vector<uint8_t> V_value(w/2 * h/2);

    for ( int i = 0; i < f; i++) {
        video_buffer << "FRAME\n";

        for ( int y = 0; y < h; y++) {
            for ( int x = 0; x < w; x++) {
                Y_value[y*w + x] = (x + f*2) % 256;
            }
            std::fill(U_value.begin(), U_value.end(), 128);
            std::fill(V_value.begin(), V_value.end(), 128);

            video_buffer.write((char*)Y_value.data(), Y_value.size());
            video_buffer.write((char*)U_value.data(), U_value.size());
            video_buffer.write((char*)V_value.data(), V_value.size());
        }
    }
    return 0;
}