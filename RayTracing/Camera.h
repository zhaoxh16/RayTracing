#pragma once
#include <Eigen/Dense>
#include "light.h"
using namespace Eigen;

#define Point Vector3d

class Camera {
public:
	Camera();
	Camera(Point p);
	~Camera();
	Vector3d pos();
	void setFocalPlane(double focalPlane) {
		this->focalPlane = focalPlane;
	}
	void setRadius(double radius) {
		this->radius = radius;
	}
	double getFocalPlane() { return focalPlane; }
	Vector3d getRandomOrigin();

private:
	Point _p;//相机所在位置
	double focalPlane;//焦平面所在位置的z分量
	double radius;

};