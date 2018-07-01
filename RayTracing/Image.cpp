#include "Image.h"

Image::Image(int cols, int rows) {
	M = new cv::Mat(rows, cols, CV_8UC3);
	this->rows = rows;
	this->cols = cols;
}

void Image::drawPixel(int x, int y, cv::Vec3b color) {
	if (x >= cols || x < 0 || y >= rows || y < 0) return;
	M->at<cv::Vec3b>(y, x) = color;
}

void Image::showImage() {
	cv::imwrite("1.jpg", *M);
	cv::imshow("M", *M);
	cv::waitKey(0);
}
