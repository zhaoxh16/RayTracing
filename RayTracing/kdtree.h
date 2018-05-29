#pragma once
#include "Photon.h"
#include <vector>
#include <algorithm>
#include "hitpoint.h"

using namespace std;

const double iniRadius = 1;

class Node {
public:
	HitPoint* obj;
	Node* p;
	Node* ch[2];
	double radius;
	int index;//根据哪个坐标进行的划分
	bool isLC;

	Node();
	Node(HitPoint* obj);
	void addChild(Node* child, int LR);
};

class Compare {
public:
	Compare(int index):index(index){}
	bool operator() (const Node* a, const Node* b) {
		if (a->obj->pos[index] < b->obj->pos[index]) return true;
		return false;
	}

private:
	int index;
};

class KDTree {
public:
	KDTree():root(NULL){}
	~KDTree();
	void addNode(HitPoint* obj);
	void build();
	void rangeSearch(double radius, Vector3d pos, Color photonColor, Vector3d size);
	vector<Node*>& getNodes() { return nodes; }
	vector<Node*> nodes;

private:
	Node* root;
	Node* split(typename vector<Node*>::iterator begin, typename vector<Node*>::iterator end, int index);
	void m_rangeSearch(Node* node,Vector3d l, Vector3d r, Vector3d& pos, Vector3d& a, Vector3d& b, Color& photonColor);//在区域内搜索，并赋予能量
	void travel(Node* node, Vector3d& pos, Color& photonColor);//遍历以node为根的子树，并将光子的能量赋值给每个节点

	int examine(Vector3d l, Vector3d r, Vector3d a, Vector3d b);//检测以l，r为顶点的长方体与顶点为a，b的长方体的关系，0为相离，1为相交，2为在内部

	//获得两个点的距离
	double getDistance2(Vector3d a, Vector3d b) {
		Vector3d c = a - b;
		return c.dot(c);
	}

	//获得两个颜色的叠加
	Color colorMultiply(Color& a, Color& b) {
		return Color(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
	}
};