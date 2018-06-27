#include "Primitive.h"

Primitive::Primitive(): useTexture(false){}

Primitive::Primitive(Material material):material(material),useTexture(false){}

void Primitive::setTexture(std::string fileName) {
	useTexture = true;
	texture = new PlaneTexture();
	texture->setImage(fileName);
}

Color Primitive::getColor(Vector3d point) {
	if(useTexture)
		return texture->getColor(point)/255.0;
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

Triangle::Triangle():Primitive(), normal(0,0,0) {
}

Triangle::Triangle(Vector3d p0,Vector3d p1, Vector3d p2, Material material):Primitive(material) {
	v[0] = p0;
	v[1] = p1;
	v[2] = p2;
	this->normal = (v[2] - v[1]).cross(v[0] - v[1]);
	normal.normalize();
}

Direction Triangle::getNormal(const Point& p) {
	return normal;
}

int Triangle::intersect(const Ray& ray, double& distance) {
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
		if (ray.direction.dot(normal) < 0) return 1;
		else return -1;
	}
	else return 0;
}