#include "QuadTree.h"

Box::Box(ofVec3f a, ofVec3f b) {
	this->a = a;
	this->b = b;
	findEdges();
	isEdge = edges.any();
	isLeaf = false;
	boxIndex = -1;
}

/*
	While you could a and b from parent Box and Quadrant, it is less operations to find points a and b 
	from the parent than to do the calculations for every child box
*/
Box::Box(ofVec3f a, ofVec3f b, bool isLeaf, bool isEdge) {
	this->a = a;
	this->b = b;
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
	if (a.x == 0) {
		edges.set(Left);
	}
	if (b.x == ofGetWindowWidth()) {
		edges.set(Right);
	}
	if (a.y == 0) {
		edges.set(Top);
	}
	if (b.y == ofGetWindowHeight()) {
		edges.set(Bottom);
	}
}

/*
	inside box is x >= min && x < max
	bitset return: 0001 top, 0010 bottom, 0100 left, 1000 right
*/
bitset<4> Box::overlapEdge(Particle * p) {
	bitset<4> bset(0);
	if (p->position.x - p->radius < a.x) bset.set(2);
	else if (p->position.x + p->radius > b.x) bset.set(3);
	if (p->position.y - p->radius < a.y) bset.set(0);
	else if (p->position.y + p->radius > b.y) bset.set(1);
	return bset; 
}

void Box::testBoxDraw() {
	ofNoFill();
	ofDrawRectangle(a.x, a.y, b.x - a.x, b.y - a.y);
	ofFill();
}

QuadTree::QuadTree() {

}

QuadTree::QuadTree(int levels) {
	this->root = new Box(ofVec3f(0), ofVec3f(ofGetWindowWidth(), ofGetWindowHeight(), 0));
	generateQuadTree(root, levels);
	linearize(levels);
	rowMod = pow(2, levels - 1);
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
	boxes[TL] = new Box(parent->a, ofVec3f(xMid, yMid), isLeaf, parent->edges.any());
	boxes[TR] = new Box(ofVec3f(xMid, parent->a.y), ofVec3f(parent->b.x, yMid), isLeaf, parent->edges.any());
	boxes[BL] = new Box(ofVec3f(parent->a.x, yMid), ofVec3f(xMid, parent->b.y), isLeaf, parent->edges.any());
	boxes[BR] = new Box(ofVec3f(xMid, yMid), parent->b, isLeaf, parent->edges.any());
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
		traversal(node->children[BL]);
		traversal(node->children[BR]);
	}
	else {
		for (Particle * p : node->particles) {
			cout << p << endl;
		}
	}
}

void QuadTree::linearize(int level) {
	vector<Box *> temp;
	bitset<8> bsetRow(0);
	bitset<8> bsetCol(0);
	int end = pow(2, level - 1);
	int base = 0;
	int count = 0;
	for (int i = 0; i < end; i++) {
		base = 2 * toDecimal(bsetRow);
		for (int j = 0; j < end; j++) {
			int tempIndex = base + toDecimal(bsetCol);
			leafs.at(tempIndex)->boxIndex = count++;
			temp.push_back(leafs.at(tempIndex));
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


void QuadTree::insert(Particle * p) {
	int i = 0;
	Box * box = root;
	while (!box->isLeaf) {
		i += p->position.x < (box->a.x + box->b.x) / 2 ? 0 : 1;
		i += p->position.y < (box->a.y + box->b.y) / 2 ? 0 : 2;
		box = box->children[i];
		i = 0;
	}
	box->addParticle(p);
}

/*
	Returns the adjacent Box of box from Edge e
*/
Box * QuadTree::adjacentBox(Box * box, Edges e) {
	switch (e) {
	case Top:
		if (!box->edges.test(Top)) return leafs.at(box->boxIndex - rowMod);
		break;
	case Bottom:
		if (!box->edges.test(Bottom)) return leafs.at(box->boxIndex + rowMod);
		break;
	case Left:
		if (!box->edges.test(Left)) return leafs.at(box->boxIndex - 1);
		break;
	case Right:
		if (!box->edges.test(Right)) return leafs.at(box->boxIndex + 1);
		break;
	default:
		return NULL;
	}
	return NULL;
}

void QuadTree::clearTree() {
	int count = 0;
	for (Box * box : leafs)
	{
		box->particles.clear();
		count += box->particles.size();
	}
}

