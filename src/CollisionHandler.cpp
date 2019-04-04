#include "CollisionHandler.h"

Pair::Pair(Particle * a, Particle * b) {
	this->a = a;
	this->b = b;
}

Pair::Pair(Particle * a, int e) {
	this->a = a;
	this->e = e;
}

CollisionHandler::CollisionHandler() {
	sys = new ParticleSystem();
	gen = new ParticleGenerator();
	omp_init_lock(&pairsLock);
}

void CollisionHandler::checkCollisionTime() {
	int time;
	ofResetElapsedTimeCounter();
	checkCollisionNSquare();
	time = ofGetElapsedTimeMillis();
	cout << sys->particles.size() << ": takes " << time << " Millis " << pairs.size() << endl;
}

void CollisionHandler::checkCollisionNSquare() {
	for (int i = 0; i < sys->particles.size(); i++)
	{
		for (int j = i + 1; j < sys->particles.size(); j++)
		{
			if (sys->particles[i].collideParticle(&sys->particles[j])) 
				pairs.push_back(Pair(&sys->particles[i], &sys->particles[j]));
		}
		int e = sys->particles[i].collideEdge(All);
		if (e > 0)
			edges.push_back(Pair(&sys->particles[i], e));
	}
}

void CollisionHandler::collisionResolve() {
	for (Pair pair : edges) {
		if (pair.e == Top || pair.e == Bottom) {
			pair.a->velocity.y *= -1;
		}
		else {
			pair.a->velocity.x *= -1;
		}
		pair.a->integrate();
	}
	edges.clear();

	for (Pair pair : pairs) {
		ofVec3f vector = (pair.a->position - pair.b->position).normalize();
		while (pair.b->position.squareDistance(pair.a->position) < pair.a->radius + pair.b->radius) {
			pair.a->position += vector;
		}
		pair.a->position += vector;

		ofVec3f n = (pair.a->position - pair.b->position).normalize();
		ofVec3f u = pair.a->velocity - pair.b->velocity;
		ofVec3f un = n * u.dot(n);
		u -= un;
		pair.a->velocity = pair.b->velocity + u;
		pair.b->velocity = pair.a->velocity + un;
	}
	pairs.clear();
}