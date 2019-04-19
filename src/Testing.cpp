#include "Testing.h"

Testing::Testing() {
	handler = new CollisionHandler(4);
	file.open("output.txt");
	runTest();
	printTimes();
	file.close();
}

void Testing::runTest() {
	const int n = 2;
	for (int i = 0; i < n; i++) {
		arrays.push_back(vector<int>());
	}

	increment = 10;
	for (int i = 0; i < 10000; i++) {
		ofResetElapsedTimeCounter();
		handler->checkCollisionNSquarePara1();
		arrays[0].push_back(ofGetSystemTimeMicros());

		handler->pairs.clear();
		handler->edges.clear();

		ofResetElapsedTimeCounter();
		handler->checkCollisionNSquarePara2();
		arrays[1].push_back(ofGetSystemTimeMicros());

		handler->pairs.clear();
		handler->edges.clear();

		handler->collisionResolve();
		handler->gen->generateParticle(handler->sys, 10);
	}


}

void Testing::printTimes() {
	for (int i = 0; i < arrays.at(0).size(); i++) {
		file << i * increment << ": ";
		for (int j = 0; j < arrays.size(); j++) {
			file << j << ": " << arrays.at(j).at(i);
		}
	}
}