#pragma once
#include "Photon.h"
#include <vector>
#include <algorithm>
#include "hitpoint.h"

using namespace std;

const double iniRadius = 80;

struct Node {
	Node *p, *lc, *rc;
	int index;
	double radius = iniRadius;
	HitPoint* point;
	Node(HitPoint* point) :point(point), p(NULL), lc(NULL), rc(NULL){}
	void setIndex(int index) { this->index = index; }
	void update() {
		double a, b, c;
		a = point->r;
		b = 0;
		c = 0;
		if (lc != NULL) b = lc->radius;
		if (rc != NULL) c = rc->radius;
		if (max((b, c), a) < radius) {
			radius = max((b, c), a);
			if (p != NULL) p->update();
		}
	}
};

class Compare {
public:
	Compare(int index):index(index){}
	bool operator() (Node* a, Node* b) {
		if (a->point->pos[index] < b->point->pos[index]) return true;
		return false;
	}

private:
	int index;
};

class KDTree {
public:
	KDTree();
	void addNode(Node* node);
	void build();
	vector<Node*> rangeSearch(Vector3d pos, double radius);
	void setSize(Vector3d a, Vector3d b);
	vector<Node*> nodes;

private:
	Node * root;
	Vector3d a, b;//kdtree的上下界

	//将[start,end)变成一个kdtree，并返回根节点
	Node* split(vector<Node*>::iterator start, vector<Node*>::iterator end, int index);

	//对以node为根节点的树进行搜索，node所占区域为[L,R],光子搜索区域为[lightL,lightR]
	void m_rangeSearch(vector<Node*>& result, Node* node, Vector3d L, Vector3d R, Vector3d lightL, Vector3d lightR, Vector3d pos);

	//判断[L,R]与[lightL,lightR]的关系，如果[L,R]在[lightL, lightR]中则返回1， 如果相交但不包含则返回2，如果不相交则返回0
	int rangeRelation(Vector3d L, Vector3d R, Vector3d lightL, Vector3d lightR);

	//对node的子树进行遍历，将所有能够接收光子的点全部加入result中
	void travel(vector<Node*>& result, Node* node, Vector3d pos);

	double distance(Vector3d a, Vector3d b) {
		return sqrt((a - b).dot(a - b));
	}

};