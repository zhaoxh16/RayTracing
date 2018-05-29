#include "kdtree.h"


Node::Node() :obj(NULL), p(NULL), isLC(false) {
	ch[0] = NULL;
	ch[1] = NULL;
	radius = iniRadius;
}

Node::Node(HitPoint* obj) : obj(obj), p(NULL), isLC(false) {
	ch[0] = NULL;
	ch[1] = NULL;
	radius = iniRadius;
}

void Node::addChild(Node* child, int LR) {
	//LR为0或1，表示child的位置
	if (child == NULL) return;
	child->p = this;
	ch[LR] = child;
	if (LR == 0) child->isLC = true;
	else child->isLC = false;
}

KDTree::~KDTree() {
	for (int i = 0; i < nodes.size(); ++i)
		if (nodes[i]) delete nodes[i];
}
void KDTree::addNode(HitPoint* obj) {
	Node* node = new Node(obj);
	nodes.push_back(node);
}
void KDTree::build() {
	int count = nodes.size();
	if (count == 0) return;
	else if (count == 1) {
		root = nodes[0];
		return;
	}
	else root = split(nodes.begin(), nodes.end(), 0);
}

Node* KDTree::split(typename vector<Node*>::iterator begin, typename vector<Node*>::iterator end, int index) {
	//对[begin,end)按照第index个坐标进行划分，并返回根节点
	if (end - begin == 0) return NULL;
	else if (end - begin == 1) {
		(*begin)->index = index;
		return *begin;
	}
	Compare cmp(index);
	nth_element(begin, begin + (end - begin) / 2, end, cmp);
	Node* subRoot = *(begin + (end - begin) / 2);
	subRoot->index = index;
	subRoot->addChild(split(begin, begin + (end - begin) / 2, (index + 1) % 3), 0);
	subRoot->addChild(split(begin + (end - begin) / 2 + 1, end, (index + 1) % 3), 1);
	return subRoot;
}

void KDTree::rangeSearch(double radius, Vector3d pos, Color photonColor, Vector3d size) {
	if (root == NULL)return;
	Vector3d l(0, 0, -size.z());
	Vector3d r(size);
	Vector3d a(pos - Vector3d(radius, radius, radius));
	Vector3d b(pos + Vector3d(radius, radius, radius));
	m_rangeSearch(root, l, r, pos, a, b, photonColor);
	
}

void KDTree::m_rangeSearch(Node* node, Vector3d l, Vector3d r,Vector3d& pos, Vector3d& a, Vector3d& b, Color& photonColor) {
	int index = node->index;
	double splitPos = node->obj->pos[index];
	Vector3d r1 = r;
	Vector3d l2 = l;
	r1[index] = splitPos;
	l2[index] = splitPos;
	if (node->ch[0] != NULL) {
		int flag = examine(l, r1, a, b);
		if (flag == 1) m_rangeSearch(node->ch[0], l, r1,pos, a, b, photonColor);
		else if (flag == 2)travel(node->ch[0],pos, photonColor);
	}
	if (node->ch[1] != NULL) {
		int flag = examine(l2, r, a, b);
		if (flag == 1)m_rangeSearch(node->ch[1], l2, r,pos, a, b, photonColor);
		else if (flag == 2)travel(node->ch[1], pos, photonColor);
	}
	HitPoint* point = node->obj;
	if (getDistance2(point->pos, pos) > point->r*point->r) return;
	Vector3d L = pos - point->pos;
	L.normalize();
	double LdN = L.dot(point->N);
	double diff = 0;
	if (LdN > 0 && point->BRDF > 0)
		diff = LdN * point->BRDF;
	Color flux = colorMultiply(photonColor, point->colorWeight)*diff;
	point->update(flux);
}

void KDTree::travel(Node* node, Vector3d& pos, Color& photonColor) {
	if (node->ch[0] != NULL) travel(node->ch[0],pos, photonColor);
	if (node->ch[1] != NULL) travel(node->ch[1],pos, photonColor);
	HitPoint* point = node->obj;
	if (getDistance2(point->pos, pos) > point->r*point->r)return;
	Vector3d L = pos - point->pos;
	L.normalize();
	double LdN = L.dot(point->N);
	double diff = 0;
	if (LdN > 0 && point->BRDF > 0)
		diff = LdN * point->BRDF;
	Color flux = colorMultiply(photonColor, point->colorWeight)*diff;
	point->update(flux);
}

int KDTree::examine(Vector3d l, Vector3d r, Vector3d a, Vector3d b) {
	if (r[0] < a[0] || r[1] < a[1] || r[2] < a[2]) return 0;
	if (l[0] > b[0] || l[1] > b[1] || l[2] > b[2]) return 0;
	if (l[0] >= a[0] && l[1] >= a[1] && l[2] >= a[2] && r[0] <= b[0] && r[1] <= b[1] && r[2] <= b[2])return 2;
	return 1;
}