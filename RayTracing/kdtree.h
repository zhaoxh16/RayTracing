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
	Vector3d a, b;//kdtree�����½�

	//��[start,end)���һ��kdtree�������ظ��ڵ�
	Node* split(vector<Node*>::iterator start, vector<Node*>::iterator end, int index);

	//����nodeΪ���ڵ��������������node��ռ����Ϊ[L,R],������������Ϊ[lightL,lightR]
	void m_rangeSearch(vector<Node*>& result, Node* node, Vector3d L, Vector3d R, Vector3d lightL, Vector3d lightR, Vector3d pos);

	//�ж�[L,R]��[lightL,lightR]�Ĺ�ϵ�����[L,R]��[lightL, lightR]���򷵻�1�� ����ཻ���������򷵻�2��������ཻ�򷵻�0
	int rangeRelation(Vector3d L, Vector3d R, Vector3d lightL, Vector3d lightR);

	//��node���������б������������ܹ����չ��ӵĵ�ȫ������result��
	void travel(vector<Node*>& result, Node* node, Vector3d pos);

	double distance(Vector3d a, Vector3d b) {
		return sqrt((a - b).dot(a - b));
	}

};