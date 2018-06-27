#pragma once
#include <Eigen/Dense>
#include "Ray.h"
#include <random>
#include <cmath>

using namespace Eigen;

double randomNumber(double limit);

class Light{
public:
	Light();
	Light(Vector3d color);
	virtual Ray getRandomEmitRay() = 0;
	void setColor(Vector3d color);
	Vector3d getColor() { return color; }

private:
	Vector3d color;

};

class CirclePlaneLight: public Light{
public:
	using Light::Light;
	void setPosition(Vector3d center, double radius, Vector3d normal);
	Ray getRandomEmitRay();

private:
	Vector3d center, normal;//normal是灯的朝向
	double radius;
	Vector3d a, b;//两条相互垂直的半径
	Vector3d randomOrigin();
	Vector3d randomDirection();

};