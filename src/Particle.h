#pragma once

#include "ofMain.h"
typedef enum {None, Top, Bottom, Left, Right, All} Edges;

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
	ofColor color;

	void	init();
	void    integrate();
	void    draw();
	bool	collideParticle(Particle * p);
	Edges	collideEdge(Edges);
};

