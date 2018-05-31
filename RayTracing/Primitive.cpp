#include "Primitive.h"

Primitive::Primitive(): useTexture(false){}

Primitive::Primitive(Material material):material(material),useTexture(false){}

void Primitive::setTexture(std::string fileName) {
	useTexture = true;
	texture->setImage(fileName);
}

Color Primitive::getColor(Vector3d point) {
	if(useTexture) return texture->getColor(point);
	else return material.color;
}

Sphere::Sphere(): Primitive(),centre(-1,-1,-1),radius(-1),sqr(radius*radius){}

Sphere::Sphere(Point centre, double radius, Material material):
	Primitive(material), centre(centre), radius(radius), sqr(radius*radius){}

int Sphere::intersect(const Ray& ray, double& distance) {
	Vector3d l = centre - ray.origin;
	int originPosition;//0表示在球面上，-1表示在球体内部，1表示在球体外部
	double sql = l.dot(l);
	if (sql < sqr - 0.0000000001) originPosition = -1;
	else if (sql > sqr + 0.0000000001) originPosition = 1;
	else originPosition = 0;
	/*
	需要小心考虑originPosition=0的情况！！！！！！还没考虑！

	*/
	double tp = l.dot(ray.direction);
	if (originPosition == 1 && tp < 0) return 0;//不相交
	double sqd = sql - tp * tp;
	if (sqd > sqr) return 0;//不相交
	double sqt = sqr - sqd;
	double sqrtsqt = sqrt(sqt);
	if (originPosition == -1) distance = tp + sqrtsqt;
	else if (originPosition == 1) distance = tp - sqrtsqt;
	else {
		distance = tp + sqrtsqt;
		if (distance < 0.0000000001) return 0;//如果仅与origin相交则表示无交点
		else return -1;//当origin在球面上且ray与球面上另一点相交时，等同于origin在内部
	}
	return originPosition;
}

Direction Sphere::getNormal(const Point& p) {
	Direction d = p-centre;
	d.normalize();
	return d;
}

Plane::Plane():Primitive(), normal(0,0,0), D(0){
	texture = new PlaneTexture();
}

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

void Plane::setTextureStatus(double stretch, Vector3d startPoint, Vector3d XDirection, Vector3d YDirection) {
	texture->setStretch(stretch);
	texture->setStartPoint(startPoint);
	dynamic_cast<PlaneTexture*>(texture)->setXDirection(XDirection);
	dynamic_cast<PlaneTexture*>(texture)->setYDirection(YDirection);
}