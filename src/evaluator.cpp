#include "evaluator.hpp"
#include <array>
#include <iostream>

namespace bev {

std::array<float,4> get_box_borders(const Box3D& box){
    // Format: Left, Right, Bottom, Top
    std::array<float, 4> borders;
    borders[0] = box.x;
    borders[1] = box.x + box.w;
    borders[2] = box.y;
    borders[3] = box.y + box.h;
    return borders;
}

float get_area_overlap(const Box3D& box1, const Box3D& box2) {
    // overlap area of the two BEV projections
    std::array<float,4> borders1 = get_box_borders(box1);
    std::array<float,4> borders2 = get_box_borders(box2);
    if (borders1[1] < borders2[0] || borders1[0] > borders2[1]){
        return 0.0f;
    }
    if (borders1[3] < borders2[2] || borders1[2] > borders2[3]){
        return 0.0f;
    }
    //otherwise calculate bounds and compute a area 
    float l; float r; float b; float t;
    if (borders1[0] <= borders2[0]){
        l = borders2[0];
    }
    else{
        l = borders1[0];
    }
    if (borders1[2] <= borders2[2]){
        b = borders2[2];
    }
    else{
        b = borders1[2];
    }
    if (borders1[1] >= borders2[1]){
        r = borders2[1];
    }
    else{
        r = borders1[1];
    }
    if (borders1[3] <= borders2[3]){
        t = borders2[3];
    }
    else{
        t = borders1[3];
    }
    float area;
    area = (r-l)*(t-b);
    return area;
}

float get_height_overlap(const Box3D& box1, const Box3D& box2) {
    // vertical overlap range at the overlapping region
    return 0.0f;
}

float get_overlap_volume(const Box3D& box1, const Box3D& box2) {
    // area overlap times height overlap
    return 0.0f;
}

float get_individual_volume(const Box3D& box) {
    // l * w * h
    return 0.0f;
}

MetricsResult get_pair_metrics(const Box3D& box1, const Box3D& box2) {
    MetricsResult result{};
    // fill area/height/volume overlaps, individual volumes, and IoU
    return result;
}

float compute_iou(float vol1, float vol2, float overlap_vol) {
    // overlap_vol / (vol1 + vol2 - overlap_vol); guard division by zero
    return 0.0f;
}

}