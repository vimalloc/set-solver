//
// Created by Landon Gilbert-Bland on 1/1/17.
//
#ifndef SET_POSSIBLECARDS_HPP
#define SET_POSSIBLECARDS_HPP
#include <opencv2/opencv.hpp>

using namespace cv;


class PossibleCards {

public:
    PossibleCards(String filename);
    void displayOriginalImage(void);
    void displayProcessedImage(void);

private:
    // Original color image (resized down)
    Mat original_image;

    // Original image with a blur and grayscale applied
    Mat processed_image;

};


#endif //SET_POSSIBLECARDS_HPP
