#include "CollisionHandler.h"

Pair::Pair(Particle * a, Particle * b) {
	this->a = a;
	this->b = b;
}

Pair::Pair(Particle * a, int e) {
	this->a = a;
	this->e = e;
}

CollisionHandler::CollisionHandler(int levels) {
	sys = new ParticleSystem();
	gen = new ParticleGenerator();
	omp_init_lock(&pairsLock);
	omp_init_lock(&edgesLock);
	tree = new QuadTree(levels);
	algorithm = 0;
	alg[0] = " N^2"; alg[1] = " N^2 Para"; alg[2] = " QTree"; alg[3] = " QTree Para";
	printCount = 0;
}

CollisionHandler::~CollisionHandler() {
	delete gen;
	delete sys;
	delete tree;
}

void CollisionHandler::addPair(Pair pair) {
	omp_set_lock(&pairsLock);
	pairs.push_back(pair);
	omp_unset_lock(&pairsLock);
}

void CollisionHandler::checkCollisionTime() {
	int time;
	ofResetElapsedTimeCounter();
	if (algorithm == 0)
	{
		checkCollisionNSquare();
	}
	else if (algorithm == 1) {

	} 
	else if (algorithm == 2) {
		checkCollisionQTree();
	}
	else {
		checkCollisionQTreeParaDynamic();
	}
	time = ofGetElapsedTimeMillis();
	if (printCount++ % 30 == 0)
	{
		printCount = 1;
		cout << sys->particles.size() << alg[algorithm] << ": takes " << time << " Millis " << pairs.size() << endl;
	}
}

