#include <numeric>
#include "Contours.hpp"

void Contours::find_contours_from_image(cv::Mat img, int low_canny_threshold) {
    // Delete anything data that was currently in here
    contours.empty();
    hierarchy.empty();
    areas.empty();
    filtered.empty();

    // Find contours on this image. Due to all the possible lighting/background situations
    // that the picture could have, I've found that Canny line detection works much more
    // consistently than performing a threshold
    cv::Mat canny_lines_img;
    cv::Canny(img, canny_lines_img, low_canny_threshold, low_canny_threshold * 3);
    cv::dilate(canny_lines_img, canny_lines_img, cv::Mat());
    cv::findContours(canny_lines_img, contours, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);

    // Now that we have our contours, setup the rest of the member variables
    // we need for this object to work.
    filtered.resize(contours.size());
    areas.resize(contours.size());
    find_contour_areas();
    std::fill(filtered.begin(), filtered.end(), false);
}

void Contours::find_contour_areas(void) {
    for(auto i=0; i<contours.size(); i++) {
        areas[i] = cv::contourArea(contours[i]);
    }
}

std::vector<int> Contours::sort_indexes_by_area() {
    // initialize original index locations
    std::vector<int> idx(contours.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing areas in the contours
    std::sort(
            idx.begin(),
            idx.end(),
            [this](size_t i1, size_t i2) {
                double area1 = this->areas[i1];
                double area2 = this->areas[i2];
                return area1 < area2;
            }
    );
    return idx;
}

void Contours::filter_min_area(double min_area) {
    for(auto i=0; i<contours.size(); i++) {
        if(areas[i] < min_area) {
            filtered[i] = true;
        }
    }
}

void Contours::filter_max_area(double max_area) {
    for(auto i=0; i<contours.size(); i++) {
        if(areas[i] > max_area) {
            filtered[i] = true;
        }
    }
}


void Contours::filter_without_children() {
    for (int i = 0; i < hierarchy.size(); i++) {
        int child_index = hierarchy[i][2];
        if (child_index == -1) {
            filtered[i] = true;
        }
    }
}

void Contours::filter_same_size_children(double area_ratio_threshold) {
    for (int i = 0; i < hierarchy.size(); i++) {
        int child_index = hierarchy[i][2];
        double parent_area = areas[i];
        double child_area = areas[child_index];
        double child_parent_area_ratio = child_area / parent_area;
        if (child_parent_area_ratio >= area_ratio_threshold) {
            filtered[i] = true;
        }
    }
}

void Contours::filter_not_rectangles() {
    for(auto i=0; i<contours.size(); i++) {
        // Skip if it is already filtered
        if (filtered[i] == true) {
            continue;
        }

        // Approximate the shape
        std::vector<cv::Point> contour = contours[i];
        std::vector<cv::Point> poly_result;
        auto perimeter = cv::arcLength(contour, true);
        approxPolyDP(contour, poly_result, 0.013 * perimeter, true);
        auto num_vertices = poly_result.size();

        // 4 verticies indicates it is a rectangle
        if (num_vertices != 4) {
            filtered[i] = true;
        }
    }
}

void Contours::filter_most_common_area(double area_tolerance) {
    // If area_tolerance was less then 1, this function would always return only the
    // contour with the greatest area.
    if (area_tolerance < 1.0) {
        throw std::out_of_range("area_tolerance must be >= 1.0");
    }

    // Actually sorting the contours vector would be a nightmare. The values
    // of the hierarchy would need to be updated to match the new indexes in
    // the sorted array. Instead, we are going to build a new vector which
    // contains all the indexes of the original contours vector, but the said
    // indexes are in order based on the the size of the contour.
    std::vector<int> sorted_indexes = sort_indexes_by_area();

    // Variables need for this algorithm
    double max_area_for_chain = 0.0;
    auto best_chain_length = 0;
    auto best_chain_start_index = 0;
    auto current_chain_length = 0;
    auto current_chain_start_index = 0;

    // Find the start index and length of the longest chain of similar areas
    for (auto i = 0; i < sorted_indexes.size(); i++) {
        auto index = sorted_indexes[i]; // Translate index into the contours vector index

        // Ignore elements that are already filtered out
        if (filtered[index] == true) {
            continue;
        }

        // Marks the end of a chain
        auto area = areas[index];
        if (area > max_area_for_chain) {
            current_chain_start_index = i;
            current_chain_length = 0;
        }

        // If this chain length ties a previous chain, save this chain as the better candidate.
        // Tie goes to the chain with bigger areas
        current_chain_length++;
        if (current_chain_length >= best_chain_length) {
            best_chain_length = current_chain_length;
            best_chain_start_index = current_chain_start_index;
        }

        // Have this be a running total. Example, if the first item was 10, the second item
        // was 12, and the third item was 14, if we base the max_area on the first item,
        // the chain wil only be 10 and 12, but if we update the max_area based on 12, 14
        // will now also be part of the chain.
        max_area_for_chain = area * area_tolerance;
    }

    // Mark everything as filtered that isn't in the best found chain
    int contours_hit = 0;
    for (int i = 0; i < sorted_indexes.size(); i++) {
        auto index = sorted_indexes[i]; // Translate index into the contours vector index

        if (i < best_chain_start_index) {
            filtered[index] = true;
        }
        else if (contours_hit != best_chain_length && filtered[index] == false) {
            contours_hit++;
        } else {
            filtered[index] = true;
        }
    }
}

std::vector<std::vector<cv::Point>> Contours::get_filtered_contours() const {
    std::vector<std::vector<cv::Point>> filtered_contours;
    for (auto i = 0; i < filtered.size(); i++) {
        if (filtered[i] == false) {
            filtered_contours.push_back(contours[i]);
        }
    }
    return filtered_contours;
}

// TODO I should just save edge thresholds instead of passing them into
//      this function
void Contours::filter_contours_edges(cv::Mat img, int distance_threshold) {
    int top_edge_threshold = distance_threshold;
    int bottom_edge_threshold = img.rows + distance_threshold;
    int left_edge_threshold = distance_threshold;
    int right_edge_threshold = img.cols + distance_threshold;

    for (auto i = 0; i < contours.size(); i++) {
        // Ignore anything that is already filtered
        if (filtered[i] == true) {
            continue;
        }

        // Loop over every point in this contour, and see if any of the points
        // lines up near any of the edges in the image
        for (auto const &point : contours[i]) {
            if (point.y < top_edge_threshold ||
                    point.y > bottom_edge_threshold ||
                    point.x < left_edge_threshold ||
                    point.x > right_edge_threshold) {

                // Debugging
                cv::Mat image_with_contours_outlined;
                img.copyTo(image_with_contours_outlined);
                cv::Scalar color(255, 0, 255);
                auto border_size = 2;
                cv::drawContours(image_with_contours_outlined, contours, i,
                                 color, border_size);
                cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
                cv::imshow("Display", image_with_contours_outlined);
                cv::waitKey(0);

                filtered[i] = true;
                break;
            }
        }
    }
}

