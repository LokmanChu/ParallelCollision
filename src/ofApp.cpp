#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(ofColor(220, 220, 220));
	gen = new ParticleGenerator();

	sys = new ParticleSystem();

	ofDisableArbTex();

	if (!ofLoadImage(particleTex, "images/nova_0.png")) {
		cout << "Particle Texture File: images/nova.png not found" << endl;
		ofExit();
	}

#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	gen->generateParticlePara(sys,100);
	sys->update();
	cout << sys->particles.size() << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawFrameRate();
	//sys->draw();

	shader.begin();

	//loadVbo();
	loadVboPara();
	particleTex.bind();
	vbo.draw(GL_POINTS, 0, (int)sys->particles.size());
	particleTex.unbind();

	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::drawFrameRate()
{
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::black);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, ofGetWindowHeight() - 10);
}

void ofApp::loadVbo() {
	if (sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;

	for (int i = 0; i < sys->particles.size(); i++) {
		points.push_back(sys->particles[i].position);
		sizes.push_back(ofVec3f(10));
	}

	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
