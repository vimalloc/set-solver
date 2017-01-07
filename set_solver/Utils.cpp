#include "Utils.hpp"

cv::Mat mask_image_to_contour(const cv::Mat &img, std::vector<cv::Point> contour) { // We need a vector of contour vectors for drawContours to work.
    std::vector<std::vector<cv::Point>> container = std::vector<std::vector<cv::Point>>(1, contour);

    // Create a mask where everything beside our contour is black
    cv::Mat mask(img.size(), CV_8UC1, cv::Scalar(0));
    cv::drawContours(mask, container, -1, cv::Scalar(255,255,255,0), CV_FILLED);
    cv::Mat masked_image;
    img.copyTo(masked_image, mask);

    // Used the masked image to create our region of interest, where everything outside
    // of the contour is black, and we have a bounding rectangle to make the image as
    // small as possible while still including all of the contour
    cv::Rect roi = cv::boundingRect(contour);
    return masked_image(roi);
}
