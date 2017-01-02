#ifndef SETSOLVER_CARD_HPP
#define SETSOLVER_CARD_HPP
#include <opencv2/opencv.hpp>


class Card {

public:
    Card(const cv::Mat img);
    Card(const cv::Mat img, const std::vector<std::vector<cv::Point>> &contour, int vector_index);

    void displayCard(void) const;

private:
    cv::Mat original_image;
    cv::Mat processed_image;
};


#endif //SETSOLVER_CARD_HPP
