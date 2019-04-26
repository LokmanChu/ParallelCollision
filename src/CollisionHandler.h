#pragma once

#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "QuadTree.h"

class Pair {
public:
	Pair(Particle * a,Particle * b);
	Pair(Particle * a, int e);

	Particle * a;
	Particle * b;
	int e;
};

class CollisionHandler {
public:
	CollisionHandler(int levels);
	~CollisionHandler();
	ParticleGenerator * gen;
	ParticleSystem * sys;

	vector<Pair> pairs;
	vector<Pair> edges;
	omp_lock_t pairsLock;
	omp_lock_t edgesLock;

	int algorithm;
	int printCount;
	const char *alg[4];

	QuadTree * tree;

	void addPair(Pair pair);

	void checkCollisionTime();
	void checkCollisionTimeMultiple();

	void checkCollisionNSquare();
	void checkCollisionNSquarePara();

	void checkCollisionQTree();
	void checkCollisionQTreeParaStatic();
	void checkCollisionQTreeParaDynamic();
	void checkCollisionQTreeParaGuided();

	void collisionResolve();
};
