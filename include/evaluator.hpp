#pragma once

#include "geometry.hpp"
#include "metrics.hpp"

namespace bev {

// Get the overlap: find overlap of the 2d projections of boxes, find the range of height at overlapping regions and multiply
float get_area_overlap(const Box3D& box1, const Box3D& box2);
float get_height_overlap(const Box3D& box1, const Box3D& box2);
float get_overlap_volume(const Box3D& box1, const Box3D& box2);
float get_individual_volume(const Box3D& box);

MetricsResult get_pair_metrics(const Box3D& box1, const Box3D& box2);

}