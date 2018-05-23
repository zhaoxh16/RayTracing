#pragma once
#include <Eigen/Dense>

using namespace Eigen;
#define Color Vector3d

class Material {
public:
	Color color;//颜色
	double refl;//反射系数
	double diff;//漫反射系数
	double spec;//镜面系数
	double refr;//折射衰减系数
	double nrefr;//折射率
	Material();
	Material(Color color, double reflection, double diffuse, double spec, double refr, double nrefr);
};