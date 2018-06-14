#include "Mesh.h"

TriangleFace::TriangleFace() {}

TriangleFace::TriangleFace(Point vertex[3]) {
	this->v[0] = vertex[0];
	this->v[1] = vertex[1];
	this->v[2] = vertex[2];
	Vector3d u1 = v[1] - v[0];
	Vector3d u2 = v[2] - v[0];
	Vector3d u1xu2 = u1.cross(u2);
	u1xu2.normalize();
	N = u1xu2;
}

Direction TriangleFace::getNormal(const Point& p) {
	return N;
}

int TriangleFace::intersect(const Ray& ray, double &distance) {
	Vector3d E1 = v[0] - v[1];
	Vector3d E2 = v[0] - v[2];
	Vector3d S = v[0] - ray.origin;
	Matrix3d mRdE1E2;
	mRdE1E2 << ray.direction[0], E1[0], E2[0], ray.direction[1], E1[1], E2[1], ray.direction[2], E1[2], E2[2];
	double detRdE1E2 = mRdE1E2.determinant();
	Matrix3d mSE1E2;
	mSE1E2 << S[0], E1[0], E2[0], S[1], E1[1], E2[1], S[2], E1[2], E2[2];
	double detSE1E2 = mSE1E2.determinant();
	Matrix3d mRdSE2;
	mRdSE2 << ray.direction[0], S[0], E2[0], ray.direction[1], S[1], E2[1], ray.direction[2], S[2], E2[2];
	double detRdSE2 = mRdSE2.determinant();
	Matrix3d mRdE1S;
	mRdE1S << ray.direction[0], E1[0], S[0], ray.direction[1], E1[1], S[1], ray.direction[2], E1[2], S[2];
	double detRdE1S = mRdE1S.determinant();
	double t = detSE1E2 / detRdE1E2;
	double b = detRdSE2 / detRdE1E2;
	double r = detRdE1S / detRdE1E2;
	Point p = (1 - b - r)*v[0] + b * v[1] + r * v[2];
	Vector3d path = p - ray.origin;
	distance = sqrt(path.dot(path));
	if (t > 0 && 0 <= b && b + r <= 1) {
		if (ray.direction.dot(N) >= 0) return 1;
		else return -1;
	}
	else return 0;
}

Mesh::Mesh(){}

Direction Mesh::getNormal(const Point& p) {

}

int Mesh::intersect(const Ray& ray, double& distance) {
	return 0;
}

void Mesh::loadMesh(const string fileName) {

}

