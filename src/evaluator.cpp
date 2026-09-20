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
    if (borders1[3] >= borders2[3]){
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
    float bottom1 = box1.z;
    float top1 = box1.z + box1.h;
    float bottom2 = box2.z;
    float top2 = box2.z + box2.h;
    if (top1 <= bottom2 || bottom1 >= top2){
        return 0.0f;
    }
    float bottom;
    float top;
    if (bottom1 >= bottom2){
        bottom = bottom1;
    }
    else{
        bottom = bottom2;
    }
    if (top1 <= top2){
        top = top1;
    }
    else{
        top = top2;
    }
    return top - bottom;
}

float get_overlap_volume(const Box3D& box1, const Box3D& box2) {
    // area overlap times height overlap
    float area = get_area_overlap(box1, box2);
    float height = get_height_overlap(box1, box2);
    return area*height;
}

float get_individual_volume(const Box3D& box) {
    // l * w * h
    return box.l*box.w*box.h;
}

MetricsResult get_pair_metrics(const Box3D& box1, const Box3D& box2) {
    MetricsResult result{};
    // fill area/height/volume overlaps, individual volumes, and IoU
    result.area_overlap = get_area_overlap(box1, box2);
    result.height_overlap = get_height_overlap(box1, box2);
    result.overlap_volume = get_overlap_volume(box1, box2);
    result.box1_volume = get_individual_volume(box1);
    result.box2_volume = get_individual_volume(box2);
    result.iou = compute_iou(
        result.box1_volume,
        result.box2_volume,
        result.overlap_volume
    );
    return result;
    return result;
}

float compute_iou(float vol1, float vol2, float overlap_vol) {
    // overlap_vol / (vol1 + vol2 - overlap_vol); guard division by zero
    float union_vol = vol1 + vol2 - overlap_vol;
    if (union_vol <= 0.0f){
        return 0.0f;
    }
    return overlap_vol / union_vol;
}

}