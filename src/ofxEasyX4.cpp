#include "ofxEasyX4.h"

ofRectangle	ofxEasyX4::getOutputCrop(int _index) {
	X4CROP crop = X4.getOutputCrop(_index);
	ofRectangle rect;
	rect.x = crop.left;
	rect.y = crop.top;
	rect.width = crop.right - crop.left;
	rect.height = crop.bottom - crop.top;
	return rect;
}

//--------------------------------------------------------------------
ofRectangle	ofxEasyX4::getOutputCropNormalized(int _index) {
	ofRectangle rect = getOutputCrop(_index);
	int width = getOutputWidth(_index);
	int height = getOutputHeight(_index);
	rect.x /= width;
	rect.y /= height;
	rect.width /= width;
	rect.height /= height;
	return rect;
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputTiming(int _index, int _width, int _height, float _refreshRate, bool _analogue) {
	X4.setOutputDefaultTimings(_index, _width, _height, _refreshRate, _analogue);
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputWidth(int _index, int _width) {
	setOutputTiming(_index, _width, getOutputHeight(_index), getOutputRefreshRate(_index), getOutputAnalogueStatus(_index));
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputHeight(int _index, int _height) {
	setOutputTiming(_index, getOutputWidth(_index), _height, getOutputRefreshRate(_index), getOutputAnalogueStatus(_index));
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputRefreshRate(int _index, float _refreshRate) {
	setOutputTiming(_index, getOutputWidth(_index), getOutputHeight(_index), _refreshRate, getOutputAnalogueStatus(_index));
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputAnalogue(int _index, bool _analogue) {
	setOutputTiming(_index, getOutputWidth(_index), getOutputHeight(_index), getOutputRefreshRate(_index), _analogue);
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputCrop(int _index, ofRectangle _rect) {
	X4CROP crop;
	crop.left = _rect.x;
	crop.top = _rect.y;
	crop.right = _rect.x + _rect.width;
	crop.bottom = _rect.y + _rect.height;

	X4.setOutputCrop(_index, crop);
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputCropNormalized(int _index, ofRectangle _rect) {
	int width = getInputWidth();
	int height = getInputHeight();
	_rect.x *= width;
	_rect.y *= height;
	_rect.width *= width;
	_rect.height *= height;

	setOutputCrop(_index, _rect);
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputTransform(int _index, X4TRANSFORM _transformation) {
	X4.setOutputTransformation(_index, _transformation);
}

//--------------------------------------------------------------------
void ofxEasyX4::setOutputTestPattern(int _index, X4IMAGESOURCE _source) {
	X4.setOutputTestPattern(_index, _source);
}