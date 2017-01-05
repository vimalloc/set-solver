#include "Card.hpp"

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

    // Do the contour detection
    auto processed_image = processImage();
    contours.find_contours_from_image(processed_image, 60);

    // Filter out everything except for (hopefully) the images in the card
    double min_area = (original_image.rows * original_image.cols) * 0.05;
    contours.filter_min_area(min_area);
    contours.filter_contours_near_edges(10);
    contours.filter_children();
    contours.filter_most_common_area(1.3);

    // This is what we think makes up shapes in this card image
    shapes = contours.get_filtered_contours();
}

long Card::get_number_of_shapes(void) const {
    return shapes.size();
}
