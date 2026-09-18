#include "evaluator.hpp"

namespace bev {

float get_area_overlap(const Box3D& box1, const Box3D& box2) {
    // overlap area of the two BEV projections
    return 0.0f;
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