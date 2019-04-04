#pragma once
#include <random>
#include "Particle.h"
#include "ParticleSystem.h"
#include "omp.h"

class ParticleGenerator {
	public:
		void generateParticle(vector<Particle> *);
		void generateParticle(ParticleSystem *,int);

		void generateParticlePara(ParticleSystem *, int);
		Particle * generateParticlePara();

		vector<Particle> * merge(vector<Particle> * A, vector<Particle> * B);
		void addAll(vector<Particle> * from, vector<Particle> * to);
		void divide(vector<Particle> * vectors[], int left, int right, bool isLeft);
	
		int intRand(const int & min, const int & max);
		int randW();
		int randH();
		ofColor randColor();

		int r;
};
