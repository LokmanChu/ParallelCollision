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
	radius = 2;
	damping = .99;
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

	if (position.y - radius <= 0 || (position.y + radius) >= ofGetWindowHeight()) {
		velocity.y *= -1;
	}
	if (position.x - radius <= 0 || (position.x + radius) >= ofGetWindowWidth()) {
		velocity.x*=-1;
	}

	forces.set(0, 0, 0);
}

Edges Particle::collideEdge(Edges e) {
	bool top = 0, bot = 0, left = 0, right = 0;
	switch (e)
	{
	case All:
	case Top:
		top = (this->position.y - this->radius) <= 0;
		//cout << top << endl;
		if (e == Top) return (top ? Top : None);
	case Bottom:
		bot = (this->position.y + this->radius) >= ofGetWindowHeight();
		//cout << bot << endl;
		if (e == Bottom) return (bot ? Bottom : None);
	case Left:
		left = (this->position.x - this->radius) <= 0;
		//cout << this->position.x << " " << left << endl;
		if (e == Left) return (left ? Left : None);
	case Right:
		right = (this->position.x + this->radius) >= ofGetWindowWidth();
		//cout << right << endl;
		if (e == Right) return (right ? Right : None);
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
	float framerate = ofGetFrameRate();
	if (framerate < 1.0) framerate = 1;

	float dt = 1.0 / framerate;

	int dist = (radius + p->radius);
	return ofVec3f(position+velocity*dt).squareDistance(ofVec3f(p->position+p->velocity*dt)) <= dist * dist;
}


