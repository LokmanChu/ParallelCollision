#include "Testing.h"

Testing::Testing() {
	handler = new CollisionHandler(4);
	data = new vector<int>();
	file.open("output.txt",std::ofstream::out | std::ofstream::trunc);
	file.close();
	cout << "Started" << endl;
	runTest();
}

void Testing::runTest() {
	int time0 = 0, time1 = 0, time2 = 0;
	algNum = 0;
	while (time0 < 20000 || time1 < 20000 || time2 < 20000) {
		time0 = getAlgorithmTime(0);
		time1 = getAlgorithmTime(0);
		time2 = getAlgorithmTime(0);

		data->push_back(median(time0, time1, time2));

		handler->collisionResolve();
		handler->gen->generateParticle(handler->sys, 1);
	}
	writeFile();
	cout  << "completed alg: " << 0 << endl;
	
	for (levels = 4; levels < 10; levels++) {
		for (algNum = 1; algNum <= 4; algNum++)
		{
			int increment = 1;
			time0 = 0, time1 = 0, time2 = 0;
			delete handler;
			handler = new CollisionHandler(levels);
			while (time0 < 20000 || time1 < 20000 || time2 < 20000) {
				time0 = getAlgorithmTime(algNum);
				time1 = getAlgorithmTime(algNum);
				time2 = getAlgorithmTime(algNum);

				data->push_back(median(time0, time1, time2));

				handler->collisionResolve();
				handler->gen->generateParticle(handler->sys, increment);
				if (data->size() % 1000 == 0) increment *= 10;
			}
			writeFile();
			cout << "for level: " << levels << " completed alg: " << algNum << endl;
		}
	}
}

int Testing::getAlgorithmTime(int n) {
	ofResetElapsedTimeCounter();
	switch (n) {
	case 0:
		handler->checkCollisionNSquare();
		break;
	case 1:
		handler->checkCollisionQTree();
		break;
	case 2:
		handler->checkCollisionQTreeParaStatic();
		break;
	case 3:
		handler->checkCollisionQTreeParaDynamic();
		break;
	case 4:
		handler->checkCollisionQTreeParaGuided();
		break;
	}
	int time = ofGetElapsedTimeMicros();
	handler->pairs.clear();
	handler->edges.clear();
	return time;
}

int Testing::median(int a, int b, int c) {
	int min, max;
	if (a > b) {
		max = a;
		min = b;
	}
	else {
		max = b;
		min = a;
	}

	if (c < min) {
		return min;
	}
	else if (c > max) {
		return max;
	}
	else {
		return c;
	}
}

void Testing::writeFile() {
	file.open("output.txt", std::ofstream::out | std::ofstream::app);
	if (algNum == 0) {
		file << "---------- N^2" << endl;
	}
	else {
		file << "---------- Para" << algNum << "(" << levels << ")" << endl;

	}
	for (int i = 0; i < data->size(); i++) {
		file << data->at(i) << endl;
	}
	file.close();
	delete data;
	data = new vector<int>();
}