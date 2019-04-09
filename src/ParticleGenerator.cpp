#include "ParticleGenerator.h"

void ParticleGenerator::generateParticle(vector<Particle *> * particles) {
	static thread_local std::mt19937 generator;
	Particle * p = new Particle();
	r = p->radius;
	p->position.set(randW(), randH(), 0);
	std::uniform_real_distribution<float> distrubution(-1.0, 1.0);
	ofVec3f dir = ofVec3f( distrubution(generator), distrubution(generator), 0);
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

Particle * ParticleGenerator::generateParticlePara() {
	static thread_local std::mt19937 generator;
	Particle * p = new Particle();
	p->position.set(randW(), randH(), 0);
	std::uniform_real_distribution<float> distrubution(-1.0, 1.0);
	ofVec3f dir = ofVec3f(distrubution(generator), distrubution(generator), 0);
	p->velocity.set(dir * 10);
	p->color.set(randColor());
	return p;
}

void ParticleGenerator::generateParticlePara(ParticleSystem * sys, int n) {
	vector<Particle*> * vectors[4];
#pragma omp parallel num_threads(4)
	{
		int id, i, nThreads, istart, iend;
		id = omp_get_thread_num();
		nThreads = omp_get_num_threads();
		vectors[id] = new vector<Particle*>;
		istart = id * n / nThreads;
		iend = (id + 1) * n / nThreads;
		if (id == nThreads - 1) iend = n;
		for (i = istart; i < iend; i++) {
			generateParticle(vectors[id]);
		}
	}

	divide(vectors, 0, 3, true);
	addAll(&sys->particles,vectors[0]);
	for (vector<Particle*> * vector : vectors) {
		delete vector;
	}
}

vector<Particle*> * ParticleGenerator::merge(vector<Particle*> * A, vector<Particle*> * B) {
	vector<Particle*> * AB = new vector<Particle*>; //cout << "1" << endl;
	AB->reserve(A->size() + B->size()); //cout << "2" << endl;
	AB->insert(AB->end(), A->begin(), A->end()); //cout << "3" << endl;
	AB->insert(AB->end(), B->begin(), B->end()); //cout << "4" << endl;
	return AB;
}

void ParticleGenerator::addAll(vector<Particle*> * from, vector<Particle*> * to)
{
	from->reserve(from->size() + to->size());
	from->insert(from->end(), to->begin(), to->end());
}

void ParticleGenerator::divide(vector<Particle*> * vectors[], int left, int right, bool isLeft) {
	if (left >= right) return;
	int mid = left + (right - left) / 2;
	divide(vectors, left, mid, true);
	divide(vectors, mid + 1, right, false);
	isLeft ? vectors[left] = merge(vectors[left], vectors[right]) : vectors[right] = merge(vectors[left], vectors[right]);
}

int ParticleGenerator::intRand(const int & min, const int & max) {
	static thread_local std::mt19937 generator;
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
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