#include "Particle.h"

//Particle was modified to have different draw methods for different particle types
//miscellaneous game values were also added.
Particle::Particle() {
	init();
}

void Particle::init()
{
	// initialize particle with some reasonable values first;
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	radius = 10;
	damping = 1;
	mass = 1;
	color = ofColor(255, 255, 255);
}


void Particle::draw() {
	ofSetColor(color);
	ofDrawSphere(position, radius);
}

void Particle::integrate() {
	float framerate = ofGetFrameRate();
	if (framerate < 1.0) return;

	float dt = 1.0 / framerate;

	position += (velocity * dt);

	ofVec3f accel = acceleration;
	accel += (forces * (1.0 / mass));

	velocity += accel * dt;
	velocity *= damping;

	forces.set(0, 0, 0);
}

Edges Particle::collideEdge(Edges e) {
	bool top, bot, left, right;
	switch (e)
	{
	case All:
	case Top:
		top = (position.y - radius) <= 0;
		//cout << top << endl;
		if (e == Top) return e;
	case Bottom:
		bot = (position.y + radius) >= ofGetWindowHeight();
		//cout << bot << endl;
		if (e == Bottom) return e;
	case Left:
		left = (position.x - radius) <= 0;
		//cout << left << endl;
		if (e == Left) return e;
	case Right:
		right = (position.x + radius) >= ofGetWindowWidth();
		//cout << right << endl;
		if (e == Right) return e;
	default:
		if (top) return Top;
		if (bot) return Bottom;
		if (left) return Left;
		if (right) return Right;
		return None;
	}
}

bool Particle::collideParticle(Particle * p)
{
	int dist = (radius + p->radius);
	return position.squareDistance(p->position) <= dist * dist;
}


