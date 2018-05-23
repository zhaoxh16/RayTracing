#pragma once

#include <opencv.hpp>

class Image {
public:
	Image(int cols, int rows);
	void drawPixel(int x, int y, cv::Vec3b color);
	void showImage();

private:
	cv::Mat* M;
	int rows, cols;
};
