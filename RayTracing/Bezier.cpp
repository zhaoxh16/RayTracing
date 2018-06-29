#include "Bezier.h"

int Box::intersect(const Ray& ray, double& ta, double& tb) {
	double t0 = 0;
	double t1 = 100000;
	for (int i = 0; i < 3; ++i) {
		double invDir = 1.0 / ray.direction[i];
		double tn = (minPoint[i] - ray.origin[i])*invDir;
		double tf = (maxPoint[i] - ray.origin[i])*invDir;
		if (tn > tf) swap(tn, tf);
		t0 = tn > t0 ? tn : t0;
		t1 = tf < t1 ? tf : t1;
		if (t0 > t1)return 0;
	}
	ta = t0;
	tb = t1;
	if (t0 == 0) {
		return -1;
	}
	else {
		return 1;
	}
}

Bezier::Bezier(vector<Vector3d> p, double axisX, double axisZ, Material material) {
	vector<double> deltaX;
	cout << p.size() << endl;
	for (int i = 0; i < p.size(); ++i) {
		deltaX.push_back(-p[i].x());
	}
	_p = new Vector3d[p.size()];
	for (int i = 0; i < p.size(); ++i)
		_p[i] = p[i];
	_pNum = p.size();
	this->axisX = axisX;
	this->axisZ = axisZ;
	this->material = material;
	
	//建立包围盒
	double maxDeltaX = -100000;
	double minY = 100000;
	double maxY = -100000;
	for (int i = 0; i < p.size(); ++i) {
		if (deltaX[i] > maxDeltaX) maxDeltaX = deltaX[i];
		if (p[i].y() > maxY)maxY = p[i].y();
		if (p[i].y() < minY)minY = p[i].y();
	}
	box.minPoint = Vector3d(axisX - maxDeltaX, minY, axisZ - maxDeltaX);
	box.maxPoint = Vector3d(axisX + maxDeltaX, maxY, axisZ + maxDeltaX);

}

//Vector3d Bezier::getPoint(double t, double theta) {
//	Vector3d* p1 = new Vector3d[_pNum];
//	for (int i = 0; i < _pNum; ++i)
//		p1[i] = _p[i];
//	int pNum = _pNum-1;
//	while (pNum > 0) {
//		Vector3d* p2 = new Vector3d[pNum];
//		for (int i = 0; i < pNum; ++i) {
//			p2[i].x() = (1 - t)*p1[i].x() + t * p1[i + 1].x();
//			p2[i].y() = (1 - t)*p1[i].y() + t * p1[i + 1].y();
//			p2[i].z() = p1[i].z();
//		}		
//		delete[] p1;
//		p1 = p2;
//		pNum -= 1;
//	}
//	Vector3d p = p1[0];
//	delete p1;
//
//	Vector3d pRotate(p.x()*cos(theta), p.y(), p.x()*sin(theta));
//	pRotate.x() += axisX;
//	pRotate.z() += axisZ;
//	return pRotate;
//}

