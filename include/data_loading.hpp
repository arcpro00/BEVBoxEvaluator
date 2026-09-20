#pragma once

#include "geometry.hpp"
#include <string>
#include <vector>

namespace bev {

// Reads one row of input and outputs a Box3D
Box3D parse_box(const std::string& line);

// Reads all box rows from a modified KITTI-style file into individual pairs
std::vector<BoxPair> load_box_pairs(const std::string& filepath);

}