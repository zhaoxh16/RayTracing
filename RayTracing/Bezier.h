#pragma once
#include <vector>
#include "Primitive.h"
#include "light.h"
using namespace std;

struct Box {
	Vector3d minPoint;
	Vector3d maxPoint;

	Box() { }
	Box(Vector3d minPoint, Vector3d maxPoint) {
		this->minPoint = minPoint;
		this->maxPoint = maxPoint;
	}

	int intersect(const Ray& ray, double& ta, double& tb);
};

class Bezier: public Primitive {
private:
	Vector3d* _p;
	int _pNum;
	double axisX, axisZ;
	Box box;//��Χ��
	double intersectionT, intersectionTheta;
	Vector3d intersectionNormal;

public:
	Bezier(vector<Vector3d> p, double axisX, double axisZ, Material material);//Bezier���ߵĿ��ƶ��㼰����ת��
	//Vector3d getPoint(double t, double theta);//��ò���Ϊt,theta�ĵ�
	int intersect(const Ray& ray, double& distance);//����ray�Ľ��㣬���޽��㷵��0���н����ҹ�Դ���ⲿ����1���н����ҹ�Դ���ڲ�����-1��ͬʱ��������ľ��븳ֵ��distance
	Direction getNormal(const Point& p);//�����һ��intersect�㴦�ķ������������pû����
	bool iteration(double t, double u, double v, Vector3d& result, Vector3d& normal, const Ray& ray);
};
