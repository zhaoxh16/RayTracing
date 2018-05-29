#include "RTEngine.h"

const double RTEngine::weightLimit = 0.001;
const double RTEngine::distanceINF = 100000000;
const double RTEngine::distanceDelta = 0.00000001;
const double RTEngine::phongPower = 20;

RTEngine::RTEngine():scene(NULL), image(NULL){
	photonCount = 0;
}

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

void RTEngine::emitRay() {
	for (int i = 0; i < scene->size().x(); ++i) {
		for (int j = 0; j < scene->size().y(); ++j) {
			Vector3d direction = Vector3d(i, j, 0) - scene->camera()->pos();
			direction.normalize();
			Ray ray(scene->camera()->pos(), direction, 1.0);
			PPMTrace(ray, 1.0, Vector3d(i, j, 0));
		}
		cout << i << endl;
	}
	tree.build();
}

void RTEngine::PPMRender() {
	for (int i = 0; i < 10000; ++i) {
		emitPhoton();
		if (i % 1000 == 0) cout << i << endl;
	}
	colorMap = new Color*[scene->size().x()];
	for (int i = 0; i < scene->size().x(); ++i) {
		colorMap[i] = new Color[scene->size().y()];
		for (int j = 0; j < scene->size().y(); ++j) {
			colorMap[i][j] = Color(0, 0, 0);
		}
	}
	cout << tree.getNodes().size() << endl;
	vector<Node*>& nodes = tree.nodes;
	int totn = 0;
	for (auto i : nodes) {
		totn++;
		HitPoint* p = i->obj;
		colorMap[(int)p->pixelPos[0]][(int)p->pixelPos[1]] += p->color / (3.1415926*p->r*p->r*(double)photonCount);
		if(totn%1000 == 0) cout << totn << endl;
	}/*
	for (int i = 0; i < tree.getNodes().size(); ++i) {
		//cout << "1" << endl;
		HitPoint* p = tree.getNodes().at(i)->obj;
		//cout << "2" << endl;
		colorMap[(int)p->pixelPos[0]][(int)p->pixelPos[1]] += p->color / (3.1415926*p->r*p->r*(double)photonCount);
		//cout << "3" << endl;
		/*if(i % 1000== 0) cout << i << endl;
	}*/
	for (int i = 0; i < scene->size().x(); ++i) {
		for (int j = 0; j < scene->size().y(); ++j) {
			if (colorMap[i][j].x() > 1) colorMap[i][j].x() = 1;
			if (colorMap[i][j].y() > 1) colorMap[i][j].y() = 1;
			if (colorMap[i][j].z() > 1) colorMap[i][j].z() = 1;
			image->drawPixel(i, scene->size().y() - j, cv::Vec3b(colorMap[i][j].x()*255.0, colorMap[i][j].y()*255.0, colorMap[i][j].z()*255.0));
		}
		cout << i << endl;
	}
}

cv::Vec3b RTEngine::getPixel(int i, int j) {
	Vector3d color(0, 0, 0);
	Vector3d direction = Vector3d(i, j, 0) - scene->camera()->pos();
	direction.normalize();
	Ray ray(scene->camera()->pos(), direction, 1.0);
	rayTrace(ray,1.0,color);
	cv::Vec3b cvColor(color.x()*255.0, color.y()*255.0, color.z()*255.0);
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
	if (color.x() > 1.0)color.x() = 1.0;
	if (color.y() > 1.0)color.y() = 1.0;
	if (color.z() > 1.0)color.z() = 1.0;
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
		Color lightColor = scene->light()->at(i)->material.color;
		double distance = sqrt(L.dot(L));
		L.normalize();
		double LdN = L.dot(N);
		double diff = getDiffuse(LdN, collidePrimitive->material.diff);
		Vector3d R = L - N * 2.0*LdN;
		double VdR = ray.direction.dot(R);
		double spec = getSpec(VdR, collidePrimitive->material.spec);
		double shade = getShade(Ray(getOrigin(p, L), L, ray.nrefr), distance);
		color += shade * (diff + spec)*Color(collidePrimitive->material.color.x()*lightColor.x(), collidePrimitive->material.color.y()*lightColor.y(), collidePrimitive->material.color.z()*lightColor.z());
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

void RTEngine::PPMTrace(Ray ray, double weight, Vector3d pixelPos) {
	if (weight < weightLimit) return;
	double collideDistance = distanceINF;
	Primitive* collidePrimitive = NULL;
	int result = getIntersectPri(ray, collidePrimitive, collideDistance);
	if (result == 0)return;
	Point p = ray.origin + ray.direction*collideDistance;
	Direction N = collidePrimitive->getNormal(p);
	HitPoint* hitPoint = new HitPoint(p, N, ray.direction, pixelPos, collidePrimitive->material.diff, collidePrimitive->material.color*weight, iniRadius, 0, Vector3d(0, 0, 0));
	tree.addNode(hitPoint);
	Direction R = ray.direction - N * ray.direction.dot(N)*2.0;
	if (collidePrimitive->material.refl > 0) {
		double newWeight = weight * collidePrimitive->material.refl;
		PPMTrace(Ray(getOrigin(p, R), R, ray.nrefr), newWeight, pixelPos);
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
			rayTrace(Ray(getOrigin(p, T), T, nrefr), newWeight, pixelPos);
		}
	}
}

