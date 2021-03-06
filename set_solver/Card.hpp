#ifndef SETSOLVER_CARD_HPP
#define SETSOLVER_CARD_HPP
#include <opencv2/opencv.hpp>
#include "Image.hpp"


class Card : public Image {

public:
    /**
     * Create a new Card from an original image and a contour that describes a card
     *
     * @param img Original image to create a card from
     * @param contours Contour which decribes a card in the image
     */
    Card(const cv::Mat &img, const std::vector<cv::Point> &contour);

    long get_number_of_shapes(void) const;

    std::string get_color(void) const;

private:
    std::vector<std::vector<cv::Point>> shapes;
};


#endif //SETSOLVER_CARD_HPP
