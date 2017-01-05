#ifndef SET_POSSIBLECARDS_HPP
#define SET_POSSIBLECARDS_HPP
#include <opencv2/opencv.hpp>
#include "Card.hpp"
#include "Image.hpp"

// TODO need to test this against a white background, but if we don't detect any (or many)
//      possible cards with the default low_threshold (possibly due to white cards on a
//      white background) we could try cutting the threshold in half and seeing if it
//      yields better results

class PossibleCards : public Image {

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
    std::vector<std::vector<cv::Point>> possible_cards;

    // Helper method to find contours which are possibly a set cards in this image
    void find_possible_cards();
};


#endif //SET_POSSIBLECARDS_HPP
