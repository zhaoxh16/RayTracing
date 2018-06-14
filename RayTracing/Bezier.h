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
	Bezier(Point* p, int pNum);//Bezier���ߵĿ��ƶ��㼰������
	Point getPoint(double t);//��ò���Ϊt�ĵ�
};
