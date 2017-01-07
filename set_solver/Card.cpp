#include <array>
#include <unordered_set>
#include "Card.hpp"
#include "Color.hpp"
#include "Utils.hpp"


namespace {
    std::string bgr_to_set_color(cv::Vec3b pixel) {
        int blue = pixel.val[0];
        int green = pixel.val[1];
        int red = pixel.val[2];
        if (green > blue && green > red) {
            return "green";
        } else if ((red * 0.6) > blue) {
            return "red";
        } else {
            return "purple";
        }

    }
}

Card::Card(const cv::Mat &img, const std::vector<cv::Point> &contour) {
    // Create a new image that is resized to hold only the contour, and where
    // every pixel outside of the contour is set to black
    original_image = mask_image_to_contour(img, contour);

    // Do the contour detection
    auto processed_image = processImage();
    contours.find_contours_from_image(processed_image, 60);

    // Filter out everything except for (hopefully) the images in the card
    double min_area = (original_image.rows * original_image.cols) * 0.05;
    contours.filter_min_area(min_area);
    contours.filter_contours_near_edges(10);
    contours.filter_children();
    contours.filter_most_common_area(1.3);

    // This is what we think makes up shapes in this card image
    shapes = contours.get_filtered_contours();
}

long Card::get_number_of_shapes(void) const {
    return shapes.size();
}

std::string Card::get_color(void) const {
    // Assuming our shape detection is correct, we only need to look
    // at one of the shapes to determine the color. However, to try and
    // get a more accurate reading (and avoid potential errors), we are
    // going to look at each of our shape contours individually and make
    // sure they all return the same color
    std::unordered_set<std::string> card_colors;
    for (auto const &contour : shapes) {
        Color c(original_image, contour);

        // Ignoring pure black, don't grab the absolute darkest pixel in this
        // card. If there is an ink smudge or something on the card, it will
        // detect that instead of a pixel on the the border (which should be
        // the darkest pixel on a pristine set card). We know the approximate
        // size of the border, so by indexing into an offset related to that,
        // we have a better chance of getting the pixel we are looking for
        // (and not getting fooled by a non-pristine set card)
        int offset = (int) (contour.size() / 0.66);
        int pure_black = 0;
        auto bgr_pixel = c.get_darkest_pixel(offset, pure_black);

        // Get the probable Set color for the pixel at this image
        std::string shape_color = bgr_to_set_color(bgr_pixel);
        card_colors.insert(shape_color);
    }

    // TODO raise exception instead of return error
    // Make sure all our shapes have the same set color
    if (card_colors.size() != 1) {
        return "error!";
    } else{
        return *card_colors.begin();
    }
}
