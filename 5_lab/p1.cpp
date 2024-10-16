#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("lenna.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    // Apply Gaussian blur
    cv::Mat image_blur;
    cv::GaussianBlur(image, image_blur, cv::Size(5, 5), 0, 0);

    // Display the image
    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);

    cv::imshow("Display window", image_blur);

    cv::imshow("Display window", image);
    cv::waitKey(0);
    return 0;
}
