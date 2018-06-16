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
	Material material;//����
	Texture* texture;
	bool useTexture;

	Primitive();
	Primitive(Material material);
	virtual int intersect(const Ray& ray, double& distance) = 0;//����ray�Ľ��㣬���޽��㷵��0���н����ҹ�Դ���ⲿ����1���н����ҹ�Դ���ڲ�����-1��ͬʱ��������ľ��븳ֵ��distance
	virtual Direction getNormal(const Point& p) = 0;//���һ�㴦�ķ�����
	virtual void setTexture(std::string fileName);
	virtual Color getColor(Vector3d point);
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
	/*void setTexture(Texture* texture);
	void getColor(Vector3d point);*/
};

class Plane : public Primitive {
public:
	Direction normal;//������
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
	Triangle(Vector3d p0,Vector3d p1, Vector3d p2, Material material);//��ʱ�����붥��
	int intersect(const Ray& ray, double& distance);
	Direction getNormal(const Point& p);

private:
	vector<Vector3d> v;
	Direction normal;//normal�������ֶ���(v[2]-v[1])x(v[0]-v[1])

};