#include "PossibleCards.hpp"

PossibleCards::PossibleCards(String filename) {
    original_image = imread(filename, IMREAD_COLOR);
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
    resize(original_image, original_image, Size(new_width, new_height));

    // Save a grayscaled/blurred image into processed_image. This
    // reduces the noise of the image, so we can do more accurate
    // line detection
    cvtColor(original_image, processed_image, COLOR_BGR2GRAY);
    GaussianBlur(processed_image, processed_image, Size(3, 3), 0);
}

void PossibleCards::displayOriginalImage(void) {
    namedWindow("Display", WINDOW_AUTOSIZE);
    imshow("Display", original_image);
    cv::waitKey(0);
}

void PossibleCards::displayProcessedImage(void) {
    namedWindow("Display", WINDOW_AUTOSIZE);
    imshow("Display", processed_image);
    cv::waitKey(0);
}
