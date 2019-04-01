#pragma once

#include "ofMain.h"

class Particle {
public:
	Particle();

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;

	void	init();
	void    integrate();
	void    draw();
	bool	inside(int x, int y);
	bool	inside(ofVec3f p);
	ofColor color;
};

