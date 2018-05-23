#include "Primitive.h"

Primitive::Primitive(){}

Primitive::Primitive(Material material):material(material){}

Sphere::Sphere(): Primitive(),centre(-1,-1,-1),radius(-1),sqr(radius*radius){}

Sphere::Sphere(Point centre, double radius, Material material):
	Primitive(material), centre(centre), radius(radius), sqr(radius*radius){}

int Sphere::intersect(const Ray& ray, double& distance) {
	Vector3d l = centre - ray.origin;
	int originPosition;//0��ʾ�������ϣ�-1��ʾ�������ڲ���1��ʾ�������ⲿ
	double sql = l.dot(l);
	if (sql < sqr - 0.0000000001) originPosition = -1;
	else if (sql > sqr + 0.0000000001) originPosition = 1;
	else originPosition = 0;
	/*
	��ҪС�Ŀ���originPosition=0�������������������û���ǣ�

	*/
	double tp = l.dot(ray.direction);
	if (originPosition == 1 && tp < 0) return 0;//���ཻ
	double sqd = sql - tp * tp;
	if (sqd > sqr) return 0;//���ཻ
	double sqt = sqr - sqd;
	double sqrtsqt = sqrt(sqt);
	if (originPosition == -1) distance = tp + sqrtsqt;
	else if (originPosition == 1) distance = tp - sqrtsqt;
	else {
		distance = tp + sqrtsqt;
		if (distance < 0.0000000001) return 0;//�������origin�ཻ���ʾ�޽���
		else return -1;//��origin����������ray����������һ���ཻʱ����ͬ��origin���ڲ�
	}
	return originPosition;
}

Direction Sphere::getNormal(const Point& p) {
	Direction d = p-centre;
	d.normalize();
	return d;
}

Plane::Plane():Primitive(), normal(0,0,0), D(0){}

Plane::Plane(Direction normal, double D, Material material):normal(normal),D(D), Primitive(material){}

int Plane::intersect(const Ray& ray, double& distance) {
	double nrd = ray.direction.dot(normal);
	if (nrd != 0) {
		distance = -(D + ray.origin.dot(normal)) / nrd;
		if (distance > 0.0000000001) return 1;
	}
	return 0;
}

Direction Plane::getNormal(const Point& p) {
	return -normal;
}