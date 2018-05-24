#pragma once
#include <Eigen/Dense>

using namespace Eigen;

class Photon {
public:
	Vector3d pos;

	Photon();
	Photon(Vector3d pos);
};