bool Bezier::iteration(double t, double u, double v, Vector3d& result, Vector3d& normal, const Ray& ray) {
	bool flag = 0;
	Matrix3d m;
	Vector3d fx(10000, 10000, 10000);
	int pNum = _pNum;
	for (int i = 0; i < 50; ++i) {
		/*cout << "fx: " << fx[0] << ' ' << fx[1] << ' ' << fx[2] << endl;*/
		if (u < -0.02 || u>1.02 || t < 0)break;
		if (v < 0)v += 2 * 3.1415926535;
		if (v > 2 * 3.1415926535) v -= 2 * 3.1415926535;
		if (v < 0 || v>2 * 3.1415926535)break;
		if (sqrt(fx.dot(fx)) < 0.001) {;
			flag = 1;
			break;
		}
		double** bernstein = new double*[pNum];
		for (int i = 0; i < pNum; ++i) {
			bernstein[i] = new double[pNum];
		}
		bernstein[0][0] = 1;
		for (int i = 1; i < pNum; ++i) {
			for (int j = 0; j <= i; ++j) {
				if (j == 0) bernstein[j][i] = (1 - u)*bernstein[j][i - 1];
				else if (j < i)bernstein[j][i] = (1 - u)*bernstein[j][i - 1] + u * bernstein[j - 1][i - 1];
				else bernstein[j][i] = u * bernstein[j - 1][i - 1];
			}
		}
		Vector3d bezierPlanePoint(0, 0, 0);
		for (int i = 0; i < pNum; ++i) {
			bezierPlanePoint += _p[i] * bernstein[i][pNum - 1];
		}
		/*Vector3d bezierPlanePoint = _p[0] * bernstein[0][3] + _p[1] * bernstein[1][3] + _p[2] * bernstein[2][3] + _p[3] * bernstein[3][3];*/
		Vector3d bezierPoint(bezierPlanePoint[0] * cos(v) + axisX, bezierPlanePoint[1], bezierPlanePoint[0]*sin(v) + axisZ);
		Vector3d diffu(0, 0, 0);
		diffu += _p[0] * (-bernstein[0][pNum - 2]);
		diffu += _p[pNum - 1] * bernstein[pNum - 2][pNum - 2];
		for (int i = 1; i < pNum - 1; ++i) {
			diffu += _p[i] * (bernstein[i - 1][pNum - 2] - bernstein[i][pNum - 2]);
		}
		diffu *= (pNum - 1);
	/*	Vector3d diffu = 3 * (_p[0] * (-bernstein[0][2]) + _p[1] * (bernstein[0][2] - bernstein[1][2]) + _p[2] * (bernstein[1][2] - bernstein[2][2]) + _p[3] * bernstein[2][2]);*/
		m(0, 0) = ray.direction[0];
		m(1, 0) = ray.direction[1];
		m(2, 0) = ray.direction[2];
		m(0, 1) = -cos(v)*diffu[0];
		m(1, 1) = -diffu[1];
		m(2, 1) = -sin(v)*diffu[0];
		m(0, 2) = sin(v)*bezierPlanePoint[0];
		m(1, 2) = 0;
		m(2, 2) = -cos(v)*bezierPlanePoint[0];
		Matrix3d n = m.inverse();
		m = n;
		fx = ray.origin + ray.direction*t - bezierPoint;
		Vector3d x(t, u, v);
		x = x - m * fx;
		t = x[0];
		u = x[1];
		v = x[2];
		for (int i = 0; i < pNum; ++i) delete[] bernstein[i];
		delete[] bernstein;
	}
	if (flag == 0) return false;
	result = Vector3d(t, u, v);
	Vector3d diffpu(-m(0, 1), -m(1, 1), -m(2, 1));
	Vector3d diffpv(-m(0, 2), -m(1, 2), -m(2, 2));
	normal = -diffpu.cross(diffpv);
	normal.normalize();
	return true;
}

int Bezier::intersect(const Ray& ray, double& distance) {
	//首先求与包围盒是否有交点
	distance = 100000;
	double ta;
	double tb;
	int intersectWithBox = box.intersect(ray, ta, tb);
	//cout << ray.origin << ray.direction << endl;
	//cout << "intersectWithBox: " << intersectWithBox << endl;
	//cout << "ta: " << ta << " tb: " << tb << endl;
	if (intersectWithBox == 0) return 0;

	Vector3d intersectionPoint(0,0,0);//最终的t,u,v
	Vector3d intersectionNormal(0, 0, 0);
	bool flag = false;//是否有交点
	double constantt;
	if(ta == 0) constantt = tb;
	else constantt = ta;
	Vector3d intersectionWithBoxPoint = ray.origin + ray.direction*constantt;
	double constantv;
	Vector3d path = intersectionWithBoxPoint - Vector3d(axisX, 0, axisZ);
	path = path - Vector3d(0,path.dot(Vector3d(0, 1, 0)),0);
	path.normalize();
	constantv = acos(-path[0]);
	if (path[2] > 0) constantv = 2 * acos(-1) - constantv;
	//随机选取初始点进行迭代
	for (int i = 0; i < 60; ++i) {
		double u = randomNumber(1);
		double v = randomNumber(1) * 2 * 3.1415926535;
		double t = constantt + (randomNumber(1) - 1.0 / 2.0) * 300;
		Vector3d result(0, 0, 0);
		Vector3d normal(0, 0, 0);
		if (iteration(t, u, v, result, normal, ray)) {
		/*	cout << "result: "<<result[0] << ' ' << result[1] << ' ' << result[2] << endl;*/
			flag = true;
			if (result[0] < distance) {
				distance = result[0];
				intersectionPoint = result;
				intersectionNormal = normal;
			}
		}
	}
	if (flag) {
		intersectionT = intersectionPoint[1];
		intersectionTheta = intersectionPoint[2];
		this->intersectionNormal = intersectionNormal;
		if (ray.direction.dot(intersectionNormal) > 0)return -1;
		return 1;
	}
	else return 0;
}

Direction Bezier::getNormal(const Point& p) {
	return intersectionNormal;
}