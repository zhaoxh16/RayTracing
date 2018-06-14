#pragma once
#include <vector>
#include "Primitive.h"
using namespace std;

class Bezier {
public:
	Bezier();
	Bezier(int m, int n);
	void setControlPoints(vector<Vector3d> points);
	int intersect(const Ray& ray, double& distance);
	void setTexture(string fileName);

	//�˴���������pointû�����壬���ص�color����һ��intersect���color
	Color getColor(Vector3d point);

	//�˴���������pointû�����壬���ص�direction����һ��intersect���direction
	Direction getNormal(const Point& p);


private:
	vector<vector<Vector3d>> controlPoints;
	int m, n;
	double u, v;//��¼��һ��intersect�õ��Ľ����u,v


};