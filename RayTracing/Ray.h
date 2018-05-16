#pragma once

//π‚œﬂ¿‡
#include <Eigen/Dense>

using namespace Eigen;

#define Point Vector3d
#define Direction Vector3d
#define Color Vector3d

class Ray {
public:
	Point origin;
	Direction direction;
	double nrefr;
	Ray();
	Ray(Point origin, Direction direction, double nrefr);
};
