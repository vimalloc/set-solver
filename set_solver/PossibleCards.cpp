#include "PossibleCards.hpp"


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
        // TODO instead of looping backwords here, I should just update the
        //      hierarchy and the contours at the same time
        for (long i=hierarchy.size()-1; i>=0; i--) {
            if (hierarchy[i][2] == -1) {
                contours.erase(contours.begin() + i);
            }
        }
    }
}

// Consts to avoid magic numbers in this code
const int resized_width = 1000;
const int min_contour_area = 1000;
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
    findContours();
    filterPossibleContours();
}

void PossibleCards::filterPossibleContours() {
    // TODO If there are a lot of triangles in the cards, this will cause the
    //      triangles to be detected instead of the cards.
    //
    // Solution 1:
    //   - Look at image hierarchy. Only accept possible cards if the card has
    //     children contours. I think this would work well, but would complicate
    //     the code for removing elements from the contour list
    //
    // Solution 2:
    //   - Get rid of the 'similar area' requirement and just attempt to make
    //     all cards possible cards. The card would have to be smart enough to
    //     realize this wasn't a card, but it needs to be able to do that anyways.
    //     The hardest thing I think would be in the user interface, where when
    //     eventually we get this to android, we want to highlight the possible
    //     cards in real time, and highlighting the triangles would not look
    //     very good.
    //
    // Solution 3:
    //   - Color analysis. Make sure all the detected cards are similar in color.
    //     I can think of a million ways this could backfire (shadows, spilt stuff
    //     on cards, etc).

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
