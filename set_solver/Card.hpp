#ifndef SETSOLVER_CARD_HPP
#define SETSOLVER_CARD_HPP
#include <opencv2/opencv.hpp>


class Card {

public:
    /**
     * Create a new Card, where the image is directly what is passed to
     * this constructor
     *
     * @param img Mat for the image of this card
     */
    Card(const cv::Mat img);

    /**
     * Create a new Card from an original image and a contour that describes a card
     *
     * @param img Original image to create a card from
     * @param contours Contour which decribes a card in the image
     */
    Card(const cv::Mat &img, const std::vector<cv::Point> &contour);

    /**
     * Displays the image for this card
     */
    void displayCard(void) const;

private:
    cv::Mat original_image;
    cv::Mat processed_image;
};


#endif //SETSOLVER_CARD_HPP
