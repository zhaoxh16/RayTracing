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
	Point _p;//�������λ��
	double focalPlane;//��ƽ������λ�õ�z����
	double radius;

};