#include "Image.hpp"


void Image::displayOriginalImage(void) const {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display", original_image);
    cv::waitKey(0);
}

void Image::displayProcessedImage(void) const {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display", processed_image);
    cv::waitKey(0);
}

void Image::displayContours(void) const {
    cv::Mat image_with_contours_outlined;
    original_image.copyTo(image_with_contours_outlined);
    cv::Scalar pink(255, 0, 255);
    auto border_size = 2;
    cv::drawContours(image_with_contours_outlined, contours, -1, pink, border_size);
    cv::imshow("Display", image_with_contours_outlined);
    cv::waitKey(0);
}

void Image::processImage(cv::Size blur_size) {
    cv::cvtColor(original_image, processed_image, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(processed_image, processed_image, blur_size, 0);
}

void Image::findContours(int low_threshold) {
    // Due to all the possible lighting/background situations that the picture could
    // be in, I've found that Canny line detection works much more consistently than
    // thresholding
    cv::Mat canny_lines_img;
    cv::Canny(processed_image, canny_lines_img, low_threshold, low_threshold * 3);
    cv::findContours(canny_lines_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}

void Image::filter_contours_min_area(int min_area) {
    contours.erase(
            std::remove_if(contours.begin(), contours.end(),
                           [&min_area](std::vector<cv::Point> &c) {
                               return cv::contourArea(c) <= min_area;
                           }),
            contours.end()
    );
}

void Image::sort_contours_by_area() {
    std::sort(
            contours.begin(),
            contours.end(),
            [](std::vector<cv::Point> &c1, std::vector<cv::Point> &c2) {
                return cv::contourArea(c1) < cv::contourArea(c2);
            }
    );

}

void Image::filter_contours_similar_area(double area_tolerance) {
    // If area_tolerance was less then 1, this function would always return only the
    // contour with the greatest area.
    if (area_tolerance < 1.0) {
        throw std::out_of_range("area_tolerance must be >= 1.0");
    }

    double max_area_for_chain = 0.0;
    auto best_chain_length = 0;
    auto best_chain_start_index = 0;
    auto current_chain_length = 0;
    auto current_chain_start_index = 0;

    // Find the start index and length of the longest chain of similar areas
    sort_contours_by_area();
    for(int i=0; i<contours.size(); i++) {
        auto area = cv::contourArea(contours[i]);

        // Marks the end of a chain
        if (area > max_area_for_chain) {
            current_chain_start_index = i;
            current_chain_length = 0;
        }

        // If this chain length ties a previous chain, save this chain as the better candidate.
        // Tie goes to the chain with bigger areas
        current_chain_length++;
        if (current_chain_length >= best_chain_length) {
            best_chain_length = current_chain_length;
            best_chain_start_index = current_chain_start_index;
        }


        // Have this be a running total. Example, if the first item was 10, the second item
        // was 12, and the third item was 14, if we base the max_area on the first item,
        // the chain wil only be 10 and 12, but if we update the max_area based on 12, 14
        // will now also be part of the chain.
        max_area_for_chain = area * area_tolerance;
    }

    // Remove all contours from the vector that are not part of the largest chain.
    // We have to remove the end ones first, otherwise removing the front ones would
    // change the indexes for the end ones.
    contours.erase(contours.begin() + best_chain_start_index + best_chain_length, contours.end());
    contours.erase(contours.begin(), contours.begin() + best_chain_start_index);
}
