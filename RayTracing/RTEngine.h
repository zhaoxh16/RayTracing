#pragma once
#include <Eigen/Dense>
#include "Scene.h"
#include "Image.h"

using namespace Eigen;

class RTEngine {
public:
	RTEngine();
	void setScene(Scene* scene);
	void render();
	void show();

private:
	Scene * scene;
	Image* image;
	static const double weightLimit;//递归深度
	static const double distanceINF;
	static const double distanceDelta;
	static const double phongPower;

	cv::Vec3b getPixel(int i, int j);
	void rayTrace(Ray ray, double weight, Vector3d& color);
	Point getOrigin(Point origin, Direction direction);//为防止在模型边缘出现问题，对origin进行更改
	int getIntersectPri(Ray ray, Primitive*& collidePrimitive, double& collideDistance);
	void getDiffuseAndPhong(Primitive* collidePrimitive, Vector3d N, Point p, Color& color, Ray ray);
	double getDiffuse(double LdN, double diff) {
		if (diff > 0 && LdN > 0) return LdN * diff;
		else return 0;
	}
	double getSpec(double VdR, double spec) {
		if (VdR > 0) return pow(VdR, phongPower)*spec;
		else return 0;
	}
	double getShade(Ray ray, double distance);

};