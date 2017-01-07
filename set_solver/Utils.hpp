//
// Created by Landon Gilbert-Bland on 1/6/17.
//

#ifndef SETSOLVER_UTILS_HPP
#define SETSOLVER_UTILS_HPP

#include <opencv2/opencv.hpp>

// TODO Add more method strings to everything. Slacking in this commit I think

cv::Mat mask_image_to_contour(const cv::Mat &img, std::vector<cv::Point> contour);

#endif //SETSOLVER_UTILS_HPP
