#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEasyX4Gui.h"
#include "ofxFPS.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
//	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	ofxEasyX4Gui		easyX4;
	ofxPanel			gui;
	ofxFPS				fps;
	
	ofParameter<int>	guiFPS;
	ofParameter<int>	guiMinFPS;
	
	ofPoint				midPoint;
	int					inputWidth;
	int					inputHeight;
	
};
