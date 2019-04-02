#include "ParticleGenerator.h"

void ParticleGenerator::generateParticle(vector<Particle> * particles) {
	static thread_local std::mt19937 generator;
	Particle * p = new Particle();
	p->position.set(randW(), randH(), 0);
	std::uniform_real_distribution<float> distrubution(-1.0, 1.0);
	ofVec3f dir = ofVec3f( distrubution(generator), distrubution(generator), 0);
	p->velocity.set(dir * 10);
	p->color.set(randColor());
	particles->push_back(*p);
}

void ParticleGenerator::generateParticle(vector<Particle> * particles, int n) {
	for (int i = 0; i < n; i++)
	{
		generateParticle(particles);
	}
}

void ParticleGenerator::generateParticlePara(vector<Particle> * particles, int n) {
	int nThreads = omp_get_num_threads();
	vector<vector<Particle> *> vectors;
	for (int i = 0; i < nThreads; i++) {
		vectors.push_back(new vector<Particle>);
	}

#pragma omp parallel
	{
		int id, i, istart, iend;
		id = omp_get_thread_num();
		istart = id * n / nThreads;
		iend = (id + 1) * n / nThreads;
		if (id == nThreads - 1) iend = n;
		for (i = istart; i < iend; i++) {
			generateParticle(vectors[id]);
		}
	}
	
	/*
	divide(vectors, 0, vectors.size());
	free(particles);
	particles = vectors[0];
	*/
}

void ParticleGenerator::merge(vector<Particle> * A, vector<Particle> * B) {
	vector<Particle> * AB = new vector<Particle>;
	AB->reserve(A->size() + B->size());
	AB->insert(AB->end(), A->begin(), A->end());
	AB->insert(AB->end(), B->begin(), B->end());
	free(A);
	free(B);
	A = AB;
}

void ParticleGenerator::divide(vector<vector<Particle> *> vectors, int left, int right) {
	if (left >= right) return;
	int mid = left + (right - left) / 2;
	divide(vectors, left, mid);
	divide(vectors, mid + 1, right);
	merge(vectors[left], vectors[right]);
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