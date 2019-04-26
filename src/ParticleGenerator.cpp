#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator() {
	generator = new std::default_random_engine();
}

ParticleGenerator::~ParticleGenerator() {
	delete generator;
}

void ParticleGenerator::setSeed(int seed) {
	delete generator;
	generator = new std::default_random_engine();
	generator->seed(seed);
}

void ParticleGenerator::generateParticle(vector<Particle *> * particles) {
	Particle * p = new Particle();
	r = p->radius;
	p->position.set(randW(), randH(), 0);
	std::uniform_real_distribution<float> distrubution(-1.0, 1.0);
	ofVec3f dir = ofVec3f( distrubution(*generator), distrubution(*generator), 0);
	p->velocity.set(dir * intRand(10,100));
	p->color.set(randColor());
	particles->push_back(p);
}

void ParticleGenerator::generateParticle(ParticleSystem * sys, int n) {
	for (int i = 0; i < n; i++)
	{
		generateParticle(&sys->particles);
	}
}

int ParticleGenerator::intRand(const int & min, const int & max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(*generator);
}

int ParticleGenerator::randW() {
	return intRand(0 + r * 10, ofGetWidth() - r * 10);
}

int ParticleGenerator::randH() {
	return intRand(0 + r * 10, ofGetHeight() - r * 10);
}

ofColor ParticleGenerator::randColor() {
	return ofColor(intRand(0, 255), intRand(0, 255), intRand(0, 255));
}