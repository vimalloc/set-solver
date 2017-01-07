#ifndef SETSOLVER_COLOR_HPP
#define SETSOLVER_COLOR_HPP
#include <opencv2/opencv.hpp>

class Color {

public:
    Color(const cv::Mat &img);
    Color(const cv::Mat &img, const cv::Rect);
    Color(const cv::Mat &img, const std::vector<cv::Point> &contour);

    cv::Vec3b get_darkest_pixel(int offset, int dark_pixel_threshold = 0);
    int get_intensity_mean_std();

private:
    cv::Mat original_img;
    cv::Mat grayscale_img;
};


#endif //SETSOLVER_COLOR_HPP
