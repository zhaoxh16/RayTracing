#pragma once

#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

const double alpha = 0.8;

struct HitPoint {
	Vector3d pos;
	Vector3d N;
	Vector3d dir;
	Vector3d pixelPos;
	double BRDF;
	Vector3d colorWeight;//三种颜色的weight
	double r;
	double count;
	Vector3d color;
	HitPoint(Vector3d pos, Vector3d N):pixelPos(0,0,0),BRDF(1),colorWeight(1,1,1),r(1),count(0),color(0,0,0) {
	
	}
	HitPoint(Vector3d pos, Vector3d N, Vector3d dir, Vector3d pixelPos, double BRDF, Vector3d colorWeight, double r, double count, Vector3d color):pos(pos),N(N),dir(dir),pixelPos(pixelPos),BRDF(BRDF),colorWeight(colorWeight),r(r),count(count),color(color){}
	void update(Color flux) {
		double ratio = sqrt((count + alpha) / (count + 1));
		r = r * ratio;
		color += flux * ratio;
		count += 1;
	}
};