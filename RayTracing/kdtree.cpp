#include "kdtree.h"
#include <iostream>

Node::Node() :photon(NULL), p(NULL), isLC(false) {
	ch[0] = NULL;
	ch[1] = NULL;
}

Node::Node(Photon* photon) : photon(photon), p(NULL), isLC(false) {
	ch[0] = NULL;
	ch[1] = NULL;
}

KDTree::KDTree(): root(NULL) {}

KDTree::~KDTree() {
	for (int i = 0; i < nodes.size(); ++i)
		if (nodes[i]) delete nodes[i];
}

void Node::addChild(Node* child, int LR) {
	if (child == NULL) return;
	child->p = this;
	ch[LR] = child;
	if (LR == 0) child->isLC = true;
	else child->isLC = false;
}

void KDTree::addNode(Photon* photon) {
	Node* node = new Node(photon);
	nodes.push_back(node);
}

void KDTree::build() {
	int count = nodes.size();
	if (count == 0) return;
	else if (count == 1) {
		root = nodes[0];
		return;
	}
	else root = split(nodes.begin(), nodes.end(),0);
}

Node* KDTree::split(vector<Node*>::iterator begin, vector<Node*>::iterator end, int index) {
	if (end - begin == 0) return NULL;
	else if (end - begin == 1) {
		cout << (*begin)->photon->pos[0] << (*begin)->photon->pos[1] << (*begin)->photon->pos[2] << endl;
		return *begin;
	}
	Compare cmp(index);
	nth_element(begin, begin + (end - begin) / 2, end, cmp);
	Node* subRoot = *(begin + (end - begin) / 2);
	cout << subRoot->photon->pos[0] << subRoot->photon->pos[1] << subRoot->photon->pos[2] << endl;
	subRoot->addChild(split(begin, begin + (end - begin) / 2, (index + 1) % 3), 0);
	subRoot->addChild(split(begin + (end - begin) / 2 + 1, end, (index + 1) % 3), 1);
	return subRoot;
}