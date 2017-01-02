#include "PossibleCards.hpp"


// Free (non-member) helper functions use by this class. Wrap them in a namespace
// here so they don't pollute any other namespaces
namespace {

    /**
     * Remove any contours from a vector of contours that has a area
     * less then the specified min_area
     *
     * @param contours vector of contours
     * @param min_area minimum area required to not remove an element from contours
     */
    void filter_contours_min_area(std::vector<std::vector<cv::Point>> &contours, int min_area) {
        contours.erase(
                std::remove_if(contours.begin(), contours.end(),
                               [&min_area](std::vector<cv::Point> &c) {
                                   return cv::contourArea(c) <= min_area;
                               }),
                contours.end()
        );
    }

    /**
     * Given a vector of contours, sort the contours by their area
     *
     * @param contours vector of contours
     */
    void sort_contours_by_area(std::vector<std::vector<cv::Point>> &contours) {
        std::sort(
                contours.begin(),
                contours.end(),
                [](std::vector<cv::Point> &c1, std::vector<cv::Point> &c2) {
                    return cv::contourArea(c1) < cv::contourArea(c2);
                }
        );

    }

    /**
     * Given a vector of contours, find the largest number of vectors that share a similiar
     * area. Remove all other contours from this vector.
     *
     * For example, if the contours had areas of [10, 50, 53, 59, 100], and the area
     * tolerance was 1.2, the vector would be modified to be [50, 53, 59].
     *
     *  10 * 1.2 = 12, so that would be one possible element
     *  50 * 1.2 = 60, so that would include 3 possible elements [50, 53, 59]
     *  etc
     *
     * @param contours vector of contours
     * @param tolerance how much larger one element can be over another element for it to
     *                  still be considered a similiar area. 1.0 indicates they must be an
     *                  exact match. This should be 1.0 or greater.
     */
    void filter_contours_similiar_area(std::vector<std::vector<cv::Point>> &contours, double area_tolerance) {
        // If area_tolerance was less then 1, this function would always return only the
        // contour with the greatest area.
        if (area_tolerance < 1.0) {
            throw std::out_of_range("area_tolerance must be >= 1.0");
        }

        double max_area_for_chain = 0;
        int best_chain_length = 0;
        int best_chain_start_index = 0;
        int current_chain_length = 0;
        int current_chain_start_index = 0;

        // Find the start index and length of the longest chain of similar areas
        sort_contours_by_area(contours);
        for(int i=0; i<contours.size(); i++) {
            double area = cv::contourArea(contours[i]);

            // Marks the end of a chain
            if (area > max_area_for_chain) {
                current_chain_start_index = i;
                current_chain_length = 0;
            }

            // If this chain length ties a previous chain, save this chain as the better candidate.
            // Tie goes to the chain with bigger areas
            current_chain_length++;
            if (current_chain_length >= best_chain_length) {
                best_chain_length = current_chain_length;
                best_chain_start_index = current_chain_start_index;
            }


            // Have this be a running total. Example, if the first item was 10, the second item
            // was 12, and the third item was 14, if we base the max_area on the first item,
            // the chain wil only be 10 and 12, but if we update the max_area based on 12, 14
            // will now also be part of the chain.
            max_area_for_chain = area * area_tolerance;
        }

        // Remove all contours from the vector that are not part of the largest chain.
        // We have to remove the end ones first, otherwise removing the front ones would
        // change the indexes for the end ones.
        contours.erase(contours.begin() + best_chain_start_index + best_chain_length, contours.end());
        contours.erase(contours.begin(), contours.begin() + best_chain_start_index);
    }
}

// Consts to avoid magic numbers in this code
const int resized_width = 1000;
const double similar_area_tolerance = 1.4;

PossibleCards::PossibleCards(std::string filename) {
    original_image = cv::imread(filename, cv::IMREAD_COLOR);
    if (original_image.empty()) {
        throw std::runtime_error("Failed to load an image");
    }

    // Resize the image, keeping the original aspect ratio. This is
    // important so that the cards don't get stretched or smushed,
    // which would cause issues when trying to find the shape of
    // a card
    double width = double(original_image.cols);
    double height = double(original_image.rows);
    int new_width = resized_width;
    int new_height = (int) ((height / width) * resized_width);
    resize(original_image, original_image, cv::Size(new_width, new_height));

    // Save a grayscaled/blurred image into processed_image. This
    // reduces the noise of the image, so we can do more accurate
    // line detection
    cvtColor(original_image, processed_image, cv::COLOR_BGR2GRAY);
    GaussianBlur(processed_image, processed_image, cv::Size(3, 3), 0);
    findPossibleCards();
}

// TODO will I need to mess with hierarchy here if all the cards are in a container
//      which has it's lines detected?
// TODO need to test this against a white background, but if we don't detect any (or many)
//      possible cards with the default low_threshold (possibly due to white cards on a
//      white background) we could try cutting the threshold in half and seeing if it
//      yields better results
void PossibleCards::findPossibleCards(int low_threshold, int min_area) {
    cv::Mat canny_lines_img;

    // Due to all the possible lighting/background situations that the picture could
    // be in, I've found that Canny line detection works much more consistently than
    // thresholding
    Canny(processed_image, canny_lines_img, low_threshold, low_threshold * 3);
    findContours(canny_lines_img, possible_cards, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Do some filtering based on the area of the cards, to attempt to pre-emptively
    // filter out any contour that isn't a good canidate for a card
    filter_contours_min_area(possible_cards, min_area);
    filter_contours_similiar_area(possible_cards, similar_area_tolerance);
}

void PossibleCards::displayOriginalImage(void) {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    imshow("Display", original_image);
    cv::waitKey(0);
}

void PossibleCards::displayProcessedImage(void) {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    imshow("Display", processed_image);
    cv::waitKey(0);
}

void PossibleCards::displayPossibleCards(void) {
    cv::Mat image_with_contours_outlined;
    original_image.copyTo(image_with_contours_outlined);
    cv::Scalar pink(255, 0, 255);
    int border_size = 2;
    drawContours(image_with_contours_outlined, possible_cards, -1, pink, border_size);
    cv::imshow("Display", image_with_contours_outlined);
    cv::waitKey(0);
}
