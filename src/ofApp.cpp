#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(ofColor(220, 220, 220));
	handler = CollisionHandler(4);
	drawGrid = false;

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA,4);
}

//--------------------------------------------------------------
void ofApp::update(){
	handler.sys->update();
	handler.checkCollisionTime();
	//handler.checkCollisionTimeMultiple();
	handler.collisionResolve();
	//cout << p1->position << endl;
	//cout << sys->particles.size() << endl;
	handler.gen->generateParticle(handler.sys,10);
}

//--------------------------------------------------------------
void ofApp::draw(){

	//handler.sys->draw();

	if (drawGrid)
		handler.tree->draw();
	
	loadFbo();
	ofSetColor(255, 255, 255);
	fbo.draw(0, 0);

	/*
	shader.begin();
	loadVbo();
	particleTex.bind();
	vbo.draw(GL_POINTS, 0, (int)handler.sys->particles.size());
	particleTex.unbind();
	shader.end();
	*/
	

	drawFrameRate();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case '0':
	case '1':
	case '2':
	case '3':
		handler.algorithm = key-48;
		break;
	case 'd':
		drawGrid = !drawGrid;
		break;
	default:
		break;
	}
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

void ofApp::loadFbo() {
	if (handler.sys->particles.size() < 1) return;

	fbo.begin();
	ofClear(255, 255, 255,0);
	for (Particle * p : handler.sys->particles) {
		ofPushStyle();
		ofSetColor(p->color);
		ofCircle(p->position, p->radius);
		ofPopStyle();
	}
	fbo.end();
}


vector<ofVec3f> * ofApp::merge(vector<ofVec3f> * A, vector<ofVec3f> * B) {
	vector<ofVec3f> * AB = new vector<ofVec3f>; //cout << "1" << endl;
	AB->reserve(A->size() + B->size()); //cout << "2" << endl;
	AB->insert(AB->end(), A->begin(), A->end()); //cout << "3" << endl;
	AB->insert(AB->end(), B->begin(), B->end()); //cout << "4" << endl;
	return AB;
}

void ofApp::addAll(vector<ofVec3f> * from, vector<ofVec3f> * to)
{
	from->reserve(from->size() + to->size());
	from->insert(from->end(), to->begin(), to->end());
}

void ofApp::divide(vector<ofVec3f> * vectors[], int left, int right, bool isLeft) {
	if (left >= right) return;
	int mid = left + (right - left) / 2;
	divide(vectors, left, mid, true);
	divide(vectors, mid + 1, right, false);
	isLeft ? vectors[left] = merge(vectors[left], vectors[right]) : vectors[right] = merge(vectors[left], vectors[right]);
}