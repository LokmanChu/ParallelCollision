#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "omp.h"
#include <stdlib.h>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawFrameRate();
		void loadVbo();
		void loadVboPara();

		vector<ofVec3f> * merge(vector<ofVec3f> * A, vector<ofVec3f> * B);
		void addAll(vector<ofVec3f> * from, vector<ofVec3f> * to);
		void divide(vector<ofVec3f> * vectors[], int left, int right, bool isLeft);

		ParticleGenerator * gen;
		ParticleSystem * sys;

		ofTexture  particleTex;

		ofVbo vbo;
		ofShader shader;
		
};
