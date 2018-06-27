#pragma once
#include <Eigen/Dense>

using namespace Eigen;

#define Color Vector3d

class Photon {
public:
	Vector3d pos;
	Color color;

	Photon();
	Photon(Vector3d pos);
};