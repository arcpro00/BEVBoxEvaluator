#include "data_loading.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <string>

namespace bev {

Box3D parse_box(const std::string& line){
    std::istringstream stream(line);
    Box3D box{};
    if (!(stream >> box.pair_id >> box.x >> box.y >> box.z >> box.l >> box.w >> box.h >> box.yaw)){
        throw std::runtime_error("Invalid box data row");
    }
    return box;
}

std::vector<BoxPair> load_box_pairs(const std::string& filepath){
    // parse header + all rows into individual pairs; throw std::runtime_error on failure
    std::ifstream file(filepath);
    if (!file.is_open()){
        throw std::runtime_error("Could not open file: "+filepath);
    }
    std::unordered_map<int, std::vector<Box3D>> grouped;
    std::string line;
    // Skip header
    std::getline(file, line);
    while (std::getline(file, line)){
        if (line.empty()){
            continue;
        }
        Box3D box = parse_box(line);
        grouped[box.pair_id].push_back(box);
    }
    std::vector<BoxPair> pairs;
    // Construct pairs from matching IDs
    for (const auto& group:grouped){
        int pair_id = group.first;
        const std::vector<Box3D>& boxes = group.second;
        if (boxes.size() != 2){
            throw std::runtime_error("Expected exactly two boxes for pair ID "+std::to_string(pair_id));
        }
        BoxPair pair{};
        pair.pair_id = pair_id;
        pair.first = boxes[0];
        pair.second = boxes[1];
        pairs.push_back(pair);
    }
    return pairs;
}
}