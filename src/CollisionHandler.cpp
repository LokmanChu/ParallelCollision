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
	//cout << sys->particles.size() << ": takes " << time << " Millis " << pairs.size() << endl;
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

	for (Pair pair : pairs)
	{
		cout << "A: " << pair.a->position << " B: " << pair.b->position << " e: " << pair.e << endl;
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
	}
	edges.clear();

	for (Pair pair : pairs) {
		ofVec3f distanceVector = pair.b->position - pair.a->position;
		float dist = distanceVector.distance(ofVec3f(0, 0, 0));

		cout << "A: " << pair.a->position << " B: " << pair.b->position << endl;

		float sine = distanceVector.y / dist;
		float cosine = distanceVector.x / dist;



		ofVec3f aTempP = ofVec3f(0, 0, 0);
		ofVec3f bTempP = ofVec3f(cosine * distanceVector.x + sine * distanceVector.y, cosine * distanceVector.y - sine * distanceVector.x, 0);

		ofVec3f aTempV = ofVec3f(cosine * pair.a->velocity.x + sine * pair.a->velocity.y, cosine * pair.a->velocity.y - sine * pair.a->velocity.x, 0);
		ofVec3f bTempV = ofVec3f(cosine * pair.b->velocity.x + sine * pair.b->velocity.y, cosine * pair.b->velocity.y - sine * pair.b->velocity.x, 0);

		ofVec3f aFinalV = ofVec3f(((pair.a->mass - pair.b->mass) * aTempV.x + 2 * pair.b->mass * bTempV.x) / (pair.a->mass + pair.b->mass), aTempV.y, 0);
		ofVec3f bFinalV = ofVec3f(((pair.b->mass - pair.a->mass) * bTempV.x + 2 * pair.b->mass * aTempV.x) / (pair.a->mass + pair.b->mass), aTempV.y, 0);

		aTempP += aFinalV;
		bTempP += aFinalV;

		aTempP = cosine * aTempP - sine * aTempP;
		bTempP = cosine * bTempP - sine * bTempP;

		pair.b->position = pair.a->position + bTempP;
		pair.a->position += aTempP;
		pair.a->velocity = ofVec3f(cosine * aFinalV.x - sine * aFinalV.y, cosine * aFinalV.y + sine * aFinalV.x, 0);
		pair.b->velocity = ofVec3f(cosine * bFinalV.x - sine * bFinalV.y, cosine * bFinalV.y + sine * bFinalV.x, 0);
	}
	pairs.clear();
}