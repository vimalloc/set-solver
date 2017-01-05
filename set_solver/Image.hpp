#ifndef SETSOLVER_IMAGE_HPP
#define SETSOLVER_IMAGE_HPP
#include <opencv2/opencv.hpp>
#include "Contours.hpp"


class Image {

public:
    /**
     * Display the original image
     */
    void displayOriginalImage(void) const;

    /**
     * Display the filtered contours found in this image
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
