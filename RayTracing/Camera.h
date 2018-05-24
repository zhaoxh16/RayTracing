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

private:
	Point _p;//相机所在位置

};