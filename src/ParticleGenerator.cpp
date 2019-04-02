#include "ParticleGenerator.h"

void ParticleGenerator::generateParticle(vector<Particle> particles) {
	static thread_local std::mt19937 generator;
	Particle * p = new Particle();
	p->position.set(randW(), randH(), 0);
	std::uniform_real_distribution<float> distrubution(0.0, 1.0);
	ofVec3f dir = ofVec3f( distrubution(generator), distrubution(generator), 0);
	p->velocity.set(dir * 10);
	p->color.set(randColor());
	particles.push_back(*p);
}

void ParticleGenerator::generateParticle(vector<Particle> particles, int n) {
	for (int i = 0; i < n; i++)
	{
		generateParticle(particles);
	}
}

int ParticleGenerator::intRand(const int & min, const int & max) {
	static thread_local std::mt19937 generator;
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

int ParticleGenerator::randW() {
	return intRand(0, ofGetWidth());
}

int ParticleGenerator::randH() {
	return intRand(0, ofGetHeight());
}

ofColor ParticleGenerator::randColor() {
	return ofColor(intRand(0, 255), intRand(0, 255), intRand(0, 255));
}