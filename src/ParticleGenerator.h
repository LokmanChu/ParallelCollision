#pragma once
#include <random>
#include "Particle.h"
#include "ParticleSystem.h"
#include "omp.h"

class ParticleGenerator {
	public:

		void setSeed(int seed);
		void generateParticle(vector<Particle *> *);
		void generateParticle(ParticleSystem *,int);

		int intRand(const int & min, const int & max);
		int randW();
		int randH();
		ofColor randColor();

		std::mt19937 generator;

		int r;
};