void RTEngine::emitPhoton() {
	int lightCount = scene->light()->size();
	int lightNum = rand() % lightCount;
	Vector3d direction = getRandomNormalizeVector();
	Ray ray(scene->light()->at(lightNum)->centre, direction, 1);
	Color color = scene->light()->at(lightNum)->material.color;
	photonTrace(ray, color, 0);
	photonCount += 1;
}

Vector3d RTEngine::getRandomNormalizeVector() {
	double u = rand() % 9999 / (double)10000;
	double v = rand() % 9999 / (double)10000;
	double theta = 2 * 3.1415926535*u;
	double phi = acos(2 * v - 1);
	double x = sin(theta)*sin(phi);
	double y = cos(theta)*sin(phi);
	double z = cos(phi);
	Vector3d vec(x, y, z);
	vec.normalize();
	return vec;
}

void RTEngine::photonTrace(Ray ray, Color& photonColor, int depth) {
	if (depth >= 5) return;
	double collideDistance = distanceINF;
	Primitive* collidePrimitive = NULL;
	int result = getIntersectPri(ray, collidePrimitive, collideDistance);
	if (result == 0)return;
	Point p = ray.origin + ray.direction*collideDistance;
	Direction N = collidePrimitive->getNormal(p);
	Direction R = ray.direction - N * ray.direction.dot(N)*2.0;
	tree.rangeSearch(iniRadius, p, photonColor, scene->size());
	Color surfaceColor = collidePrimitive->material.color;
	Color newPhotonColor(surfaceColor[0] * photonColor[0], surfaceColor[1] * photonColor[1], surfaceColor[2] * photonColor[2]);
	double possibility = rand()%9999 / double(10000);
	if (possibility < collidePrimitive->material.refl)
		photonTrace(Ray(getOrigin(p, R), R, ray.nrefr), newPhotonColor,depth+1);
	else if (possibility < collidePrimitive->material.refl + collidePrimitive->material.refr) {
		double nrefr;
		if (result == 1)nrefr = collidePrimitive->material.nrefr;
		else nrefr = 1;
		Direction newN = N * (double)result;
		double cosI = -newN.dot(ray.direction);
		double n = ray.nrefr / nrefr;
		double cosT2 = 1.0 - n * n*(1.0 - cosI * cosI);
		if (cosT2 > distanceDelta) {
			Direction T = n * ray.direction + (n*cosI - sqrt(cosT2))*newN;;
			photonTrace(Ray(getOrigin(p, T), T, nrefr), newPhotonColor,depth+1);
		}
	}
	else if (possibility < collidePrimitive->material.refl+collidePrimitive->material.refr+collidePrimitive->material.diff) {
		Vector3d dir = getRandomNormalizeVector();
		if (dir.dot(N) < 0) dir = -dir;
		photonTrace(Ray(getOrigin(p, dir), dir, ray.nrefr), newPhotonColor, depth+1);
	}
}