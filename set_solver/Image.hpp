#ifndef SETSOLVER_IMAGE_HPP
#define SETSOLVER_IMAGE_HPP
#include <opencv2/opencv.hpp>
#include <set>
#include "Contours.hpp"


// TODO need to test this against a white background, but if we don't detect any (or many)
//      possible cards with the default low_threshold (possibly due to white cards on a
//      white background) we could try cutting the threshold in half and seeing if it
//      yields better results

class Image {

public:
    /**
     * Display the original image
     */
    void displayOriginalImage(void) const;

    /**
     * Display the *possible* cards in this image. Cards highlighed
     * by this method may or may not be valid set cards
     */
    void displayContours(void) const;


protected:
    // Original color image (resized down)
    cv::Mat original_image;

    // Helper class for handling contours in this image
    Contours contours;

    /**
     * Proccesses an image, applying a blur to reduce noise and grayscaling the
     * image so line detection can be applied
     *
     * @param blur_size cv::Size passed directly to the guassian blur as the
     *        ksize paramater
     */
    cv::Mat processImage(cv::Size blur_size = cv::Size(3, 3));
};


#endif //SETSOLVER_IMAGE_HPP
