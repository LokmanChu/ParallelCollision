#pragma once
#include "ofMain.h"
#include "ParticleSystem.h"

typedef enum {TL,TR,BL,BR} Quadrant;

class Box {
public:
	Box(ofVec3f a,ofVec3f b); //Index Based Boxes
	Box(ofVec3f a, ofVec3f b, Box * parent, Quadrant q, bool isLeaf, bool isEdge); //Quadtree boxes
	
	ofVec3f a, b;
	Box * parent;
	Box * children[4];
	Quadrant quadrant;
	vector<Particle *> particles;
	bool isEdge;
	int edges[2] = { 0,0 };
	omp_lock_t writelock;

	void addParticle(Particle * p);
	void findEdges();

	void testBoxDraw();
};

class QuadTree {
public:
	QuadTree(int levels);
	QuadTree();

	Box * root;
	Box * children[4];
	vector<Box *> leafs;

	void generateQuadTree(Box * root, int levels);
	void divideQuadrants(Box * parent, Box * [], bool isLeaf);
	void draw();
	void traversal(Box *);
};