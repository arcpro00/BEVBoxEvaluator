#pragma once

namespace bev {

struct Box3D {
    int pair_id;
    float x;
    float y;
    float z;
    float l;
    float w;
    float h;
    float yaw;
};

struct BoxPair {
    int pair_id;
    Box3D first;
    Box3D second;
};

float normalize_yaw(float yaw);

}