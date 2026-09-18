#include "data_loading.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace bev {

Box3D obtain_box_data(const std::string& filepath) {
    // parse a single modified KITTI-style box record from the file
    return Box3D{};
}

std::vector<Box3D> load_boxes(const std::string& filepath) {
    // parse header + all rows into individual boxes; throw std::runtime_error on failure
    return {};
}

}