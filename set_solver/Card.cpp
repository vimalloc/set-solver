/*
 * Way to debug stuff by printing out images
 *
 * Card class
 *   - Takes an image of a card (basically bounding rect)
 *   - Does all the pre-processing which needs to be done
 *   - Can check for attributes of card
 */


#include "Card.hpp"

Card::Card(cv::Mat img) {
    original_image = img;
}


Card::Card(cv::Mat img, std::vector<cv::Point> contour) {
    cv::Rect roi = cv::boundingRect(contour);
    original_image = img(roi);




    /*
    val mask = new Mat(colorImg.size(), CV_8UC1, new Scalar(0))
    drawContours(mask, contours, i.toInt, new Scalar(255,255,255,0), CV_FILLED,
                 8, noArray(), Int.MaxValue, new Point())

    val imgRoi = new Mat()
    colorImg.copyTo(imgRoi, mask)
    val contourRegion = imgRoi(roi)
     */
}

void Card::displayCard(void) {
    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display", original_image);
    cv::waitKey(0);
}
