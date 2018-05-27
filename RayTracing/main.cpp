#include "Primitive.h"
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "kdtree.h"
#include "RTEngine.h"

using namespace std;
using namespace Eigen;

int main() {
	Scene* scene = new Scene();
	scene->setSize(Vector3d(800, 600, 2000));
	scene->setCamera(Vector3d(400, 250, -500));
	Material m(Vector3d(139, 104, 0), 0.1, 0.9, 0.5, 0, 0);
	Material lightMaterial(Vector3d(255, 255, 255), 0, 0, 0, 0, 0);
	scene->addL(new Light(Point(150, 550, 500), 100, lightMaterial));
	scene->addL(new Light(Point(400, 590, 100), 100, lightMaterial));
	scene->addP(new Sphere(Point(200, 200, 300), 100, Material(Vector3d(240, 32, 160), 0.3, 0.7, 0.3, 0.5, 1.33)));
	scene->addP(new Sphere(Point(500, 100, 300), 100, Material(Vector3d(87, 201, 0), 0.3, 0.7, 0.3, 0.5, 1.33)));
	scene->addP(new Sphere(Point(450, 300, 400), 150, Material(Vector3d(230, 224, 176), 0.3, 0.7, 0.3, 0.5, 1.33)));
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			scene->addP(new Sphere(Point(300 + 60 * i, 150 + 60 * j, 600), 10, Material(Vector3d(87, 201, 0), 0, 1.0, 0, 0, 1.33)));
		}
	}
	scene->addP(new Plane(Direction(0, 1, 0), -scene->size().y(), m));
	scene->addP(new Plane(Direction(-1, 0, 0), 0, m));
	scene->addP(new Plane(Direction(1, 0, 0), -scene->size().x(), m));
	scene->addP(new Plane(Direction(0, -1, 0), 0, m));
	scene->addP(new Plane(Direction(0, 0, 1), -800, m));
	RTEngine engine;
	engine.setScene(scene);
	engine.render();
	engine.show();
	system("pause");
	return 0;
}