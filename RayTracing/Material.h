#pragma once
#include <Eigen/Dense>

using namespace Eigen;
#define Color Vector3d

class Material {
public:
	Color color;//��ɫ
	double refl;//����ϵ��
	double diff;//������ϵ��
	double spec;//����ϵ��
	double refr;//����˥��ϵ��
	double nrefr;//������
	Material();
	Material(Color color, double reflection, double diffuse, double spec, double refr, double nrefr);
};