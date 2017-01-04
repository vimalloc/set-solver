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

    auto processed_image = processImage();
    contours.find_contours_from_image(processed_image);
}
