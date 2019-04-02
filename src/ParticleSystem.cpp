#include "ParticleSystem.h"

void ParticleSystem::add(const Particle &p) {
	particles.push_back(p);
}

void ParticleSystem::addForce(ParticleForce *f) {
	forces.push_back(f);
}

void ParticleSystem::remove(int i) {
	particles.erase(particles.begin() + i);
}

void ParticleSystem::removeAll()
{
	vector<Particle>::iterator p = particles.begin();
	vector<Particle>::iterator tmp;

	while (p != particles.end()) {
		tmp = particles.erase(p);
		p = tmp;
	}
}

void ParticleSystem::removeAllForces()
{
	forces.clear();
}


void ParticleSystem::reset() {
	for (int i = 0; i < forces.size(); i++) {
		forces[i]->applied = false;
	}
}

void ParticleSystem::update() {
	// check if empty and just return
	if (particles.size() == 0) return;

	vector<Particle>::iterator p = particles.begin();
	vector<Particle>::iterator tmp;

	if (forces.size() != 0) {
		// update forces on all particles first 
		//
		for (int i = 0; i < particles.size(); i++) {
			for (int k = 0; k < forces.size(); k++) {
				if (!forces[k]->applied)
					forces[k]->updateForce(&particles[i]);
			}
		}

		// update all forces only applied once to "applied"
		// so they are not applied again.
		//
		for (int i = 0; i < forces.size(); i++) {
			if (forces[i]->applyOnce)
				forces[i]->applied = true;
		}
	}

	// integrate all the particles in the store
	//
	for (int i = 0; i < particles.size(); i++)
		particles[i].integrate();

}

void ParticleSystem::draw() {

	for (int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
}




