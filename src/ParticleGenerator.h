#pragma once
#include <random>
#include "Particle.h"

class ParticleGenerator {
	public:
		void generateParticle(vector<Particle>);
		void generateParticle(vector<Particle>,int);
	
		int intRand(const int & min, const int & max);
		int randW();
		int randH();
		ofColor randColor();
};