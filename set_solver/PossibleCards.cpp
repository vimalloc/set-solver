#include "PossibleCards.hpp"

// TODO the card hierarchys are going to get super fucked up. This isn't good
//      design at all. Fix it.


// Free (non-member) helper functions use by this class. Wrap them in a namespace
// here so they don't pollute any other namespaces
namespace {
    /**
     * Takes a vector of contours and removes any contours that aren't (approximatally)
     * a rectangle
     *
     * @param contours A vector of contours
     */
    void filter_contours_not_rectangles(std::vector<std::vector<cv::Point>> &contours) {
        contours.erase(
                std::remove_if(contours.begin(), contours.end(),
                               [](std::vector<cv::Point> &c) {
                                   // We don't want to change the contour for this image, it
                                   // could make bounding rectangles harder to use
                                   std::vector<cv::Point> poly_result;
                                   auto peri = arcLength(c, true);
                                   approxPolyDP(c, poly_result, 0.013 * peri, true);
                                   auto num_vertices = poly_result.size();
                                   return num_vertices != 4;
                               }),
                contours.end()
        );
    }

    void filter_contours_without_children(std::vector<std::vector<cv::Point>> &contours,
                                          std::vector<cv::Vec4i> &hierarchy) {
        // Get all the indexes we need to remove (From both contours and hierarchy)
        std::vector<int> to_remove;
        for (int i=0; i<hierarchy.size(); i++) {
            // If this has no children, that means we didn't detect any images
            // inside the possible card, so it can therefor not be a possible card
            int child_index = hierarchy[i][2];
            if (child_index == -1) {
                to_remove.push_back(i);
                continue;
            }

            // If this contour has a parent, and the parent has a similar
            // area to this contour, remove this contour. This indicates
            // detecting the outside and inside contours for a possible card.
            double this_area = cv::contourArea(contours[i]);
            double child_area = cv::contourArea(contours[child_index]);
            double child_parent_area_ratio = child_area / this_area;

            if (child_parent_area_ratio >= 0.85) {
                to_remove.push_back(i);
            }
        }

        // Actually remove the items from contours and hierarchy
        std::sort(to_remove.begin(), to_remove.end());
        for(long i = to_remove.size() - 1; i >= 0; i--) {
            contours.erase(contours.begin() + to_remove[i]);
        }
    }
}

// Consts to avoid magic numbers in this code
const int resized_width = 1000;
const int min_contour_area = 1000;
const int canny_threshold = 75;
const double similar_area_tolerance = 1.4;

PossibleCards::PossibleCards(std::string filename) {
    original_image = cv::imread(filename, cv::IMREAD_COLOR);
    if (original_image.empty()) {
        throw std::runtime_error("Failed to load an image");
    }

    // Resize the image, keeping the original aspect ratio. This is
    // important so that the cards don't get stretched or smushed,
    // which would cause issues when trying to find the shape of
    // a card
    double width = double(original_image.cols);
    double height = double(original_image.rows);
    int new_width = resized_width;
    int new_height = (int) ((height / width) * resized_width);
    cv::resize(original_image, original_image, cv::Size(new_width, new_height));

    processImage();
    findContours(canny_threshold);
    filterPossibleContours();
}

void PossibleCards::filterPossibleContours() {
    // Do some filtering based on the area of the cards, to attempt to preemptively
    // filter out any contour that isn't a good candidate for a card.
    filter_contours_without_children(contours, hierarchy);
    filter_contours_min_area(min_contour_area);
    filter_contours_not_rectangles(contours);
    filter_contours_similar_area(similar_area_tolerance);
}

std::vector<Card> PossibleCards::getCards(void) const {
    auto cards = std::vector<Card>();
    for (auto const &contour : contours) {
        cards.push_back(Card(original_image, contour));
    }
    return cards;
}
