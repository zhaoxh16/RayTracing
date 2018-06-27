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
	Box box;//包围盒
	double intersectionT, intersectionTheta;
	Vector3d intersectionNormal;

public:
	Bezier(vector<Vector3d> p, double axisX, double axisZ, Material material);//Bezier曲线的控制顶点及其旋转轴
	//Vector3d getPoint(double t, double theta);//获得参数为t,theta的点
	int intersect(const Ray& ray, double& distance);//求与ray的交点，若无交点返回0，有交点且光源在外部返回1，有交点且光源在内部返回-1；同时将到交点的距离赋值给distance
	Direction getNormal(const Point& p);//获得上一次intersect点处的法向量，传入的p没有用
	bool iteration(double t, double u, double v, Vector3d& result, Vector3d& normal, const Ray& ray);
};
