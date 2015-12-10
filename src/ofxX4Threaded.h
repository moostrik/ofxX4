#pragma once

#include "ofMain.h"
#include "ofxX4.h"


class ofxX4Threaded : public ofThread{
public:
//	ofxX4Threaded() { ; }
//	virtual ~ofxX4Threaded() { ; }
	
	void setup();
	void update();
	
//	void setDeviceName(ListenerClass * listener, ListenerMethod method, string _name);

	bool getDeviceInited();
	
private:
	ofxX4	X4;
	
	void threadedFunction();
	
};