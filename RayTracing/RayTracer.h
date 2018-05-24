#pragma once
#include "Ray.h"
#include "Primitive.h"
#include "Eigen/Dense"
#include <iostream>
#include <cmath>

using namespace Eigen;

const double weightLimit = 0.001;

double getShade(Ray ray, double distance, Primitive** pri, int count) {
	for (int k = 0; k < count; ++k) {
		Primitive* obj = pri[k];
		double d;
		int result = obj->intersect(ray, d);
		if (result != 0 && d < distance) {
			return 0;
		}
	}
	return 1.0;
}

double getDiffuse(double LdN, double diff) {
	if (diff > 0 && LdN > 0) return LdN * diff;
	else return 0;
}

double getSpec(double VdR, double spec) {
	if (VdR > 0) return pow(VdR, 20)*spec;
	else return 0;
}

int getIntersectPri(Primitive** pri, int count, Ray& ray, Primitive*& myPrimitive, double& myDistance) {
	myDistance = 10000000;
	int myResult = 0;
	for (int k = 0; k < count; ++k) {
		Primitive* obj = pri[k];
		double distance;
		int result = obj->intersect(ray, distance);
		if (result == 0) continue;
		if (distance<myDistance&& distance > 0.0000000001) {
			myDistance = distance;
			myPrimitive = obj;
			myResult = result;
		}
	}
	return myResult;
}

void getDiffuseAndPhong(Primitive** pri, int count, Primitive* obj, Light** light, int lightCount, Vector3d& N, Point& p, Color& color, Ray& ray) {
	for (int i = 0; i < lightCount; ++i) {
		Vector3d L = light[i]->centre - p;
		double distance = sqrt(L.dot(L));
		L.normalize();
		double LdN = L.dot(N);
		double diff = getDiffuse(LdN, obj->material.diff);
		Vector3d R = L - N * 2.0*LdN;
		double VdR = ray.direction.dot(R);
		double spec = getSpec(VdR, obj->material.spec);
		double shade = getShade(Ray(p + 0.00000001*L, L, ray.nrefr), distance, pri, count);
		color += shade * (diff + spec) *obj->material.color;
	}
}

void rayTrace(Primitive** pri, int count, Light** light, int lightCount, double weight, Color& _color, Ray ray) {
	if (weight < weightLimit) return;
	double myDistance = 1000000;
	Color myColor(0, 0, 0);
	Primitive* myPrimitive = NULL;
	//获得交点、距离、内部/外部
	int result = getIntersectPri(pri, count, ray, myPrimitive, myDistance);
	//如果没交点则返回
	if (result == 0) return;
	//计算交点和法线
	Point p = ray.origin + ray.direction * myDistance;
	Direction N = myPrimitive->getNormal(p);
	getDiffuseAndPhong(pri, count, myPrimitive, light, lightCount, N, p, myColor, ray);
	//更改颜色
	_color += myColor;
	//反射光线
	Direction R = ray.direction - N * ray.direction.dot(N)*2.0;
	if (myPrimitive->material.refl > 0) {
		double newWeight = weight * myPrimitive->material.refl;
		Color newColor(0, 0, 0);
		rayTrace(pri, count, light, lightCount, newWeight, newColor, Ray(p + R * 0.00000001, R, ray.nrefr));
		_color += newColor * myPrimitive->material.refl;
	}
	if (myPrimitive->material.refr > 0) {
		double nrefr;
		if(result == 1) nrefr = myPrimitive->material.nrefr;
		else nrefr = 1;
		double newWeight = weight * myPrimitive->material.refr;
		Direction newN = N * (double)result;
		double cosI = -newN.dot(ray.direction);
		double n = ray.nrefr / nrefr;
		double cosT2 = 1.0 - n*n*(1.0 - cosI * cosI);
		if (cosT2 > 0.00000001) {
			Direction T = n * ray.direction + (n * cosI - sqrt(cosT2))*newN;
			Color newColor(0, 0, 0);
			rayTrace(pri, count, light, lightCount, newWeight, newColor, Ray(p + T * 0.000001, T, nrefr));
			_color += newColor * myPrimitive->material.refr;
		}
	}
	if (_color.x() > 255) _color.x() = 255;
	if (_color.y() > 255) _color.y() = 255;
	if (_color.z() > 255) _color.z() = 255;
	//std::cout << _color << std::endl;
}