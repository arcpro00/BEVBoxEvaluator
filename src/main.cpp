#include "data_loading.hpp"
#include "evaluator.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2){
        std::cerr << "Usage: box_eval_app <data_file>\n";
        return EXIT_FAILURE;
    }
    try {
        std::vector<bev::BoxPair> pairs = bev::load_box_pairs(argv[1]);
        if (pairs.empty()){
            std::cerr << "No box pairs found.\n";
            return EXIT_FAILURE;
        }
        float total_iou = 0.0f;
        float min_iou = 1.0f;
        float max_iou = 0.0f;
        int zero_overlap_count = 0;
        std::cout << "Pair ID | Area Overlap | Height Overlap | 3D IoU\n";
        std::cout << "-------------------------------------------------\n";
        for (const auto& pair:pairs){
            const bev::Box3D& box1 = pair.first;
            const bev::Box3D& box2 = pair.second;
            bev::MetricsResult metrics = bev::get_pair_metrics(box1,box2);
            std::cout << pair.pair_id << " | " << metrics.area_overlap << " | " << metrics.height_overlap << " | " << metrics.iou << "\n";
            total_iou += metrics.iou;
            if (metrics.iou < min_iou) {
                min_iou = metrics.iou;
            }

            if (metrics.iou > max_iou) {
                max_iou = metrics.iou;
            }

            if (metrics.iou == 0.0f) {
                zero_overlap_count++;
            }
        }
        std::cout << "\nPairs evaluated: " << pairs.size() << "\n";
        std::cout << "Mean 3D IoU:         " << total_iou / pairs.size() << "\n";
        std::cout << "Minimum 3D IoU:      " << min_iou << "\n";
        std::cout << "Maximum 3D IoU:      " << max_iou << "\n";
        std::cout << "Zero-overlap pairs:  " << zero_overlap_count << "\n";
    }
    catch (const std::exception& error){
        std::cerr << "Error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}