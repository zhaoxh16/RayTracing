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
	Material m(Vector3d(139, 104, 0), 0, 1, 0, 0, 0);
	Material lightMaterial(Vector3d(2550000000, 2550000000, 2550000000), 0, 0, 0, 0, 0);
	Light* light1 = new CirclePlaneLight(Vector3d(255000000, 255000000, 255000000));
	dynamic_cast<CirclePlaneLight*>(light1)->setPosition(Point(400, 590, 0), 50, Vector3d(0, -1, 0));
	scene->addL(light1);
	//scene->addP(new Sphere(Point(200, 200, 300), 100, Material(Vector3d(240, 32, 160), 0.3, 0.5, 0.3, 0.2, 1.33)));
	//scene->addP(new Sphere(Point(500, 100, 300), 100, Material(Vector3d(87, 201, 0), 0.3, 0.5, 0.3, 0.2, 1.33)));
	scene->addP(new Sphere(Point(600, 100, 300), 100, Material(Vector3d(230, 224, 176), 0.2, 0.3, 0.3, 0.5, 1.33)));
	Plane* bottomPlane = new Plane(Direction(0, -1, 0), 0, m);
	bottomPlane->setTexture("D:/OneDrive/Tsinghua/2018Spring/计算机图形学/地板.jpg");
	bottomPlane->setTextureStatus(1, Vector3d(0, 0, 0), Vector3d(1, 0, 0), Vector3d(0, 0, 1));
	scene->addP(bottomPlane);
	Material m2(Vector3d(154,255,154), 0, 1, 0, 0, 0);
	Material m3(Vector3d(255, 255, 255), 0, 1, 0, 0, 0);
	Material m4(Vector3d(255, 255, 255), 0, 1, 0, 0, 0);
	scene->addP(new Plane(Direction(0, 0, 1), -600, m4));
	scene->addP(new Plane(Direction(0, 1, 0), -600, m3));
	scene->addP(new Plane(Direction(1, 0, 0), -800, m));
	scene->addP(new Plane(Direction(-1, 0, 0), 0, m2));
	scene->addP(new Plane(Direction(0, 0, -1), 600, m4));
	vector<Vector3d> bezierPoints;
	bezierPoints.push_back(Vector3d(-0.001, 94.74+102.9, 0)*2);
	bezierPoints.push_back(Vector3d(-15, 94.74 + 102.9, 0) * 2);
	bezierPoints.push_back(Vector3d(-21.8, 94.74 + 102.9, 0) * 2);
	bezierPoints.push_back(Vector3d(-37.6, 94.74 + 102.9, 0) * 2);
	Bezier* bezier = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-37.6, 94.74 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-33.2, 85.6 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-24.6, 74.7 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-22.1, 65.4 + 102.9, 0) * 2;
	Bezier* bezier2 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-22.14, 65.4 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-19.6, 56.4 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-18.9, 43.3 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-18.9, 33.6 + 102.9, 0) * 2;
	Bezier* bezier3 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-18.9, 33.6 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-18.8, 23.5 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-25.7, 7.3 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-35.6, -1.9 + 102.9, 0) * 2;
	Bezier* bezier4 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-35.6, -1.9 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-45.9, -11.3 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-49.6, -11.1 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-59, -25.3 + 102.9, 0) * 2;
	Bezier* bezier5 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-59, -25.3 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-66, -36.1 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-68.7, -48.7 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-65.5, -60.7 + 102.9, 0) * 2;
	Bezier* bezier6 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-65.5, -60.7 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-60.1, -79.2 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-61.7, -81.3 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-45.2, -92.4 + 102.9, 0) * 2;
	Bezier* bezier7 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0.3, 0, 1));
	bezierPoints[0] = Vector3d(-45.2, -92.4 + 102.9, 0) * 2;
	bezierPoints[1] = Vector3d(-29.4, -102.6 + 102.9, 0) * 2;
	bezierPoints[2] = Vector3d(-10.5, -102.2 + 102.9, 0) * 2;
	bezierPoints[3] = Vector3d(-0.01, -102.9 + 102.9, 0) * 2;
	Bezier* bezier8 = new Bezier(bezierPoints, 300, 300, Material(Vector3d(255, 0, 255), 0.2, 0.8, 0, 0, 1));
	scene->addP(bezier);
	scene->addP(bezier2);
	scene->addP(bezier3);
	scene->addP(bezier4);
	scene->addP(bezier5);
	scene->addP(bezier6);
	scene->addP(bezier7);
	scene->addP(bezier8);
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