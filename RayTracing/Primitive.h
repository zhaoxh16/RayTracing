#pragma once

#include <Eigen/Dense>
#include <string>
#include "Ray.h"
#include <cmath>
#include "Material.h"
#include "texture.h"

using namespace Eigen;

#define Color Vector3d
#define Point Vector3d

class Primitive {
public:
	Material material;//材质
	Texture* texture;
	bool useTexture;

	Primitive();
	Primitive(Material material);
	virtual int intersect(const Ray& ray, double& distance) = 0;//求与ray的交点，若无交点返回0，有交点且光源在外部返回1，有交点且光源在内部返回-1；同时将到交点的距离赋值给distance
	virtual Direction getNormal(const Point& p) = 0;//获得一点处的法向量
	virtual void setTexture(std::string fileName);
	virtual Color getColor(Vector3d point);
};

class Sphere: public Primitive {
public:
	Point centre;//圆心
	double radius;//半径
	double sqr;//半径的平方

	Sphere();
	Sphere(Point centre, double radius, Material material);
	int intersect(const Ray& ray, double& distance);//求交
	Direction getNormal(const Point& p);//p点的法向量（p在球面上）
	/*void setTexture(Texture* texture);
	void getColor(Vector3d point);*/
};

class Plane : public Primitive {
public:
	Direction normal;//法向量
	double D;

	Plane();
	Plane(Direction normal, double D, Material material);
	int intersect(const Ray& ray, double& distance);
	Direction getNormal(const Point& p);
	void setTextureStatus(double stretch, Vector3d startPoint, Vector3d XDirection, Vector3d YDirection);
};

class Triangle: public Primitive {
public:
	Triangle();
	Triangle(Vector3d p0,Vector3d p1, Vector3d p2, Material material);//逆时针输入顶点
	int intersect(const Ray& ray, double& distance);
	Direction getNormal(const Point& p);

private:
	vector<Vector3d> v;
	Direction normal;//normal依据右手定则(v[2]-v[1])x(v[0]-v[1])

};