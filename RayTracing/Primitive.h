#pragma once

#include <Eigen/Dense>
#include <string>
#include "Ray.h"
#include <cmath>
#include "Material.h"

using namespace Eigen;

#define Color Vector3d
#define Point Vector3d

class Primitive {
public:
	Material material;//����

	Primitive();
	Primitive(Material material);
	virtual int intersect(const Ray& ray, double& distance) = 0;//����ray�Ľ��㣬���޽��㷵��0���н����ҹ�Դ���ⲿ����1���н����ҹ�Դ���ڲ�����-1��ͬʱ��������ľ��븳ֵ��distance
	virtual Direction getNormal(const Point& p) = 0;//���һ�㴦�ķ�����
};

class Sphere: public Primitive {
public:
	Point centre;//Բ��
	double radius;//�뾶
	double sqr;//�뾶��ƽ��

	Sphere();
	Sphere(Point centre, double radius, Material material);
	int intersect(const Ray& ray, double& distance);//��
	Direction getNormal(const Point& p);//p��ķ�������p�������ϣ�
};

class Light : public Sphere {
public:
	using Sphere::Sphere;
};

class Plane : public Primitive {
public:
	Direction normal;//������
	double D;

	Plane();
	Plane(Direction normal, double D, Material material);
	int intersect(const Ray& ray, double& distance);
	Direction getNormal(const Point& p);
};