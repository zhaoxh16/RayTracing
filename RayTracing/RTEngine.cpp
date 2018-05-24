#include "RTEngine.h"

const double RTEngine::weightLimit = 0.001;
const double RTEngine::distanceINF = 100000000;
const double RTEngine::distanceDelta = 0.00000001;
const double RTEngine::phongPower = 20;

RTEngine::RTEngine():scene(NULL), image(NULL){}

void RTEngine::setScene(Scene* scene) {
	this->scene = scene;
	this->image = new Image(scene->size().x(), scene->size().y());
}

void RTEngine::render() {
	for (int i = 0; i < scene->size().x(); ++i) {
		for (int j = 0; j < scene->size().y(); ++j) {
			image->drawPixel(i, scene->size().y()-j, getPixel(i, j));
		}
		cout << i << endl;
	}
}

void RTEngine::show() {
	image->showImage();
}

cv::Vec3b RTEngine::getPixel(int i, int j) {
	Vector3d color(0, 0, 0);
	Vector3d direction = Vector3d(i, j, 0) - scene->camera()->pos();
	direction.normalize();
	Ray ray(scene->camera()->pos(), direction, 1.0);
	rayTrace(ray,1.0,color);
	cv::Vec3b cvColor(color.x(), color.y(), color.z());
	return cvColor;
}

void RTEngine::rayTrace(Ray ray, double weight, Vector3d& color) {
	if (weight < weightLimit) return;
	double collideDistance = distanceINF;
	Color myColor(0, 0, 0);
	Primitive* collidePrimitive = NULL;
	int result = getIntersectPri(ray, collidePrimitive, collideDistance);
	if (result == 0)return;
	Point p = ray.origin + ray.direction*collideDistance;
	Direction N = collidePrimitive->getNormal(p);
	getDiffuseAndPhong(collidePrimitive, N, p, myColor, ray);
	color += myColor;
	Direction R = ray.direction - N * ray.direction.dot(N)*2.0;
	if (collidePrimitive->material.refl > 0) {
		double newWeight = weight * collidePrimitive->material.refl;
		Color newColor(0, 0, 0);
		rayTrace(Ray(getOrigin(p, R), R, ray.nrefr),newWeight, newColor);
		color += newColor * collidePrimitive->material.refl;
	}
	if (collidePrimitive->material.refr > 0) {
		double nrefr;
		if (result == 1)nrefr = collidePrimitive->material.nrefr;
		else nrefr = 1;
		double newWeight = weight * collidePrimitive->material.refr;
		Direction newN = N * (double)result;
		double cosI = -newN.dot(ray.direction);
		double n = ray.nrefr / nrefr;
		double cosT2 = 1.0 - n * n*(1.0 - cosI * cosI);
		if (cosT2 > distanceDelta) {
			Direction T = n * ray.direction + (n*cosI - sqrt(cosT2))*newN;
			Color newColor(0, 0, 0);
			rayTrace(Ray(getOrigin(p, T), T, nrefr), newWeight, newColor);
			color += newColor * collidePrimitive->material.refr;
		}
	}
	if (color.x() > 255)color.x() = 255;
	if (color.y() > 255)color.y() = 255;
	if (color.z() > 255)color.z() = 255;
}

Point RTEngine::getOrigin(Point origin, Direction direction) {
	return origin + direction * distanceDelta;
}

int RTEngine::getIntersectPri(Ray ray, Primitive*& collidePrimitive, double& collideDistance) {
	collideDistance = distanceINF;
	int myResult = 0;
	for (int k = 0; k < scene->primitive()->size(); ++k) {
		Primitive* obj = scene->primitive()->at(k);
		double distance;
		int result = obj->intersect(ray, distance);
		if (result == 0) continue;
		if (distance<collideDistance && distance>distanceDelta) {
			collideDistance = distance;
			collidePrimitive = obj;
			myResult = result;
		}
	}
	return myResult;
}

void RTEngine::getDiffuseAndPhong(Primitive* collidePrimitive, Vector3d N, Point p, Color& color, Ray ray) {
	for (int i = 0; i < scene->light()->size(); ++i) {
		Vector3d L = scene->light()->at(i)->centre - p;
		double distance = sqrt(L.dot(L));
		L.normalize();
		double LdN = L.dot(N);
		double diff = getDiffuse(LdN, collidePrimitive->material.diff);
		Vector3d R = L - N * 2.0*LdN;
		double VdR = ray.direction.dot(R);
		double spec = getSpec(VdR, collidePrimitive->material.spec);
		double shade = getShade(Ray(getOrigin(p, L), L, ray.nrefr), distance);
		color += shade * (diff + spec)*collidePrimitive->material.color;
	}
}

double RTEngine::getShade(Ray ray, double distance) {
	for (int i = 0; i < scene->primitive()->size(); ++i) {
		Primitive* obj = scene->primitive()->at(i);
		double d;
		int result = obj->intersect(ray, d);
		if (result != 0 && d < distance) return 0;
	}
	return 1.0;
}