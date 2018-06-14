#pragma once
#include <Eigen/Dense>
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

private:
	Point _p;//�������λ��
	double focalPlane;//��ƽ������λ�õ�z����


};