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
		vector<Particle> * merge(vector<Particle> *, vector<Particle> *);
		void addAll(vector<Particle> *, vector<Particle> *);
		void divide(vector<Particle> *[], int, int, bool);
	
		int intRand(const int & min, const int & max);
		int randW();
		int randH();
		ofColor randColor();


};