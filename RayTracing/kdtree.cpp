#include "kdtree.h"

KDTree::KDTree(): root(NULL){}

void KDTree::addNode(Node* node) {
	nodes.push_back(node);
}

void KDTree::build() {
	if (nodes.size() == 0) return;
	root = split(nodes.begin(), nodes.end(), 0);
}

Node* KDTree::split(vector<Node*>::iterator start, vector<Node*>::iterator end, int index) {
	int size = end - start;
	if (size == 0)return NULL;
	Compare myCompare(index);
	nth_element(start, start + size / 2, end, myCompare);
	Node* subRoot = *(start + size / 2);
	subRoot->index = index;
	subRoot->lc = split(start, start + size / 2, (index + 1) % 3);
	subRoot->rc = split(start + size / 2 + 1, end, (index + 1) % 3);
	if (subRoot->lc != NULL) subRoot->lc->p = subRoot;
	if (subRoot->rc != NULL) subRoot->rc->p = subRoot;
	return subRoot;
}

void KDTree::setSize(Vector3d a, Vector3d b) {
	this->a = a;
	this->b = b;
}

vector<Node*> KDTree::rangeSearch(Vector3d pos, double radius) {
	vector<Node*> result;
	if (root == NULL) return result;
	Vector3d lightL(pos - Vector3d(1.0, 1.0, 1.0)*radius);
	Vector3d lightR(pos + Vector3d(1.0, 1.0, 1.0)*radius);
	m_rangeSearch(result, root, a, b, lightL, lightR, pos);
	return result;
}

void KDTree::m_rangeSearch(vector<Node*>& result, Node* node, Vector3d L, Vector3d R, Vector3d lightL, Vector3d lightR, Vector3d pos) {
	if (node == NULL) return;
	int relation = rangeRelation(L, R, lightL, lightR);
	if (relation == 0) return;
	/*else if (relation == 1) {
		travel(result, node, pos);
		return;
	}*/
	//如果两个长方体相交
	//首先检测该节点上的HitPoint是否能够接收
	if (distance(node->point->pos, pos) < node->point->r) 
		result.push_back(node);
	Vector3d L2 = L;
	Vector3d R2 = R;
	L2[node->index] = node->point->pos[node->index];
	R2[node->index] = node->point->pos[node->index];
	m_rangeSearch(result, node->lc, L, R2, lightL, lightR, pos);
	m_rangeSearch(result, node->rc, L2, R, lightL, lightR, pos);
}

int KDTree::rangeRelation(Vector3d L, Vector3d R, Vector3d lightL, Vector3d lightR) {
	if (L[0] >= lightL[0] && L[1] >= lightL[1] && L[2] >= lightL[2] && R[0] <= lightR[0] && R[1] <= lightR[1] && R[2] <= lightR[2]) return 1;
	if (L[0] > lightR[0] || L[1] > lightR[1] || L[2] > lightR[2] || R[0] < lightL[0] || R[1] < lightL[1] || R[2] < lightL[2]) return 0;
	return 2;
}