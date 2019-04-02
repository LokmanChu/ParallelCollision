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
	radius = 1;
	damping = 1;
	mass = 1;
	color = ofColor(255, 255, 255);

}


void Particle::draw() {
	ofPushMatrix();
	ofFill();
	ofSetColor(color);
	ofDrawSphere(position, radius);
	ofPopMatrix();
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

bool Particle::inside(int x, int y) {
	return ofVec3f(x, y).squareDistance(position) <= radius * radius;
}

bool Particle::inside(ofVec3f p)
{
	return inside(p.x, p.y);
}


