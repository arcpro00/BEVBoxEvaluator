#pragma once

#include "geometry.hpp"
#include <string>

namespace bev {

// Reads the box pair input file -> outputs the Box3D
Box3D obtain_box_data(const std::string& filepath);

}