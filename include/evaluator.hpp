//Get the overlap: find overlap of the 2d projections of boxes, find the range of height at overlapping regions and multiply
#include "geometry.hpp"
float get_area_overlap(Box3D box1, Box3D box2);
float get_height_overlap(Box3D box1, Box3D box2);
float get_overlap_volume(Box3D box1, Box3D box2);
float get_individual_volume(Box3D box);