#include "utilities.h"


uint8_t Utilities::Clamping_YUV(int value) {
    if (value > 255){
        return 255;
    } else if (value < 0){
        return 0;
    } else {
        return static_cast<uint8_t>(value);
    }
}