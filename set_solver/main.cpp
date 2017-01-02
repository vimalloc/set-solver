//
// Created by Landon Gilbert-Bland on 1/1/17.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include "PossibleCards.hpp"


/*
 * Way to debug stuff by printing out images
 *
 * PossibleCards class
 *   - Takes an image
 *   - Resizes it, blurs it, etc as needed (perhaps have a seperate grayscale image)
 *   - Number of possible cards in the image (rectanges). Use canny detection for now, figure all that out
 *     - Should be smart, rectangles must be about the same size, with others thrown out
 *
 * Card class
 *   - Takes an image of a card (basically bounding rect)
 *   - Does all the pre-processing which needs to be done
 *   - Can check for attributes of card
 */

using namespace std;

int main(int argc, char** argv ) {
    if ( argc != 2 ) {
        cout  << "usage: ./SetSolver <Image_Path>\n";
        return -1;
    }

    try {
        PossibleCards cards(argv[1]);
        //cards.displayOriginalImage();
        //cards.displayProcessedImage();
    } catch (std::runtime_error &e) {
        cout << e.what() << "\n";
        return -1;
    }

    return 0;
}
