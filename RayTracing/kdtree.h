#pragma once
#include "Photon.h"
#include <vector>
#include <algorithm>

using namespace std;

class Node {
public:
	Photon * photon;
	Node* p;
	Node* ch[2];
	bool isLC;

	Node();
	Node(Photon* photon);
	void addChild(Node* child, int LR);//LRΪ0��1����ʾchild��λ��
};

class Compare {
public:
	Compare(int index):index(index){}
	bool operator() (const Node* a, const Node* b) {
		if (a->photon->pos[index] < b->photon->pos[index]) return true;
		return false;
	}

private:
	int index;
};

class KDTree {
public:
	KDTree();
	~KDTree();
	void addNode(Photon* photon);
	void build();

private:
	Node* root;
	vector<Node*> nodes;
	Node* split(vector<Node*>::iterator begin, vector<Node*>::iterator end, int index);//��[begin,end)���յ�index��������л��֣������ظ��ڵ�

};