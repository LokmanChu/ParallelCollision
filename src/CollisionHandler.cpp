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
	omp_init_lock(&edgesLock);
	tree = new QuadTree(3);}

void CollisionHandler::checkCollisionTime() {
	int time;
	ofResetElapsedTimeCounter();
	//checkCollisionNSquare();
	checkCollisionQTree();
	time = ofGetElapsedTimeMillis();
	cout << sys->particles.size() << ": takes " << time << " Millis " << pairs.size() << endl;
}

void CollisionHandler::checkCollisionNSquare() {
	for (int i = 0; i < sys->particles.size(); i++)
	{
		for (int j = i + 1; j < sys->particles.size(); j++)
		{
			if (sys->particles[i]->collideParticle(sys->particles[j])) 
				pairs.push_back(Pair(sys->particles[i], sys->particles[j]));
		}

		int e = sys->particles[i]->collideEdge(All);
		if (None != e) {
			edges.push_back(Pair(sys->particles[i], e));
		}
	}
}

void CollisionHandler::checkCollisionQTree() {
	for (int i = 0; i < sys->particles.size(); i++) {
		tree->insert(sys->particles[i]);
	}
	int setIndex;
	for (Box * box : tree->leafs)
	{
		for (int i = 0; i < box->particles.size(); i++)
		{
			bitset<4> bset = box->overlapEdge(box->particles.at(i));
			setIndex = 0;
			while (bset.any()) {
				if (box->edges.test(setIndex)) {
					//if (5 != sys->particles[i]->collideEdge((Edges)setIndex)) {
						//cout << (Edges)setIndex << endl;
						//edges.push_back(Pair(sys->particles[i], setIndex));
					//}
				}
				else if (bset.test(setIndex)) {
					Box * adjacent = tree->adjacentBox(box, (Edges)setIndex);
					for (int j = 0; j < adjacent->particles.size(); j++)
					{
						if (box->particles[i]->collideParticle(adjacent->particles[j])) {
							pairs.push_back(Pair(box->particles[i], adjacent->particles[j]));
						}
					}
				}
				bset.reset(setIndex);
				setIndex++;
			}
			for (int j = i + 1; j < box->particles.size(); j++)
			{
				if (box->particles[i]->collideParticle(box->particles[j])) {
					pairs.push_back(Pair(box->particles[i], box->particles[j]));
				}
			}
		}
	}
	tree->clearTree();
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