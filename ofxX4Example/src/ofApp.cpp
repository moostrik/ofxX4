#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	X4.setup(0);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString("Press `I` to print info to console", 20, 20);
	ofDrawBitmapString("Press `P` to print settings to console", 20, 40);
	ofDrawBitmapString("Press `E` to print extended settings to console", 20, 60);
	ofDrawBitmapString("Press `S` to save settings to XML", 20, 80);
	ofDrawBitmapString("Press `L` to load settings from XML", 20, 100);
	ofDrawBitmapString("Press `V` to load settings from VQS file (X4 windows app)", 20, 120);
	ofDrawBitmapString("Press `C` to reconnect to device", 20, 140);
	
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'i':
		case 'I':
			X4.printInfo();
			break;
		case 'p':
		case 'P':
			X4.printSettings();
			break;
		case 'e':
		case 'E':
			X4.printSettings(true);
			break;
		case 'l':
		case 'L':
			X4.loadSettings("X4Settings.xml");
			break;
		case 's':
		case 'S':
			X4.saveSettings("X4Settings.xml");
			break;
		case 'c':
		case 'C':
			X4.open(0);
			break;
		case 'v':
		case 'V':
			X4.loadVQS("X4Settings.vqs");
			break;
			
		default:
			break;
	}
 
}
