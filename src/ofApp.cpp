#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(ofColor(220, 220, 220));
	handler = CollisionHandler();

	ofDisableArbTex();

	if (!ofLoadImage(particleTex, "images/nova_0.png")) {
		cout << "Particle Texture File: images/nova.png not found" << endl;
		ofExit();
	}
	//handler.tree->traversal(handler.tree->root);
	//cout << handler.tree->leafs.size() << endl;


	//handler.gen->generateParticle(handler.sys, 1);
	//handler.gen->generateParticle(handler.sys, 1);
	//p1 = handler.sys->particles.at(0);
	//p2 = handler.sys->particles.at(1);

	//p1->position.set(ofVec3f(200, 50, 50)); p1->velocity = ofVec3f(-100,0,0);
	//p2->position = ofVec3f(200, 50, 50); p2->velocity = ofVec3f(-5,0,0);

#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
	handler.sys->update();
	handler.checkCollisionTime();
	handler.collisionResolve();
	//cout << p1->position << endl;
	//cout << sys->particles.size() << endl;
	handler.gen->generateParticle(handler.sys,10);
}

//--------------------------------------------------------------
void ofApp::draw(){

	handler.sys->draw();

	handler.tree->draw();

	//handler.tree->draw(handler.tree->root);
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
	if (handler.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;

	for (int i = 0; i < handler.sys->particles.size(); i++) {
		points.push_back(handler.sys->particles[i]->position);
		sizes.push_back(ofVec3f(10));
	}

	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}

void ofApp::loadVboPara() {
	if (handler.sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;

	vector<ofVec3f> * sizesA[4];
	vector<ofVec3f> * pointsA[4];

#pragma omp parallel num_threads(4)
	{
		int id, i, nThreads, istart, iend;
		id = omp_get_thread_num();
		nThreads = omp_get_num_threads();
		sizesA[id] = new vector<ofVec3f>;
		pointsA[id] = new vector<ofVec3f>;
		istart = id * handler.sys->particles.size() / nThreads;
		iend = (id + 1) * handler.sys->particles.size() / nThreads;
		if (id == nThreads - 1) iend = handler.sys->particles.size();
		for (i = istart; i < iend; i++) {
			pointsA[id]->push_back(handler.sys->particles[i]->position);
			sizesA[id]->push_back(ofVec3f(10));
		}
	}

	divide(sizesA, 0, 3, true);
	divide(pointsA, 0, 3, true);

	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&pointsA[0]->at(0), total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizesA[0]->at(0), total, GL_STATIC_DRAW);
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