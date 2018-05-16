#pragma once

#include "Primitive.h"
#include "Ray.h"
#include <opencv.hpp>
#include <iostream>
#include "RayTracer.h"
#include <thread>

class MyImage {
public:
	cv::Mat * M;
	int rows, cols;

	void drawPixel(int x, int y, cv::Vec3b color) {
		if (x >= cols || x < 0 || y >= rows || y < 0) return;
		M->at<cv::Vec3b>(y, x) = color;
	}

	MyImage(int rows, int cols) {
		M = new cv::Mat(rows, cols, CV_8UC3);
		this->rows = rows;
		this->cols = cols;
	}

	void showImage() {
		cv::imshow("M", *M);
		cv::waitKey(0);
	}
};

Color** pixel;
int finish = 0;//完成的线程数量
double maxPixel = 0.0;//最大rgb值

void getPixel(int startX, int endX, int startY, int endY, Primitive** myPri, int count, Light** light, int lightCount, Point see) {
	for (int i = startX; i < endX; ++i) {
		for (int j = startY; j < endY; ++j) {
			Color myColor(0, 0, 0);
			Direction direction = Vector3d(i, j, 0) - see;
			direction.normalize();
			Ray ray(see, direction, 1);
			rayTrace(myPri, count, light, lightCount, 1.0, myColor, ray);
			pixel[i][j] = myColor;	
		}
		std::cout << i << std::endl;
	}
	finish += 1;
}

void test(Primitive** myPri, Light** light, Vector3d size, Point see, int count, int lightCount) {
	MyImage myImage(size.y(), size.x());
	pixel = new Color*[size.x()];
	for (int i = 0; i < size.x(); ++i) {
		pixel[i] = new Color[size.y()];
	}
	std::thread t1(getPixel, 0, size.x() / 2, 0, size.y() / 2, myPri, count, light, lightCount, see);
	std::thread t2(getPixel, size.x()/2, size.x(), 0, size.y() / 2, myPri, count, light, lightCount, see);
	std::thread t3(getPixel, 0, size.x() / 2, size.y()/2, size.y(), myPri, count, light, lightCount, see);
	std::thread t4(getPixel, size.x()/2, size.x(), size.y() / 2, size.y(), myPri, count, light, lightCount, see);
	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();
	while (finish != 4) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
	for (int i = 0; i < size.x(); ++i) {
		for (int j = 0; j < size.y(); ++j) {
			myImage.drawPixel(i, j, cv::Vec3b(pixel[i][(int)size.y() - j].x(), pixel[i][(int)size.y()-j].y(), pixel[i][(int)size.y() - j].z()));
		}
	}
	myImage.showImage();
}
