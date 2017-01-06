#include <array>
#include "Card.hpp"

Card::Card(const cv::Mat &img, const std::vector<cv::Point> &contour) {
    // We need a vector of contour vectors for drawContours to work.
    std::vector<std::vector<cv::Point>> container = std::vector<std::vector<cv::Point>>(1, contour);

    // Create a mask where everything beside our contour is black
    cv::Mat mask(img.size(), CV_8UC1, cv::Scalar(0));
    cv::drawContours(mask, container, -1, cv::Scalar(255,255,255,0), CV_FILLED);
    cv::Mat masked_image;
    img.copyTo(masked_image, mask);

    // Used the masked image to create our region of interest, where everything outside
    // of the contour is black
    cv::Rect roi = cv::boundingRect(contour);
    original_image = masked_image(roi);

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
    // TODO iti should be more effeciant if we only analyze one of the shapes

    // Grayscale the picture, we only want to deal with one channel to make our
    // lives easier. The colors on the card should always be darker (closer to
    // 0) then the rest of the card, so this is a good fit.
    cv::Mat gray_img;
    cv::cvtColor(original_image, gray_img, cv::COLOR_BGR2GRAY);


    // This stores the the grayscale intensity of a pixel, as well as the
    // index of this pixel in the original image. We are going to sort all
    // the pixels by their grayscale intensity, but we need to be able to
    // get the original pixel that grayscaled pixel was pointing at back out
    unsigned long size = (unsigned long) (gray_img.rows * gray_img.cols);
    std::vector<std::array<int, 2>> intensities_indexes;
    intensities_indexes.reserve(size);

    // Deep copy all the grayscale intensities and their original indexes into
    // a new vector
    unsigned char *input = gray_img.data;
    for(auto i = 0; i < size; i++) {
        std::array<int, 2> intensity_index;
        intensity_index[0] = int(input[i]);
        intensity_index[1] = i;
        intensities_indexes.push_back(intensity_index);
    }


    // Sort this deep copied vector by the grayscale intensities. item[0] is the
    // intensity, and item[1] is the original index
    std::sort(
            intensities_indexes.begin(),
            intensities_indexes.end(),
            [](std::array<int, 2> i1, std::array<int, 2> i2) { return i1[0] < i2[0]; }
    );

    // We want to get one of the darkest pixels of the image, as that should be on
    // the color border that we are looking for. We don't grab the darkest pixel,
    // in case there is something else (pen, etc) on the card. We know how many
    // pixels made up the contour for this shape, and since the actual border is
    // slightly smaller than the contour we have, if we grab the pixel at 2/3's
    // the number of pixels in the contour, we should have a pretty safe bet
    // (unless there is significant 'defects, such as ink, on the card.
    int index = 0;
    for (auto const &contour : shapes) {
        index += contour.size();
    }
    index = (int) (index * 0.5);

    // Adjust our index to account for all of the pure black (intenstiy 0) pixels
    // there are in the picture (as a result of the bounding rectangle on the
    // original image
    for(auto i = 0; i < intensities_indexes.size(); i++) {
        if (intensities_indexes[i][0] != 0) {
            index += i;
            break;
        }
    }

    // Use the index into the intensities_indexes vector to get the index back
    // into the original image
    int real_index = intensities_indexes[index][1];

    // Pull out the pixel, and use it to see what the card color is
    auto pixel = original_image.at<cv::Vec3b>(real_index);
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
