#include "evaluator.hpp"
#include <array>
#include <iostream>
#include <vector>

namespace bev {

std::array<float,4> get_box_borders(const Box3D& box){
    // Format: Left, Right, Bottom, Top
    std::array<float, 4> borders;
    borders[0] = box.x;
    borders[1] = box.x + box.l;
    borders[2] = box.y;
    borders[3] = box.y + box.w;
    return borders;
}

using Point = std::array<float, 2>;

// Helper for getting box corners
std::array<Point, 4> get_rotated_corners(const Box3D& box) {
    // Corners without rotation in counterclockwise rotation
    std::array<Point, 4> corners = {{
        {-box.l/2, -box.w/2},
        { box.l/2, -box.w/2},
        { box.l/2,  box.w/2},
        {-box.l/2,  box.w/2}
    }};
    float yaw = normalize_yaw(box.yaw);
    float cos_yaw = std::cos(yaw);
    float sin_yaw = std::sin(yaw);
    // Rotation mapped corner coordinates
    for (int i = 0; i < 4; i++) {
        float dx = corners[i][0];
        float dy = corners[i][1];
        corners[i][0] = box.x + dx*cos_yaw - dy*sin_yaw;
        corners[i][1] = box.y + dx*sin_yaw + dy*cos_yaw;
    }
    return corners;
}

// Helper for checking which side of an edge a point lies for counterclockwise ordered edges
bool is_inside(const Point& p, const Point& a, const Point& b) {
    float cross = (b[0] - a[0])*(p[1] - a[1]) - (b[1] - a[1])*(p[0] - a[0]);
    if (cross >= 0.0f){
        return true;
    }
    else{
        return false;
    }
}

//Helper for finding where edges intersect
Point get_intersection(const Point& p, const Point& q, const Point& a, const Point& b) {
    float cross_p = (b[0] - a[0])*(p[1] - a[1]) - (b[1] - a[1])*(p[0] - a[0]);
    float cross_q = (b[0] - a[0])*(q[1] - a[1]) - (b[1] - a[1])*(q[0] - a[0]);
    float t = cross_p / (cross_p - cross_q);
    Point intersection;
    intersection[0] = p[0] + t*(q[0] - p[0]);
    intersection[1] = p[1] + t*(q[1] - p[1]);
    return intersection;
}

float get_area_overlap(const Box3D& box1, const Box3D& box2) {
    // overlap area of the two BEV projections
    // Get corners in a counterclockwise ordered rotation for each box
    std::array<Point, 4> corners1 = get_rotated_corners(box1);
    std::array<Point, 4> corners2 = get_rotated_corners(box2);
    // Clip corners to get corners of the overlapping polygon
        // For each edge of box 2
    std::vector<Point> polygon = {
    corners1[0],
    corners1[1],
    corners1[2],
    corners1[3]
    };
    for(int i = 0; i < 4; i++){
        Point a = corners2[i];
        Point b = corners2[(i + 1) % 4];
        std::vector<Point> clipped;
        // For each edge of the current polygon
        for (std::size_t j = 0; j < polygon.size(); j++){
            // Check if box 1 edge is inside of box 2 edge and handle appropriately
            Point p1 = polygon[j];
            Point p2 = polygon[(j+1)%polygon.size()];
            bool inside1 = is_inside(p1,a,b);
            bool inside2 = is_inside(p2,a,b);
            if (inside1 && inside2){
                clipped.push_back(p2);
            }
            else if (inside1 && !inside2){
                clipped.push_back(get_intersection(p1, p2, a, b));
            }
            else if (!inside1 && inside2){
                clipped.push_back(get_intersection(p1, p2, a, b));
                clipped.push_back(p2);
            }
        }
        polygon = clipped;
            if (polygon.empty()){
            break;
        }
    }

    // Area of polygon sorted in counterclockwise order is then obtained using the shoelace algorithm
    if (polygon.size() < 3){
        return 0.0f;
    }
    float dot1 = 0.0f;
    float dot2 = 0.0f;
    for (std::size_t i = 0; i < polygon.size(); i++){
        std::size_t next = (i + 1) % polygon.size();
        dot1 += polygon[i][0]*polygon[next][1];
        dot2 += polygon[i][1]*polygon[next][0];
    }
    return std::abs(dot1 - dot2)/2.0f;
}

float get_height_overlap(const Box3D& box1, const Box3D& box2) {
    // vertical overlap range at the overlapping region
    float bottom1 = box1.z;
    float top1 = box1.z + box1.h;
    float bottom2 = box2.z;
    float top2 = box2.z + box2.h;
    if (top1 <= bottom2 || bottom1 >= top2){
        return 0.0f;
    }
    float bottom;
    float top;
    if (bottom1 >= bottom2){
        bottom = bottom1;
    }
    else{
        bottom = bottom2;
    }
    if (top1 <= top2){
        top = top1;
    }
    else{
        top = top2;
    }
    return top - bottom;
}

float get_overlap_volume(const Box3D& box1, const Box3D& box2) {
    // area overlap times height overlap
    float area = get_area_overlap(box1, box2);
    float height = get_height_overlap(box1, box2);
    return area*height;
}

float get_individual_volume(const Box3D& box) {
    // l * w * h
    return box.l*box.w*box.h;
}

MetricsResult get_pair_metrics(const Box3D& box1, const Box3D& box2) {
    MetricsResult result{};
    // fill area/height/volume overlaps, individual volumes, and IoU
    result.area_overlap = get_area_overlap(box1, box2);
    result.height_overlap = get_height_overlap(box1, box2);
    result.overlap_volume = get_overlap_volume(box1, box2);
    result.box1_volume = get_individual_volume(box1);
    result.box2_volume = get_individual_volume(box2);
    result.iou = compute_iou(
        result.box1_volume,
        result.box2_volume,
        result.overlap_volume
    );
    return result;
    return result;
}

float compute_iou(float vol1, float vol2, float overlap_vol) {
    // overlap_vol / (vol1 + vol2 - overlap_vol); guard division by zero
    float union_vol = vol1 + vol2 - overlap_vol;
    if (union_vol <= 0.0f){
        return 0.0f;
    }
    return overlap_vol / union_vol;
}

}