void CollisionHandler::checkCollisionTimeMultiple() {
	double time1,time2;
	ofResetElapsedTimeCounter();
	checkCollisionQTreeParaStatic();
	time1 = ofGetElapsedTimeMicros();
	cout << "N^2:" << sys->particles.size() << ": takes " << time1 << " Micros " << pairs.size() << endl;
	pairs.clear();
	edges.clear();

	ofResetElapsedTimeCounter();
	checkCollisionQTreeParaDynamic();
	time2 = ofGetElapsedTimeMicros();
	cout << "N^2 Para:" << sys->particles.size() << ": takes " << time2 << " Micros " << pairs.size() << endl;
	cout << (time1 - time2) / time1 << endl;
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

void CollisionHandler::checkCollisionNSquarePara() {
#pragma omp parallel num_threads(4)
	{
		#pragma omp for schedule(dynamic)
		for (int i = 0; i < sys->particles.size(); i++)
		{
			for (int j = i + 1; j < sys->particles.size(); j++)
			{
				if (sys->particles[i]->collideParticle(sys->particles[j]))
					addPair(Pair(sys->particles[i], sys->particles[j]));
			}
		}
	}
}

void CollisionHandler::checkCollisionQTree() {
	
	for (int i = 0; i < sys->particles.size(); i++) {		//Add every Particle to tree
		tree->insert(sys->particles[i]);
	}

	int setIndex;
	for (Box * box : tree->leafs)							//Loop through each leaf
	{
		for (int i = 0; i < box->particles.size(); i++)		//Loop through each Particle in leaf
		{
			bitset<4> bset = box->overlapEdge(box->particles.at(i));	//List of all Edges
			setIndex = 0;												//edge counter
			while (bset.any()) {
				if (!box->edges.test(setIndex) && bset.test(setIndex)) {
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

void CollisionHandler::checkCollisionQTreeParaStatic() {
	#pragma omp parallel num_threads(4)
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < sys->particles.size(); i++) {		//Add every Particle to tree
			tree->insert(sys->particles[i]);
		}

		#pragma omp for schedule(dynamic, 1)
		for (int index = 0; index < tree->leafs.size(); index++)		//Loop through each leaf
		{
			Box * box = tree->leafs.at(index);
			int setIndex;
			for (int i = 0; i < box->particles.size(); i++)		//Loop through each Particle in leaf
			{
				bitset<4> bset = box->overlapEdge(box->particles.at(i));	//List of all Edges
				setIndex = 0;												//edge counter
				while (bset.any()) {
					if (!box->edges.test(setIndex) && bset.test(setIndex)) {
						Box * adjacent = tree->adjacentBox(box, (Edges)setIndex);
						for (int j = 0; j < adjacent->particles.size(); j++)
						{
							if (box->particles[i]->collideParticle(adjacent->particles[j])) {
								addPair(Pair(box->particles[i], adjacent->particles[j]));
							}
						}
					}
					bset.reset(setIndex);
					setIndex++;
				}
				for (int j = i + 1; j < box->particles.size(); j++)
				{
					if (box->particles[i]->collideParticle(box->particles[j])) {
						addPair(Pair(box->particles[i], box->particles[j]));
					}
				}
			}
		}
	}
	tree->clearTree();
}

void CollisionHandler::checkCollisionQTreeParaDynamic() {
#pragma omp parallel num_threads(4)
	{
#pragma omp for schedule(static)
		for (int i = 0; i < sys->particles.size(); i++) {		//Add every Particle to tree
			tree->insert(sys->particles[i]);
		}

#pragma omp for schedule(dynamic, 1)
		for (int index = 0; index < tree->leafs.size(); index++)		//Loop through each leaf
		{
			Box * box = tree->leafs.at(index);
			int setIndex;
			for (int i = 0; i < box->particles.size(); i++)		//Loop through each Particle in leaf
			{
				bitset<4> bset = box->overlapEdge(box->particles.at(i));	//List of all Edges
				setIndex = 0;												//edge counter
				while (bset.any()) {
					if (!box->edges.test(setIndex) && bset.test(setIndex)) {
						Box * adjacent = tree->adjacentBox(box, (Edges)setIndex);
						for (int j = 0; j < adjacent->particles.size(); j++)
						{
							if (box->particles[i]->collideParticle(adjacent->particles[j])) {
								addPair(Pair(box->particles[i], adjacent->particles[j]));
							}
						}
					}
					bset.reset(setIndex);
					setIndex++;
				}
				for (int j = i + 1; j < box->particles.size(); j++)
				{
					if (box->particles[i]->collideParticle(box->particles[j])) {
						addPair(Pair(box->particles[i], box->particles[j]));
					}
				}
			}
		}
	}
	tree->clearTree();
}

void CollisionHandler::checkCollisionQTreeParaGuided() {
#pragma omp parallel num_threads(4)
	{
#pragma omp for schedule(static)
		for (int i = 0; i < sys->particles.size(); i++) {		//Add every Particle to tree
			tree->insert(sys->particles[i]);
		}

#pragma omp for schedule(guided)
		for (int index = 0; index < tree->leafs.size(); index++)		//Loop through each leaf
		{
			Box * box = tree->leafs.at(index);
			int setIndex;
			for (int i = 0; i < box->particles.size(); i++)		//Loop through each Particle in leaf
			{
				bitset<4> bset = box->overlapEdge(box->particles.at(i));	//List of all Edges
				setIndex = 0;												//edge counter
				while (bset.any()) {
					if (!box->edges.test(setIndex) && bset.test(setIndex)) {
						Box * adjacent = tree->adjacentBox(box, (Edges)setIndex);
						for (int j = 0; j < adjacent->particles.size(); j++)
						{
							if (box->particles[i]->collideParticle(adjacent->particles[j])) {
								addPair(Pair(box->particles[i], adjacent->particles[j]));
							}
						}
					}
					bset.reset(setIndex);
					setIndex++;
				}
				for (int j = i + 1; j < box->particles.size(); j++)
				{
					if (box->particles[i]->collideParticle(box->particles[j])) {
						addPair(Pair(box->particles[i], box->particles[j]));
					}
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
		//while (pair.b->position.squareDistance(pair.a->position) < pair.a->radius + pair.b->radius) {
			//pair.a->position += vector;
		//}
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