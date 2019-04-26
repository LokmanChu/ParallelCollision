#pragma once
#include <random>
#include "Particle.h"
#include "ParticleSystem.h"
#include "omp.h"

class ParticleGenerator {
	public:
		ParticleGenerator();
		~ParticleGenerator();
		void setSeed(int seed);
		void generateParticle(vector<Particle *> *);
		void generateParticle(ParticleSystem *,int);

		int intRand(const int & min, const int & max);
		int randW();
		int randH();
		ofColor randColor();

		std::default_random_engine * generator;

		int r;
};
