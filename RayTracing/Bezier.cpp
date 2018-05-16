#include "Bezier.h"

Point::Point():x(-1),y(-1){}

Point::Point(double x,double y):x(x),y(y){}

Bezier::Bezier(Point* p, int pNum) {
	_p = new Point[pNum];
	for (int i = 0; i < pNum; ++i)
		_p[i] = p[i];
	_pNum = pNum;
}

Point Bezier::getPoint(double t) {
	Point* p1 = new Point[_pNum];
	for (int i = 0; i < _pNum; ++i)
		p1[i] = _p[i];
	int pNum = _pNum-1;
	while (pNum > 0) {
		Point* p2 = new Point[pNum];
		for (int i = 0; i < pNum; ++i) {
			p2[i].x = (1 - t)*p1[i].x + t * p1[i + 1].x;
			p2[i].y = (1 - t)*p1[i].y + t * p1[i + 1].y;
		}
		delete[] p1;
		p1 = p2;
		pNum -= 1;
	}
	return p1[0];
}