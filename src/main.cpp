#include "data_loading.hpp"
#include "evaluator.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: box_eval_app <data_file>\n";
        return EXIT_FAILURE;
    }

    std::vector<bev::Box3D> boxes = bev::load_boxes(argv[1]);

    // group boxes into pairs by pair_id, evaluate each pair, print metrics

    return EXIT_SUCCESS;
}