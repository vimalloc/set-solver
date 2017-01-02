//
// Created by Landon Gilbert-Bland on 1/1/17.
//
#ifndef SET_POSSIBLECARDS_HPP
#define SET_POSSIBLECARDS_HPP
#include <opencv2/opencv.hpp>
#include "Card.hpp"

class PossibleCards {

public:
    /**
     * Create a new PossibleCards object from an image file.
     *
     * This raises a runtime error an image could not be loaded from the
     * specified file
     *
     * @param filename Name of the to try and find cards in
     */
    PossibleCards(std::string filename);

    /**
     * Display the original image
     */
    void displayOriginalImage(void) const;

    /**
     * Display the processed image
     */
    void displayProcessedImage(void) const;

    /**
     * Display the *possible* cards in this image. Cards highlighed
     * by this method may or may not be valid set cards
     */
    void displayPossibleCards(void) const;

    /**
     * Returns a vector of all the real set cards. This method analyzes
     * all of the possible cards and returns Card objects for each one
     * that happens to really be a card.
     *
     * The results of this operation are not saved in this object, and
     * multiple calls to this method will result in re-calculating
     * the cards every time. Store the results of this method and
     * avoid calling this multiple times
     *
     * @return a vector containing all the Cards that were found in this image
     */
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
