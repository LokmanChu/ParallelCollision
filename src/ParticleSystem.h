#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "omp.h"

class ParticleForce {
protected:
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
	void removeAll();
	void removeAllForces();
	void update();
	void reset();
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};


