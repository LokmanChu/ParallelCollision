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
	this->isLeaf = isLeaf;
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

bool Box::inside(Particle * p) {
	return (p->position.x >= a.x && p->position.x < b.x &&  p->position.y >= a.y && p->position.y < b.y);
}

void Box::testBoxDraw() {
	ofNoFill();
	ofDrawRectangle(a.x, a.y, b.x - a.x, b.y - a.y);
	ofFill();
}

QuadTree::QuadTree() {
	QuadTree(2);
}

QuadTree::QuadTree(int levels) {
	this->root = new Box(ofVec3f(0), ofVec3f(ofGetWindowWidth(), ofGetWindowHeight(), 0));
	generateQuadTree(root, levels);
}

void QuadTree::generateQuadTree(Box * root, int levels) {
	if (levels-- == 0) return;
	divideQuadrants(root,levels == 1);
	for (int i = 0; i < 4; i++)
	{
		generateQuadTree(root->children[i],levels);
	}
}

void QuadTree::divideQuadrants(Box * parent, bool isLeaf) {
	int xMid = (parent->b.x + parent->a.x) / 2;
	int yMid = (parent->b.y + parent->a.y) / 2;
	Box * boxes[4];
	boxes[TL] = new Box(parent->a, ofVec3f(xMid, yMid), parent, TL, isLeaf, parent->edges[0] == Left || parent->edges[1] == Top);
	boxes[TR] = new Box(ofVec3f(xMid, parent->a.y), ofVec3f(parent->b.x, yMid), parent, TR, isLeaf, parent->edges[0] == Right || parent->edges[1] == Top);
	boxes[BL] = new Box(ofVec3f(parent->a.x, yMid), ofVec3f(xMid, parent->b.y), parent, BL, isLeaf, parent->edges[0] == Left || parent->edges[1] == Bottom);
	boxes[BR] = new Box(ofVec3f(xMid, yMid), parent->b, parent, BR, isLeaf, parent->edges[0] == Right || parent->edges[1] == Bottom);
	for (int i = 0; i < 4; i++) {
		parent->children.push_back(boxes[i]);
	}
	if (isLeaf) {
		leafs.push_back(boxes[TL]);
		leafs.push_back(boxes[TR]);
		leafs.push_back(boxes[BL]);
		leafs.push_back(boxes[BR]);
	}
}

void QuadTree::traversal(Box * node) {
	if (!node->isLeaf) {
		traversal(node->children[TL]);
		traversal(node->children[TR]);
	}
	else {
		leafs.push_back(node);
	}
	if (!node->isLeaf) {
		traversal(node->children[BL]);
		traversal(node->children[BR]);
	}
}

void QuadTree::linearize(int level) {
	vector<Box *> temp;
	bitset<8> bsetRow(0);
	bitset<8> bsetCol(0);
	int end = pow(2, level - 1);
	int base = 0;
	for (int i = 0; i < end; i++) {
		base = 2 * toDecimal(bsetRow);
		for (int j = 0; j < end; j++) {
			temp.push_back(leafs.at(base + toDecimal(bsetCol)));
			cout << base + toDecimal(bsetCol) << endl;
			incrementBitSet(&bsetCol);
		}
		incrementBitSet(&bsetRow);
		bsetCol.reset();
	}
	leafs.clear();
	leafs = temp;
}

void QuadTree::incrementBitSet(bitset<8> * bitset) {
	for (int i = 0; i < bitset->size(); i++)
	{
		bitset->flip(i);
		if (bitset->test(i))
		{
			break;
		}
	}
}

int QuadTree::toDecimal(bitset<8> bitset) {
	int base = 1;
	int result = 0;
	for (int i = 0; i < bitset.size(); i++) {
		if (bitset.test(i)) {
			result += base;
		}
		base *= 4;
	}
	return result;
}

void QuadTree::draw() {
	int count = 0;
	for (Box * box : leafs)
	{
		box->testBoxDraw();
		string str;
		str += std::to_string(count);
		ofSetColor(ofColor::black);
		ofDrawBitmapString(str, box->a.x + 20, box->a.y + 20);
		count++;
	}
}

/*
	Returns the adjacent Box of box from Edge e
*/
Box * adjacentBox(Box * box, Edges e) {
	switch (e) {
	case Top:
		if (box->quadrant >= 2) {
			return box->parent->children[box->quadrant - 2];
		}
		else {

		}
		
	case Bottom:
		break;
	case Left:
		break;
	case Right:
		break;
	}
}

void QuadTree::clearTree() {
	for (Box * box : leafs)
	{
		box->particles.clear();
	}
}

