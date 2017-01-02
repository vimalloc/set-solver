/*
 * Way to debug stuff by printing out images
 *
 * Card class
 *   - Takes an image of a card (basically bounding rect)
 *   - Does all the pre-processing which needs to be done
 *   - Can check for attributes of card
 */


#include "Card.hpp"

Card::Card(cv::Mat img) {
    original_image = img;
}


Card::Card(const cv::Mat img, const std::vector<std::vector<cv::Point>> &contours,
           int contour_index) {
    // Create a mask where everything beside our contour is black
    cv::Mat mask(img.size(), CV_8UC1, cv::Scalar(0));
    cv::drawContours(mask, contours, contour_index, cv::Scalar(255,255,255,0), CV_FILLED);
    cv::Mat masked_image;
    img.copyTo(masked_image, mask);

    // Used the masked image to create our region of intreset, where everything outside
    // of the contour is black
    cv::Rect roi = cv::boundingRect(contours[contour_index]);
    original_image = masked_image(roi);
}

void Card::displayCard(void) const {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display", original_image);
    cv::waitKey(0);
}
