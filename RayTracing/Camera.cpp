#include "Camera.h"

Camera::Camera() :_p(-1, -1, -1) {}

Camera::Camera(Point p) : _p(p) {}

Camera::~Camera() {}

Vector3d Camera::pos()
{
	return _p;
}