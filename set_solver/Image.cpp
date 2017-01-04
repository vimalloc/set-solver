#include "Image.hpp"

void Image::displayOriginalImage(void) const {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display", original_image);
    cv::waitKey(0);
}

void Image::displayContours(void) const {
    // Filtered list of contours we are interested in
    auto filtered_contours = contours.get_filtered_contours();

    // We don't want to override the original image with this. Make a copy
    // to keep the original intact
    cv::Mat image_with_contours_outlined;
    original_image.copyTo(image_with_contours_outlined);

    cv::Scalar color(255, 0, 255);
    auto border_size = 2;
    auto contour_index = -1; // -1 indicates all contours should be drawn
    cv::drawContours(image_with_contours_outlined, filtered_contours, contour_index,
                     color, border_size);

    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display", image_with_contours_outlined);
    cv::waitKey(0);
}

cv::Mat Image::processImage(cv::Size blur_size) {
    cv::Mat processed_image;
    cv::cvtColor(original_image, processed_image, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(processed_image, processed_image, blur_size, 0);
    return processed_image;
}

