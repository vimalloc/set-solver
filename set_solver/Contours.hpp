//
// Created by lgbland on 1/3/17.
//

#ifndef SETSOLVER_CONTOURHIERARCHYHELPER_HPP
#define SETSOLVER_CONTOURHIERARCHYHELPER_HPP
#include <opencv2/opencv.hpp>


class Contours {

// TODO Mark everything constant that we can

public:
    /**
     * Initializes this object from an image. If this object has already been initialized
     * with a previous image, this will remove all data relating to the previous image
     * and replace it with the data for this new image
     *
     * @param img cv::Mat that is grayscaled
     * @param low_canny_threshold Lower threshold for canny line detection
     */
    void find_contours_from_image(cv::Mat img, int low_canny_threshold = 100);

    /**
     * Filter out any contours that have less area then min_area
     *
     * @param min_area minimum area to not mark an image as filtered
     */
    void filter_min_area(int min_area);

    /**
     * Filters any contours that do not have children
     */
    void filter_without_children();

    /**
     * Filters any contours where a child contour has the same approximate
     * area as the parent contour.
     *
     * @param area_ratio_threshold Threshold to consider two areas the same.
           *                       This should be between 0 and 1.
     */
    void filter_same_size_children(double area_ratio_threshold = 0.85);

    /**
     * Filters any contours that are not (aproximatally) a rectangle
     */
    void filter_not_rectangles();

    /**
     * Filter the contours down to the largest number of contours that share a
     * similar area
     *
     * @param area_tolerance Tolerance for two areas to be considered similar.
     *                       This should be between 0 and 1.
     */
    void filter_most_common_area(double area_tolerance);

    /**
     * Get a new vector of contours which contain only the contours that have
     * not been filtered out
     *
     * @return Vector of non-filtered contours
     */
    std::vector<std::vector<cv::Point>> get_filtered_contours() const;

private:
    // Vector of all the contours that are found in an image
    std::vector<std::vector<cv::Point>> contours;

    // Hierarchy for all the vectors in this image (as return from cv::RETR_TREE)
    std::vector<cv::Vec4i> hierarchy;

    // Areas of all the contours in the image. The indexes into this vector correspond
    // with the indexes into the contours vector. Ex: If areas[1] = 100, that means
    // that contours[1] has an area of 100
    std::vector<double> areas;

    // Vector of all the contours we have filtered out. We dont want to actually delete
    // the contours, as that would ruin the hierarchy map (and require us to do a lot
    // of vector shifting). Instead, we just keep track of what items have been filtered.
    // The indexes into this vector correspond with the indexes into the contours vector.
    // Ex: If filtered[1] = true, contours[1] is considered to be filtered
    std::vector<bool> filtered;

    // Find the areas for each contour and stores them in the areas vector
    void find_contour_areas(void);

    // Actually sorting the contours vector would be a nightmare. The values
    // of the hierarchy would need to be updated to match the new indexes in
    // the sorted array.
    //
    // Instead, we use this method to build a 'sorted mapping vector'. This
    // will return a new vector that cotains all the indexes into contours
    // in sorted order (by the contours area, from smallest to largest). We
    // can then use this to get a sorted result of the contours areas, without
    // having to sort the contours themselves (and thus, ruining the hierarchy).
    //
    // This is based on http://stackoverflow.com/questions/1577475/
    std::vector<int> sort_indexes_by_area();
};


#endif //SETSOLVER_CONTOURHIERARCHYHELPER_HPP
