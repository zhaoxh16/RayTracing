#pragma once

struct Point {
	double x, y;
	
	Point();
	Point(double x, double y);
};

class Bezier {
private:
	Point* _p;
	int _pNum;

public:
	Bezier(Point* p, int pNum);//Bezier曲线的控制顶点及其数量
	Point getPoint(double t);//获得参数为t的点
};
