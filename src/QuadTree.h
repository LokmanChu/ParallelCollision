#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"

typedef enum {TL,TR,BL,BR} Quadrant;

class Box {
public:
	Box(ofVec3f a,ofVec3f b); //Index Based Boxes
	Box(ofVec3f a, ofVec3f b, bool isLeaf, bool isEdge); //Quadtree boxes
	
	ofVec3f a, b;
	vector<Box *> children;
	vector<Particle *> particles;
	bool isEdge;
	bool isLeaf;
	int boxIndex;
	bitset<4> edges;
	omp_lock_t writelock;

	void addParticle(Particle * p);
	void findEdges();

	bitset<4> overlapEdge(Particle * p);

	void testBoxDraw();
};


class QuadTree {
public:
	QuadTree(int levels);
	QuadTree();

	Box * root;
	vector<Box *> leafs;
	int rowMod;
	
	void generateQuadTree(Box * root, int levels);
	void divideQuadrants(Box * parent, bool isLeaf);
	void traversal(Box *);
	void linearize(int level);
	void incrementBitSet(bitset<8> * bitset);
	int toDecimal(bitset<8> bitset);
	void draw();

	void insert(Particle * p);
	Box * adjacentBox(Box * box, Edges e);
	void clearTree();
};
