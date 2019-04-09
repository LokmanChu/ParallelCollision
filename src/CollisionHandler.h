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
	CollisionHandler();
	ParticleGenerator * gen;
	ParticleSystem * sys;

	vector<Pair> pairs;
	vector<Pair> edges;
	omp_lock_t pairsLock;
	omp_lock_t edgesLock;


	QuadTree * tree;

	void checkCollisionTime();
	void checkCollisionNSquare();
	void checkCollisionQTree();

	void collisionResolve();
};
