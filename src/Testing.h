#pragma once
#include "ParticleGenerator.h"
#include "CollisionHandler.h"

#include <iostream>
#include <fstream>

class Testing {
public:
	
	Testing();

	void runTest();

	int getAlgorithmTime(int);
	int median(int, int, int);

	void writeFile();

	CollisionHandler * handler;
	
	int algNum;
	int levels;
	ofstream file;
	vector<int> * data;
};
