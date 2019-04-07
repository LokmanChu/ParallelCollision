#include "QuadTree.h"

Box::Box(ofVec3f a, ofVec3f b) {
	this->a = a;
	this->b = b;
	omp_init_lock(&writelock);
	findEdges();
	isEdge = edges[0] || edges[1];
}

/*
	While you could a and b from parent Box and Quadrant, it is less operations to find points a and b 
	from the parent than to do the calculations for every child box
*/
Box::Box(ofVec3f a, ofVec3f b, Box * parent, Quadrant q, bool isLeaf, bool isEdge) {
	this->a = a;
	this->b = b;
	this->parent = parent; 
	this->quadrant = q;
	if (isLeaf) {
		omp_init_lock(&writelock);
	}
	this->isEdge = isEdge;
	if (isEdge) {
		findEdges();
	}
}

void Box::addParticle(Particle * p) {
	omp_set_lock(&writelock);
	particles.push_back(p);
	omp_unset_lock(&writelock);
}

void Box::findEdges() {
	if (a.x == 0 || b.x == ofGetWindowWidth()) {
		edges[0] = a.x == 0 ? Left : Right;
	}
	if (a.y == 0 || b.y == ofGetWindowHeight()) {
		edges[1] = a.y == 0 ? Top : Bottom;
	}
}

void Box::testBoxDraw() {
	ofNoFill();
	ofDrawRectangle(ofRectangle(a, b));
	ofFill();
}

QuadTree::QuadTree() {
	QuadTree(2);
}

QuadTree::QuadTree(int levels) {
	this->root = new Box(ofVec3f(0), ofVec3f(ofGetScreenWidth(), ofGetWindowHeight(), 0));
	divideQuadrants(this->root, this->children,levels);
}

void QuadTree::generateQuadTree(Box * root, int levels) {
	if (levels == 0) return;
	divideQuadrants(root, root->children,levels == 1);
	for (int i = 0; i < 4; i++)
	{
		generateQuadTree(root->children[i], levels - 1);
	}
}

void QuadTree::divideQuadrants(Box * parent, Box * boxes[], bool isLeaf) {
	int xMid = parent->b.x / 2;
	int yMid = parent->b.y / 2;
	boxes[TL] = new Box(parent->a, ofVec3f(xMid, yMid), parent, TL, isLeaf, parent->edges[0] == Left || parent->edges[1] == Top);
	boxes[TR] = new Box(ofVec3f(xMid, parent->a.y), ofVec3f(parent->b.x, yMid), parent, TR, isLeaf, parent->edges[0] == Right || parent->edges[1] == Top);
	boxes[BL] = new Box(ofVec3f(parent->a.x, yMid), ofVec3f(xMid, parent->b.y), parent, BL, isLeaf, parent->edges[0] == Left || parent->edges[1] == Bottom);
	boxes[BR] = new Box(ofVec3f(xMid, yMid), parent->b, parent, BR, isLeaf, parent->edges[0] == Right || parent->edges[1] == Bottom);
	
	if (isLeaf) {
		leafs.push_back(boxes[TL]);
		leafs.push_back(boxes[TR]);
		leafs.push_back(boxes[BL]);
		leafs.push_back(boxes[BR]);
	}
}

void QuadTree::draw() {
	for (Box * box : leafs)
	{
		box->testBoxDraw();
	}
}

void QuadTree::traversal(Box * node) {
	
}

