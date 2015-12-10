#pragma once

#include "ofMain.h"
#include "ofxX4SL.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	ofxX4SL X4;
	
};
