#pragma once
#include <random>
#include "Particle.h"
#include "omp.h"

class ParticleGenerator {
	public:
		void generateParticle(vector<Particle> *);
		void generateParticle(vector<Particle> *,int);

		void generateParticlePara(vector<Particle> *, int);
		void merge(vector<Particle> *, vector<Particle> *);
		void divide(vector<vector<Particle> *>, int, int);
	
		int intRand(const int & min, const int & max);
		int randW();
		int randH();
		ofColor randColor();


};