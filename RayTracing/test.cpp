#include "Primitive.h"
#include <iostream>
#include <Eigen/Dense>
#include "test.h"

using namespace std;
using namespace Eigen;

int main() {
	Primitive** myPri = new Primitive*[33];
	Light** light = new Light*[2];
	Vector3d size(800, 600, 2000);
	myPri[4] = new Sphere(Point(200, 200, 300), 100, Material(Vector3d(240, 32, 160), 0.3, 0.6, 0.5,0.8,1.33));
	myPri[5] = new Sphere(Point(500, 100, 300), 100, Material(Vector3d(87, 201, 0), 0.3, 0.6, 0.5,0.8,1.33));
	myPri[6] = new Sphere(Point(450, 300, 400), 150, Material(Vector3d(230, 224, 176), 0.3, 0.6, 0.5,0.8,1.33));
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			myPri[7 + i + 5 * j] = new Sphere(Point(300 + 60 * i, 150 + 60 * j, 600), 10, Material(Vector3d(87, 201, 0), 0, 1.0, 0, 0, 1.33));
		}
	}
	light[0] = new Light(Point(150, 550, 500), 100, Material(Vector3d(125,125,125),0.3,0.5, 0.1,0,0));
	//light[0] = new Light(Point(400, 590, 100), 100, Material());
	Material m(Vector3d(139, 104, 0), 0.3, 0.7, 0.5,0,0);
	myPri[3] = new Plane(Direction(0, 1, 0), -size.y(), m);
	myPri[2] = new Plane(Direction(-1, 0, 0), 0, m);
	myPri[1] = new Plane(Direction(1, 0, 0), -size.x(), m);
	myPri[0] = new Plane(Direction(0, -1, 0), 0, m);
	myPri[32] = new Plane(Direction(0, 0, 1), -800, m);


	Point see(400, 250, -500);
	test(myPri, light, size, see, 33, 1);
	system("pause");
	return 0;
}