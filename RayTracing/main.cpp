#include "Primitive.h"
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "RTEngine.h"
#include "Bezier.h"

using namespace std;
using namespace Eigen;

int main() {
	Scene* scene = new Scene();
	scene->setSize(Vector3d(800, 600, 700));
	scene->setCamera(Vector3d(400, 250, -500));
	scene->camera()->setFocalPlane(300);
	scene->camera()->setRadius(30);
	Material m(Vector3d(139, 104, 0), 0.1, 0.9, 0.5, 0, 0);
	Material lightMaterial(Vector3d(2550000000, 2550000000, 2550000000), 0, 0, 0, 0, 0);
	Light* light1 = new CirclePlaneLight(Vector3d(2550000000 / 4, 2550000000 / 4, 2550000000 / 4));
	dynamic_cast<CirclePlaneLight*>(light1)->setPosition(Point(400, 590, 100), 30, Vector3d(0, -1, 0));
	scene->addL(light1);
	scene->addP(new Sphere(Point(200, 200, 300), 100, Material(Vector3d(240, 32, 160), 0.3, 0.5, 0.3, 0.2, 1.33)));
	scene->addP(new Sphere(Point(500, 100, 300), 100, Material(Vector3d(87, 201, 0), 0.3, 0.5, 0.3, 0.2, 1.33)));
	scene->addP(new Sphere(Point(450, 300, 400), 150, Material(Vector3d(230, 224, 176), 0.3, 0.7, 0.3, 0.5, 1.33)));
	//scene->addP(new Plane(Direction(0, 1, 0), -scene->size().y(), m));
	//scene->addP(new Plane(Direction(-1, 0, 0), 0, m));
	//scene->addP(new Plane(Direction(1, 0, 0), -scene->size().x(), m));
	Plane* bottomPlane = new Plane(Direction(0, -1, 0), 0, m);
	bottomPlane->setTexture("C:/Users/DELL/Desktop/leaf.jpg");
	bottomPlane->setTextureStatus(1, Vector3d(0, 0, 0), Vector3d(1, 0, 0), Vector3d(0, 0, 1));
	scene->addP(bottomPlane);
	//scene->addP(new Plane(Direction(0, 0, 1), -600, m));
	//scene->addP(new Plane(Direction(0, 0, -1), 600, m));
	vector<Vector3d> bezierPoints;
	bezierPoints.push_back(Vector3d(0, 150, 0));
	bezierPoints.push_back(Vector3d(-100, 100, 0));
	bezierPoints.push_back(Vector3d(-30, 50, 0));
	bezierPoints.push_back(Vector3d(-50, 0, 0));
	Bezier* bezier = new Bezier(bezierPoints, 300, 100, Material(Vector3d(87, 201, 0), 0.3, 0.5, 0.3, 0.2, 1));
	scene->addP(bezier);
	RTEngine engine;
	engine.setScene(scene);
	engine.emitRay();
	engine.PPMRender();
	engine.show();
	/*engine.render();
	engine.show();*/
	system("pause");
	return 0;
}