#ifndef SETSOLVER_IMAGE_HPP
#define SETSOLVER_IMAGE_HPP
#include <opencv2/opencv.hpp>


// TODO will I need to mess with hierarchy here if all the cards are in a container
//      which has it's lines detected?
//
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
     * Display the processed image
     */
    void displayProcessedImage(void) const;

    /**
     * Display the *possible* cards in this image. Cards highlighed
     * by this method may or may not be valid set cards
     */
    void displayContours(void) const;


protected:
    // Original color image (resized down)
    cv::Mat original_image;

    // Original image with a blur and grayscale applied
    cv::Mat processed_image;

    // Contours that might make up a set card
    std::vector<std::vector<cv::Point>> contours;

    /**
     * Proccesses an image, applying a blur to reduce noise and grayscaling the
     * image so line detection can be applied
     *
     * @param blur_size cv::Size passed directly to the guassian blur as the
     *        ksize paramater
     */
    void processImage(cv::Size blur_size = cv::Size(3, 3));

    /**
     * Helper method which finds contours in the processed image and
     * saves them in the contours vector
     *
     * @param low_threshold lower threshold to use for canny line detection
     */
    void findContours(int low_threshold = 100);

    /**
     * Sort the contours vector by the area of the contours
     */
    void sort_contours_by_area();

    /**
     * Remove any contours from the contours vector that has a area * less then
     * the specified min_area
     *
     * @param min_area minimum area required to not remove an element from contours
     */
    void filter_contours_min_area(int min_area);

    /**
     * Find the largest number of contours that share a similiar area. Remove all other
     * contours from the contours vector.
     *
     * For example, if the contours had areas of [10, 50, 53, 59, 100], and the area
     * tolerance was 1.2, the vector would be modified to be [50, 53, 59].
     *
     *  10 * 1.2 = 12, so that would be one possible element
     *  50 * 1.2 = 60, so that would include 3 possible elements [50, 53, 59]
     *  etc
     *
     * @param tolerance how much larger one element can be over another element for it to
     *                  still be considered a similiar area. 1.0 indicates they must be an
     *                  exact match. This should be 1.0 or greater.
     */
    void filter_contours_similar_area(double area_tolerance);
};


#endif //SETSOLVER_IMAGE_HPP
