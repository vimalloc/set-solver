//
// Created by Landon Gilbert-Bland on 1/1/17.
//
#ifndef SET_POSSIBLECARDS_HPP
#define SET_POSSIBLECARDS_HPP
#include <opencv2/opencv.hpp>
#include "Card.hpp"

class PossibleCards {

public:
    PossibleCards(std::string filename);
    void displayOriginalImage(void) const;
    void displayProcessedImage(void) const;
    void displayPossibleCards(void) const;
    std::vector<Card> getCards(void) const;

private:
    // Original color image (resized down)
    cv::Mat original_image;

    // Original image with a blur and grayscale applied
    cv::Mat processed_image;

    // Contours that might make up a set card
    std::vector<std::vector<cv::Point>> possible_cards;

    void findPossibleCards(int = 100, int = 1000);
};


#endif //SET_POSSIBLECARDS_HPP
