#pragma once

#include "geometry.hpp"
#include <string>
#include <vector>

namespace bev {

// Reads a single Box3D from the box pair input file
Box3D obtain_box_data(const std::string& filepath);

// Reads all box rows from a modified KITTI-style file into individual boxes
std::vector<Box3D> load_boxes(const std::string& filepath);

}