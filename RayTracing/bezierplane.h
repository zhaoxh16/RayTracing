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

	//此处传进来的point没有意义，返回的color是上一个intersect点的color
	Color getColor(Vector3d point);

	//此处传进来的point没有意义，返回的direction是上一个intersect点的direction
	Direction getNormal(const Point& p);


private:
	vector<vector<Vector3d>> controlPoints;
	int m, n;
	double u, v;//记录上一次intersect得到的交点的u,v


};