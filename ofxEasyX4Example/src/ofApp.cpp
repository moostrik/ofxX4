#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	easyX4.setup(0);
	
	gui.setup("settings");
	gui.add(guiFPS.set("average FPS", 0, 0, 60));
	gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
	gui.add(easyX4.parameters);
	
	dynamic_cast <ofxGuiGroup *>(gui.getControl(2))->minimizeAll();
	
	midPoint = ofPoint(0.5, 0.5);
	
	if (easyX4.getDeviceInited()) {
		inputWidth = easyX4.getInputWidth();
		inputHeight = easyX4.getInputHeight();
	}
	else {
		inputWidth = 0;
		inputHeight = 0;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofDrawLine(midPoint.x * ofGetWindowWidth(), 0, midPoint.x * ofGetWindowWidth(), ofGetWindowHeight());
	ofDrawLine(0, midPoint.y * ofGetWindowHeight(), ofGetWindowWidth(), midPoint.y * ofGetWindowHeight());
	
	string centreString = "[" + ofToString(int(midPoint.x * inputWidth)) + ", " + ofToString(int(midPoint.y * inputHeight)) + "]";
	
	int drawX = midPoint.x * ofGetWindowWidth() + 4;
	int drawY = midPoint.y * ofGetWindowHeight() + 14;
	
	ofDrawBitmapString(centreString, drawX, drawY);
	ofDrawBitmapString("drag to select crop layout, release to apply", drawX, 14);
	
	
	guiFPS.set(fps.getFPS());
	guiMinFPS.set(fps.getMinFPS());
	
	
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	midPoint = ofPoint(x / (float)ofGetWindowWidth(), y / (float)ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	midPoint = ofPoint(x / (float)ofGetWindowWidth(), y / (float)ofGetWindowHeight());
	ofRectangle rect;

	if (easyX4.getDeviceInited()) {
		rect = ofRectangle(0, 0, midPoint.x, midPoint.y);
		easyX4.setOutputCropNormalized(0, rect);

		rect = ofRectangle(midPoint.x, 0, 1 - midPoint.x, midPoint.y);
		easyX4.setOutputCropNormalized(1, rect);

		rect = ofRectangle(0, midPoint.y, midPoint.x, 1 - midPoint.y);
		easyX4.setOutputCropNormalized(2, rect);

		rect = ofRectangle(midPoint.x, midPoint.y, 1 - midPoint.x, 1 - midPoint.y);
		easyX4.setOutputCropNormalized(3, rect);


		inputWidth = easyX4.getInputWidth();
		inputHeight = easyX4.getInputHeight();
	}
	
}
