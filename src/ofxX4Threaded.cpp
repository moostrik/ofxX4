//
//  ofxX4Threaded.cpp
//  ofxX4Example
//
//  Created by Ties East on 7/10/15.
//
//

#include "ofxX4Threaded.h"


void ofxX4Threaded::setup() {
	startThread();
}
/*
void ofxX4Threaded::getDeviceInited() {
	lock();
	return allDevicedInited;
	unlock();
}
*/
void ofxX4Threaded::threadedFunction() {
	
	while(isThreadRunning()) {
		if(!X4.getDeviceInited()) {
			X4.setup(0);
			
			allDevicedInited[4] = true;
			
			printf("SETUP \n");
		}
		
		if (nameChangeNeeded) {
			x4.setName(nameChangeNeeded);
			listenerMethod.
			nameChangeNeeded = NULL;
		}
	}
}


void setDeviceName( _listenerMethod, name) {
	listenerMethod = _listenerMethod;
	nameChangeNeeded = name;
}

