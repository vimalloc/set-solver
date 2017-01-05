#include "PossibleCards.hpp"

// TODO can still have a situation where a bunch of inner triangles are grabbed
//      instead of the real possible cards. Hasn't come up in any images I've
//      taken yet though... Think on it

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

    // Process our original image (grayscale and remove noise), and find
    // our contours based on the processed image
    auto processed_image = processImage();
    contours.find_contours_from_image(processed_image, canny_threshold);

    // Find any contours that are possibly a set card
    find_possible_cards();
}

void PossibleCards::find_possible_cards() {
    contours.filter_min_area(min_contour_area);
    contours.filter_without_children();
    contours.filter_same_size_children();
    contours.filter_not_rectangles();
    contours.filter_most_common_area(similar_area_tolerance);
    possible_cards = contours.get_filtered_contours();
}

std::vector<Card> PossibleCards::getCards(void) const {
    auto cards = std::vector<Card>();
    for (auto const &contour : possible_cards) {
        cards.push_back(Card(original_image, contour));
    }
    return cards;
}
