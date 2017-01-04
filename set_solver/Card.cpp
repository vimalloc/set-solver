#include "Card.hpp"

const int min_area = 100;

Card::Card(const cv::Mat &img, const std::vector<cv::Point> &contour) {
    // We need a vector of contour vectors for drawContours to work.
    std::vector<std::vector<cv::Point>> container = std::vector<std::vector<cv::Point>>(1, contour);

    // Create a mask where everything beside our contour is black
    cv::Mat mask(img.size(), CV_8UC1, cv::Scalar(0));
    cv::drawContours(mask, container, -1, cv::Scalar(255,255,255,0), CV_FILLED);
    cv::Mat masked_image;
    img.copyTo(masked_image, mask);

    // Used the masked image to create our region of interest, where everything outside
    // of the contour is black
    cv::Rect roi = cv::boundingRect(contour);
    original_image = masked_image(roi);

    // TODO need a way to filter out the outside contours.
    //   Solution 1:
    //      - Increase the size of the bonding rectangle, so it detects the outside
    //        of the card as one continous contour, and use filter_max_area
    //      - Or similarly, just fiat that there will be an outer contour, and
    //        ignore that one?
    //
    //   Solution 2:
    //      - Remove any contours that have a point next to the edge of an image

    // Find the contours and attempt to filter everything out except the
    // images in the card
    auto processed_image = processImage();
    contours.find_contours_from_image(processed_image);
    contours.filter_min_area(min_area);
    contours.filter_max_area((img.rows * img.cols) / 0.85);
    contours.filter_same_size_children();
    contours.filter_contours_edges(original_image, 40);
    contours.filter_most_common_area(1.4);
}
