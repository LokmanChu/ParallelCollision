#pragma once
#include "ParticleGenerator.h"
#include "CollisionHandler.h"

#include <iostream>
#include <fstream>

class Testing {
public:
	
	Testing();

	void runTest();
	void printTimes();

	CollisionHandler * handler;
	
	int increment;
	ofstream file;
	vector<vector<int>> arrays;
};
