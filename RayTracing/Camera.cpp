#include "Camera.h"

Camera::Camera() :_p(-1, -1, -1) {}

Camera::Camera(Point p) : _p(p) {}

Camera::~Camera() {}

Vector3d Camera::pos()
{
	return _p;
}

Vector3d Camera::getRandomOrigin() {
	double theta = randomNumber(1) * 2 * 3.1415926535;
	Vector3d direction = Vector3d(1,0,0) * cos(theta) + Vector3d(0,1,0) * sin(theta);
	direction.normalize();
	double r = randomNumber(1)*radius;
	return r * direction + _p;
}