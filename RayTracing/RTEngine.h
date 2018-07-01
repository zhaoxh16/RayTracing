#pragma once
#include <Eigen/Dense>
#include "Scene.h"
#include "Image.h"
#include "kdtree.h"
#include "hitpoint.h"
#include "light.h"
#include "omp.h"

using namespace Eigen;

class RTEngine {
public:
	RTEngine();
	void setScene(Scene* scene);
	void render();
	void show();

	//������PPM�¼ӵ�����
	void emitRay();
	void PPMRender();

private:
	Scene* scene;
	Image* image;
	static const double weightLimit;//�ݹ����
	static const double distanceINF;
	static const double distanceDelta;
	static const double phongPower;

	cv::Vec3b getPixel(int i, int j);
	void rayTrace(Ray ray, double weight, Vector3d& color);
	Point getOrigin(Point origin, Direction direction);//Ϊ��ֹ��ģ�ͱ�Ե�������⣬��origin���и���
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

	//������PPM�¼ӵ�
	KDTree tree;
	int photonCount;//�����������
	void PPMTrace(Ray ray, double weight, Vector3d pixel);
	Color** colorMap;

	//�����ĳ����Դ����һ������
	void emitPhoton();
	Vector3d getRandomNormalizeVector();
	void photonTrace(Ray ray, Color& photonColor, int depth);

	//����
	void getRayDOF(Vector3d originalPosition, int number, vector<Ray>& rays);


};