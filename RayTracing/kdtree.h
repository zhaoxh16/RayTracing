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
	int index;//�����ĸ�������еĻ���
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
	void m_rangeSearch(Node* node,Vector3d l, Vector3d r, Vector3d& pos, Vector3d& a, Vector3d& b, Color& photonColor);//������������������������
	void travel(Node* node, Vector3d& pos, Color& photonColor);//������nodeΪ�����������������ӵ�������ֵ��ÿ���ڵ�

	int examine(Vector3d l, Vector3d r, Vector3d a, Vector3d b);//�����l��rΪ����ĳ������붥��Ϊa��b�ĳ�����Ĺ�ϵ��0Ϊ���룬1Ϊ�ཻ��2Ϊ���ڲ�

	//���������ľ���
	double getDistance2(Vector3d a, Vector3d b) {
		Vector3d c = a - b;
		return c.dot(c);
	}

	//���������ɫ�ĵ���
	Color colorMultiply(Color& a, Color& b) {
		return Color(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
	}
};