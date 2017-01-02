#ifndef SETSOLVER_CARD_HPP
#define SETSOLVER_CARD_HPP
#include <opencv2/opencv.hpp>


class Card {

public:
    Card(cv::Mat img);
    Card(cv::Mat img, std::vector<cv::Point> contour);
    void displayCard(void) const;

private:
    cv::Mat original_image;
};


#endif //SETSOLVER_CARD_HPP
