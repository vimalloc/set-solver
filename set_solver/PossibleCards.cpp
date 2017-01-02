#include "PossibleCards.hpp"

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
    int new_width = 1000;
    int new_height = (int) ((height / width) * new_width);
    resize(original_image, original_image, cv::Size(new_width, new_height));

    // Save a grayscaled/blurred image into processed_image. This
    // reduces the noise of the image, so we can do more accurate
    // line detection
    cvtColor(original_image, processed_image, cv::COLOR_BGR2GRAY);
    GaussianBlur(processed_image, processed_image, cv::Size(3, 3), 0);
    findPossibleCards();
}

void PossibleCards::displayOriginalImage(void) {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    imshow("Display", original_image);
    cv::waitKey(0);
}

void PossibleCards::displayProcessedImage(void) {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    imshow("Display", processed_image);
    cv::waitKey(0);
}

void PossibleCards::findPossibleCards(int low_threshold, int min_area) {
    cv::Mat canny_lines_img;

    // Attempt to grab just the lines, so we can build contours from those.
    Canny(processed_image, canny_lines_img, low_threshold, low_threshold * 3);

    // Debug1
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    imshow("Display", original_image);
    cv::waitKey(0);

    // Detect the contours
    findContours(canny_lines_img, possible_cards, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // We resized the image so the width is 1000 pixels. Therefor, each card
    // should have a decent number of pixels to them. Remove any contours
    // that are bellow our area threshold
    possible_cards.erase(
        std::remove_if(possible_cards.begin(), possible_cards.end(),
                       [&min_area](std::vector<cv::Point> &c) {
                           return cv::contourArea(c) <= min_area;
                       }),
        possible_cards.end()
    );

    // Debug2
    cv::Mat image_with_contours_outlined;
    original_image.copyTo(image_with_contours_outlined);
    cv::Scalar pink(255, 0, 255);
    drawContours(image_with_contours_outlined, possible_cards, -1, pink, 2);
    cv::imshow("Display", image_with_contours_outlined);
    cv::waitKey(0);
}
