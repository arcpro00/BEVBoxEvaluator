#include "geometry.hpp"
#include <cmath>
#include <numbers>

namespace bev {

float normalize_yaw(float yaw) {
    // wrap yaw into the valid range from 0 to pi
    float pi = 3.1415927f;
    float valid_yaw = std::fmod(yaw,2*pi);
    if (valid_yaw >= pi){
        valid_yaw = valid_yaw-2*pi;
    }
    if (valid_yaw < -pi){
        valid_yaw = valid_yaw+2*pi;
    }
    if (valid_yaw < 0){
        valid_yaw = valid_yaw+pi;
    }
    return valid_yaw;
}

}