#pragma once
#include "Photon.h"
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Node {
public:
	T* obj;
	Node<T>* p;
	Node<T>* ch[2];
	bool isLC;

	Node() :obj(NULL), p(NULL), isLC(false) {
		ch[0] = NULL;
		ch[1] = NULL;
	}

	Node(T* obj) : obj(obj), p(NULL), isLC(false) {
		ch[0] = NULL;
		ch[1] = NULL;
	}

	void addChild(Node<T>* child, int LR) {
		//LR为0或1，表示child的位置
		if (child == NULL) return;
		child->p = this;
		ch[LR] = child;
		if (LR == 0) child->isLC = true;
		else child->isLC = false;
	}
};

template<typename T>
class Compare {
public:
	Compare(int index):index(index){}
	bool operator() (const Node<T>* a, const Node<T>* b) {
		if (a->obj->pos[index] < b->obj->pos[index]) return true;
		return false;
	}

private:
	int index;
};

template<typename T>
class KDTree {
public:
	KDTree():root(NULL){}
	~KDTree() {
		for (int i = 0; i < nodes.size(); ++i)
			if (nodes[i]) delete nodes[i];
	}
	void addNode(T* obj) {
		Node<T>* node = new Node<T>(obj);
		nodes.push_back(node);
	}
	void build() {
		int count = nodes.size();
		if (count == 0) return;
		else if (count == 1) {
			root = nodes[0];
			return;
		}
		else root = split(nodes.begin(), nodes.end(), 0);
	}

private:
	Node<T>* root;
	vector<Node<T>*> nodes;
	Node<T>* split(typename vector<Node<T>*>::iterator begin, typename vector<Node<T>*>::iterator end, int index) {
		//对[begin,end)按照第index个坐标进行划分，并返回根节点
		if (end - begin == 0) return NULL;
		else if (end - begin == 1) {
			cout << (*begin)->obj->pos[0] << (*begin)->obj->pos[1] << (*begin)->obj->pos[2] << endl;
			return *begin;
		}
		Compare cmp(index);
		nth_element(begin, begin + (end - begin) / 2, end, cmp);
		Node* subRoot = *(begin + (end - begin) / 2);
		cout << subRoot->obj->pos[0] << subRoot->obj->pos[1] << subRoot->obj->pos[2] << endl;
		subRoot->addChild(split(begin, begin + (end - begin) / 2, (index + 1) % 3), 0);
		subRoot->addChild(split(begin + (end - begin) / 2 + 1, end, (index + 1) % 3), 1);
		return subRoot;
	}

};