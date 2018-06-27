#pragma once
#include "Primitive.h"
#include <string>

class TriangleFace : public Primitive {
public:
	TriangleFace();
	TriangleFace(Point vertex[3]);
	Direction getNormal(const Point& p);
	int intersect(const Ray& ray, double& distance);

private:
	Point v[3];
	Direction N;
};

class Mesh: public Primitive {
public:
	Mesh();
	Direction getNormal(const Point& p);
	int intersect(const Ray& ray, double& distance);
	void loadMesh(const string fileName);

private:
	vector<TriangleFace*> faces;

};