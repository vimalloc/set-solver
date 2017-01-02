#include <iostream>
#include <opencv2/opencv.hpp>
#include "PossibleCards.hpp"


int main(int argc, char** argv ) {
    if ( argc != 2 ) {
        std::cout  << "usage: ./SetSolver <Image_Path>\n";
        return -1;
    }

    try {
        PossibleCards cards(argv[1]);
        cards.displayOriginalImage();
        cards.displayProcessedImage();
        cards.displayPossibleCards();
    } catch (std::runtime_error &e) {
        std::cout << e.what() << "\n";
        return -1;
    }

    return 0;
}
