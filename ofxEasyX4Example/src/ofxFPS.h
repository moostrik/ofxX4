#pragma once

#include "ofMain.h"

class ofxFPS{
public:
	ofxFPS() {
		lastTimeSet = false;
		FPS = 0;
		minFPS = 0;
		
		ofAddListener(ofEvents().update, this, &ofxFPS::update);
	}

	void update(ofEventArgs & args) {
		if (!lastTimeSet) { lastTimeSet = true; lastTime = ofGetElapsedTimef(); }
		
		deltaTime = ofGetElapsedTimef() - lastTime;
		lastTime = ofGetElapsedTimef();
		
		FPS = (int)(ofGetFrameRate() + 0.5);
		// calculate minimum fps
		deltaTimeDeque.push_back(deltaTime);
		
		while (deltaTimeDeque.size() > FPS)
			deltaTimeDeque.pop_front();
		
		float longestTime = 0;
		for (int i=0; i<deltaTimeDeque.size(); i++){
			if (deltaTimeDeque[i] > longestTime)
				longestTime = deltaTimeDeque[i];
		}
		
		minFPS = (int)(1.0 / longestTime + 0.5);
	}
	
	int getFPS() { return FPS; }
	int getMinFPS() {return minFPS; }
	
private:
	
	deque<float>		deltaTimeDeque;
	bool				lastTimeSet;
	float				lastTime;
	float				deltaTime;
	
	int FPS;
	int minFPS;

};