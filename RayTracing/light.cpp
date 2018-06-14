#include "light.h"

double randomNumber(double limit) {
	return (double)rand() / (double)RAND_MAX * limit;
}

Light::Light() {
	color = Vector3d(0, 0, 0);
}

Light::Light(Vector3d color) {
	this->color = color/255.0;
}

void Light::setColor(Vector3d color) {
	this->color = color/255.0;
}

void CirclePlaneLight::setPosition(Vector3d center, double radius, Vector3d normal) {
	this->center = center;
	this->radius = radius;
	this->normal = normal;
	Vector3d randomVector(randomNumber(1), randomNumber(1), randomNumber(1));
	randomVector.normalize();
	while (randomVector == normal || randomVector == -normal) {
		randomVector = Vector3d(randomNumber(1), randomNumber(1), randomNumber(1));
		randomVector.normalize();
	}
	double dotResult = randomVector.dot(normal);
	a = randomVector - dotResult * normal;
	a.normalize();
	b = a.cross(normal);
	b.normalize();
}

Ray CirclePlaneLight::getRandomEmitRay() {
	return Ray(randomOrigin(), randomDirection(), 1);
}

Vector3d CirclePlaneLight::randomOrigin() {
	double theta = randomNumber(1) * 2 * 3.1415926535;
	Vector3d direction = a * cos(theta) + b * sin(theta);
	direction.normalize();
	double r = randomNumber(1)*radius;
	return r * direction + center;
}

Vector3d CirclePlaneLight::randomDirection() {
	Vector3d vec;
	do {
		double u = rand() % 9999 / (double)10000;
		double v = rand() % 9999 / (double)10000;
		double theta = 2 * 3.1415926535*u;
		double phi = acos(2 * v - 1);
		double x = sin(theta)*sin(phi);
		double y = cos(theta)*sin(phi);
		double z = cos(phi);
		vec = Vector3d(x, y, z);
		vec.normalize();
	} while (vec.dot(normal) < 0);
	return vec;
}