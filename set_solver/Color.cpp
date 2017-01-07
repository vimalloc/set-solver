#include <array>
#include "Color.hpp"
#include "Utils.hpp"

namespace {
    typedef struct {
        int intensity;
        int original_index;
    } PixelIntensity;

    // Deep copy all the pixels from a grayscale image into a new vector, while storing
    // their index into the original image
    std::vector<PixelIntensity> image_to_pixel_intensity(cv::Mat &img) {
        unsigned long size = (unsigned long) (img.rows * img.cols);
        std::vector<PixelIntensity> pixels;
        pixels.reserve(size);

        unsigned char *input = img.data;
        for(auto i = 0; i < size; i++) {
            PixelIntensity p;
            p.original_index = i;
            p.intensity = input[i];
            pixels.push_back(p);
        }

        return pixels;
    }

    // Sort a list of pixels (by their intensity) without modifying their original
    // index into the image.
    void sort_pixel_intensities(std::vector<PixelIntensity> &pixels) {
        std::sort(
            pixels.begin(),
            pixels.end(),
            [](PixelIntensity p1, PixelIntensity p2) {
                return p1.intensity < p2.intensity;
            }
        );
    }
}


Color::Color(const cv::Mat &img) {
    original_img = img;
    cv::cvtColor(img, grayscale_img, cv::COLOR_BGR2GRAY);
}

Color::Color(const cv::Mat &img, const cv::Rect) {
    // TODO only move images in the rect into new grayscale mat
    original_img = img;
    cv::cvtColor(img, grayscale_img, cv::COLOR_BGR2GRAY);
}

Color::Color(const cv::Mat &img, const std::vector<cv::Point> &contour) {
    original_img = mask_image_to_contour(img, contour);
    cv::cvtColor(original_img, grayscale_img, cv::COLOR_BGR2GRAY);
}

cv::Vec3b Color::get_darkest_pixel(int offset, int dark_pixel_threshold) {
    // Get a list of pixels sorted by their intensity, with access to the original
    // index into the image
    std::vector<PixelIntensity> pixels = image_to_pixel_intensity(grayscale_img);
    sort_pixel_intensities(pixels);

    // Find the pixel offset away from the darkest pixel in this image (ignoring any
    // pixels that are darker then dark_pixel_threshold)
    auto index = offset;
    for(auto i = 0; i < pixels.size(); i++) {
        PixelIntensity pixel = pixels[i];
        if (pixel.intensity > dark_pixel_threshold) {
            index += i;
            break;
        }
    }

    int real_index = pixels[index].original_index;
    return original_img.at<cv::Vec3b>(real_index);
}

int Color::get_intensity_mean_std() {
    return 0;
}
