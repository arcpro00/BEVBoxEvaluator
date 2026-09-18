#pragma once

#include "geometry.hpp"

namespace bev {

struct MetricsResult {
    float area_overlap;
    float height_overlap;
    float overlap_volume;
    float box1_volume;
    float box2_volume;
    float iou;
};

float compute_iou(float vol1, float vol2, float overlap_vol);

}