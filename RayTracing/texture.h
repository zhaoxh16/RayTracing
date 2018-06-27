#pragma once
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace Eigen;
using namespace std;
using cv::Vec3b;
using cv::imread;
using cv::Mat;

class Texture {
public:
	Texture();
	void setImage(string fileName);
	void setStretch(double stretch);
	virtual void setStartPoint(Vector3d point) = 0;
	virtual Vector3d getColor(Vector3d point) = 0;

protected:
	double stretch;
	Mat image;
	int rows, cols;

};

class PlaneTexture :public Texture {
public:
	void setStartPoint(Vector3d point);
	void setXDirection(Vector3d XDirection);
	void setYDirection(Vector3d YDirection);
	Vector3d getColor(Vector3d point);

private:
	Vector3d XDirection;
	Vector3d YDirection;
	Vector3d startPoint;

};