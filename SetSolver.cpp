#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>


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

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}
