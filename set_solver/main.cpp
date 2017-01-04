#include <iostream>
#include <opencv2/opencv.hpp>
#include "PossibleCards.hpp"


int main(int argc, char** argv ) {
    if ( argc != 2 ) {
        std::cout  << "usage: ./SetSolver <Image_Path>\n";
        return -1;
    }

    try {
        PossibleCards possible_cards(argv[1]);
        possible_cards.displayOriginalImage();
        possible_cards.displayContours();
        /*
        std::vector<Card> cards = possible_cards.getCards();
        std::cout << cards.size() << "\n";
        for (auto const &card : cards) {
            //card.displayOriginalImage();
            //card.displayProcessedImage();
            //card.displayContours();
        }
        */
    } catch (std::runtime_error &e) {
        std::cout << e.what() << "\n";
        return -1;
    }

    return 0;
}
