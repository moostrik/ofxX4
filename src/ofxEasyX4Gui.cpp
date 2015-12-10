
#include "ofxEasyX4Gui.h"

void ofxEasyX4Gui::setup(int _deviceNum) {
	ofxEasyX4::setup(_deviceNum);
	setupParameters(_deviceNum);
	getInfoParametersFromDevice();
	getInputParametersFromDevice();
	getOutputParametersFromDevice();
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setupParameters(int _deviceNum) {
	
	blockListners = false;
	
	parameters.setName("X4_" + ofToString(_deviceNum));

	parameters.add(pScan.set("scan for devices", false));
	pScan.addListener(this, &ofxEasyX4Gui::pScanListner);
	parameters.add(pNumberOfDevices.set("number of devices", "0"));
	parameters.add(pDeviceSelector.set("device select", 0, -1, 0));
	parameters.add(pConnect.set("connect", false));
	pConnect.addListener(this, &ofxEasyX4Gui::pConnectListner);
	parameters.add(pName.set("name", "unknown"));
	parameters.add(pSerial.set("serial", "unknown"));
	parameters.add(pPrintSettings.set("print settings", pPrintSettings));
	pPrintSettings.addListener(this, &ofxEasyX4Gui::pPrintSettingsListner);
	
	InputParameters.setName("input");
	InputParameters.add(pInputUpdate.set("fetch", false));
	pInputUpdate.addListener(this, &ofxEasyX4Gui::pInputUpdateListner);
	InputParameters.add(pInputDualLinkStatus.set("dual link status", "false"));
	InputParameters.add(pInputWidth.set("width", 1280, resWidth[0], resWidth[resWidthCount-1]));
	pInputWidth.addListener(this, &ofxEasyX4Gui::pWidthListner);
	InputParameters.add(pInputHeight.set("Height", 1024, resHeight[0], resHeight[resHeightCount-1]));
	pInputHeight.addListener(this, &ofxEasyX4Gui::pHeightListner);
	InputParameters.add(pInputRefreshRate.set("refresh rate", 60, resRefreshRate[0], resRefreshRate[resRefreshRateCount-1]));
	pInputRefreshRate.addListener(this, &ofxEasyX4Gui::pRefreshRateListner);
	InputParameters.add(pSmoothText.set("smooth for text", false));
	InputParameters.add(pEqualisationLevel.set("equalisation", X4EQUALISATION_LEVEL0, X4EQUALISATION_LEVEL0, X4EQUALISATION_LEVEL7));
	InputParameters.add(pInputApply.set("apply", false));
	pInputApply.addListener(this, &ofxEasyX4Gui::pInputApplyListner);
	parameters.add(InputParameters);
	
	OutputParameters = new ofParameterGroup[4];
	pOutputWidth = new ofParameter<int>[4];
	pOutputHeight = new ofParameter<int>[4];
	pOutputRefreshRate = new ofParameter<float>[4];
	pOutputAnalogue  = new ofParameter<bool>[4];
	OutputCropParameters = new ofParameterGroup[4];
	pOutputCropX = new ofParameter<int>[4];
	pOutputCropY = new ofParameter<int>[4];
	pOutputCropWidth = new ofParameter<int>[4];
	pOutputCropHeight = new ofParameter<int>[4];
	pOutputTransform = new ofParameter<int>[4];
	pOutputTransformName = new ofParameter<string>[4];
	pOutputTestPattern = new ofParameter<int>[4];
	pOutputTestPatternName = new ofParameter<string>[4];
	pOutputApply = new ofParameter<bool>[4];
	for (int i=0; i<4; i++) {
		OutputParameters[i].setName("output " + ofToString(i));
		OutputParameters[i].add(pOutputWidth[i].set("width", 1280, resWidth[0], resWidth[resWidthCount-1]));
		pOutputWidth[i].addListener(this, &ofxEasyX4Gui::pWidthListner);
		OutputParameters[i].add(pOutputHeight[i].set("Height", 1024, resHeight[0], resHeight[resHeightCount-1]));
		pOutputHeight[i].addListener(this, &ofxEasyX4Gui::pHeightListner);
		OutputParameters[i].add(pOutputRefreshRate[i].set("refresh rate", 60, resRefreshRate[0], resRefreshRate[resRefreshRateCount-1]));
		pOutputRefreshRate[i].addListener(this, &ofxEasyX4Gui::pRefreshRateListner);
		OutputParameters[i].add(pOutputAnalogue[i].set("analogue", false));
		OutputCropParameters[i].setName("crop");
		OutputCropParameters[i].add(pOutputCropX[i].set("x", 0, 0, pInputWidth.get()));
		pOutputCropX[i].addListener(this, &ofxEasyX4Gui::pCropListner);
		OutputCropParameters[i].add(pOutputCropY[i].set("y", 0, 0, pInputHeight.get()));
		pOutputCropY[i].addListener(this, &ofxEasyX4Gui::pCropListner);
		OutputCropParameters[i].add(pOutputCropWidth[i].set("width", pInputWidth.get(), 0, pInputWidth.get()));
		pOutputCropWidth[i].addListener(this, &ofxEasyX4Gui::pCropListner);
		OutputCropParameters[i].add(pOutputCropHeight[i].set("height", pInputHeight.get(), 0, pInputHeight.get()));
		pOutputCropHeight[i].addListener(this, &ofxEasyX4Gui::pCropListner);
		OutputParameters[i].add(OutputCropParameters[i]);
		OutputParameters[i].add(pOutputTransform[i].set("TR 90  180  270 flpV H", ROTATION_NONE, ROTATION_NONE, FLIP_VERT));
		OutputParameters[i].add(pOutputTestPattern[i].set("TPT GR   COL   GR2  C2 ", X4IMGSRC_CROPPING, X4IMGSRC_CROPPING, X4IMGSRC_TSTPAT_COLOURMOVE));
		OutputParameters[i].add(pOutputApply[i].set("apply", false));
		parameters.add(OutputParameters[i]);
	}
	
	pOutputApply[0].addListener(this, &ofxEasyX4Gui::pOutputApplyListner0);
	pOutputApply[1].addListener(this, &ofxEasyX4Gui::pOutputApplyListner1);
	pOutputApply[2].addListener(this, &ofxEasyX4Gui::pOutputApplyListner2);
	pOutputApply[3].addListener(this, &ofxEasyX4Gui::pOutputApplyListner3);
	
	saveParameters.setName("save on X4 Flash");
	saveParameters.add(pUpdateFlash.set("save", false));
	pUpdateFlash.addListener(this, &ofxEasyX4Gui::pUpdateFlashListner);
	parameters.add(saveParameters);
	
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void ofxEasyX4Gui::pScanListner(bool &_value) {
	if (_value) {
		_value = false;
		pNumberOfDevices.set(ofToString(X4.getNumDevices()));
		pDeviceSelector.setMax(MAX(X4.getNumDevices() - 1, 0));
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pConnectListner(bool &_value) {
	if (_value) {
		_value = false;
		if (pDeviceSelector.get() >= 0) {
			open(pDeviceSelector.get());
			getInfoParametersFromDevice();
			getInputParametersFromDevice();
			getOutputParametersFromDevice();
		}
		else {
			close();
		}
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pPrintSettingsListner(bool &_value) {
	if (X4.getDeviceInited() && _value)
		_value = false;
		X4.printSettings(true);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pInputUpdateListner(bool &_value) {
	if(_value) {
		_value = false;
		getInputParametersFromDevice();
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pInputApplyListner(bool &_value) {
	if(_value) {
		_value = false;
		setInputParametersToDevice();
		getInputParametersFromDevice();
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pOutputApplyListner(int _index, bool &_value) {
	if(_value) {
		_value = false;
		setOutputParametersToDevice(_index);
		getOutputParametersFromDevice(_index);
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pUpdateFlashListner(bool &_value) {
	if(_value) {
		_value = false;
		if (X4.getDeviceInited()) updateFlash();
	}
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void ofxEasyX4Gui::getInfoParametersFromDevice() {
	if (X4.getDeviceInited()) {
		blockListners = true;

		pNumberOfDevices.set(ofToString(X4.getNumDevices()));
		pDeviceSelector.setMax(MAX(X4.getNumDevices() - 1, 0));
		pName.set(getName());
		pSerial.set(getSerial());
		
		blockListners = false;
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::getInputParametersFromDevice() {
	if (X4.getDeviceInited()) {
		blockListners = true;
		
		X4TIMINGS timings = X4.getInputCurrentTimings();
		if (timings.HorAddrTime == 0) {
			timings = X4.getInputPreferredTimings();
		}
		
		pInputWidth.set(timings.HorAddrTime);
		pInputHeight.set(timings.VerAddrTime);
		pInputRefreshRate.set(timings.VerFrequency / 1000.0);
		pInputDualLinkStatus.set(ofToString(getInputDualLinkStatus()));
		pSmoothText.set(getSmoothText());
		pEqualisationLevel.set(getEqualisationLevel());
		
		for (int i=0; i<4; i++) {
			pOutputCropX[i].setMax(timings.HorAddrTime);
			pOutputCropY[i].setMax(timings.VerAddrTime);
			pOutputCropWidth[i].setMax(timings.HorAddrTime);
			pOutputCropHeight[i].setMax(timings.VerAddrTime);
			
			ofRectangle rect = getOutputCrop(i);
			pOutputCropX[i].set(rect.x);
			pOutputCropY[i].set(rect.y);
			pOutputCropWidth[i].set(rect.width);
			pOutputCropHeight[i].set(rect.height);
		}
		
		blockListners = false;
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::getOutputParametersFromDevice(int _index) {
	if (X4.getDeviceInited()) {
		blockListners = true;
		
		pOutputWidth[_index].set(getOutputDefaultWidth(_index));
		pOutputHeight[_index].set(getOutputDefaultHeight(_index));
		pOutputRefreshRate[_index].set(getOutputDefaultRefreshRate(_index));
		pOutputAnalogue[_index].set(getOutputDefaultAnalogueStatus(_index));
		
		ofRectangle rect = getOutputCrop(_index);
		pOutputCropX[_index].set(rect.x);
		pOutputCropY[_index].set(rect.y);
		pOutputCropWidth[_index].set(rect.width);
		pOutputCropHeight[_index].set(rect.height);
		pOutputTransform[_index].set(getOutputTransform(_index));
		pOutputTestPattern[_index].set(getOutputTestPatternMode(_index));
		
		blockListners = false;
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setInputParametersToDevice() {
	if (X4.getDeviceInited()) {
		X4.setInputEqualisation(X4EQUALISATION(pEqualisationLevel.get()));
		X4.setDeviceSmoothness(pSmoothText.get()? X4SMOOTHNESS_LEVEL0: X4SMOOTHNESS_LEVEL1);
		X4.setInputPreferredTimings(pInputWidth.get(), pInputHeight.get(), pInputRefreshRate.get());
	}
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputParametersToDevice(int _index) {
	if (X4.getDeviceInited()) {
		X4.setOutputDefaultTimings(_index, pOutputWidth[_index].get(), pOutputHeight[_index].get(), pOutputRefreshRate[_index].get(), pOutputAnalogue[_index].get());
		
		X4CROP crop;
		crop.left = pOutputCropX[_index].get();
		crop.top = pOutputCropY[_index].get();
		crop.right = pOutputCropX[_index].get() + pOutputCropWidth[_index].get();
		crop.bottom = pOutputCropY[_index].get() + pOutputCropHeight[_index].get();
		X4.setOutputCrop(_index, crop);
		
		X4.setOutputTransformation(_index, (X4TRANSFORM)pOutputTransform[_index].get());
		X4.setOutputTestPattern(_index, (X4IMAGESOURCE)pOutputTestPattern[_index].get());
	}
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void ofxEasyX4Gui::pWidthListner(int &_value) {
	if(!blockListners)_value = getClosestValue(_value, resWidth, resWidthCount);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pHeightListner(int &_value) {
	if(!blockListners)_value = getClosestValue(_value, resHeight, resHeightCount);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pRefreshRateListner(float &_value) {
	if(!blockListners)_value = getClosestValue(_value, resRefreshRate, resRefreshRateCount);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::pCropListner(int &_value) {
	_value = _value + (_value % 2);
}

//--------------------------------------------------------------------
int ofxEasyX4Gui::getClosestValue(int _value, const int* _values, int _valueSize) {
	for (int i=0; i<_valueSize-2; i++ ) {
		if (_value > _values[i] && _value < _values[i+1]) return (_value - _values[i] < _values[i+1] - _value)? _values[i] : _values[i+1] ;
	}
	return (_value - _values[0] < _values[_valueSize-1] - _value)? _values[0] : _values[_valueSize-1] ;
}

//--------------------------------------------------------------------
float ofxEasyX4Gui::getClosestValue(float _value, const float* _values, int _valueSize) {
	for (int i=0; i<_valueSize-2; i++ ) {
		if (_value > _values[i] && _value < _values[i+1]) return (_value - _values[i] < _values[i+1] - _value)? _values[i] : _values[i+1] ;
	}
	return (_value - _values[0] < _values[_valueSize-1] - _value)? _values[0] : _values[_valueSize-1] ;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
void ofxEasyX4Gui::setName(string _name) {
	ofxEasyX4::setName(_name);
	blockListners = true;
	pName.set(getName());
	blockListners = true;
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setSmoothText() {
	ofxEasyX4::setSmoothText();
	blockListners = true;
	pSmoothText.set(getSmoothText());
	blockListners = true;
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setSmoothVideo() {
	ofxEasyX4::setSmoothVideo();
	blockListners = true;
	pSmoothText.set(getSmoothVideo());
	blockListners = true;
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setEqualisationOff() {
//	blockListners = true;
//	pSmoothText.set(false);
//	blockListners = true;
	ofxEasyX4::setEqualisationOff();
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setEqualisationLevel(X4EQUALISATION _level){
	ofxEasyX4::setEqualisationLevel(_level);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setInputTiming(int _width, int _height, float _refreshRate) {
	ofxEasyX4::setInputTiming(_width, _height, _refreshRate);
	getInputParametersFromDevice();
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setInputWidth(int _width) {
	ofxEasyX4::setInputWidth(_width);
	getInputParametersFromDevice();
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setInputHeight(int _height) {
	ofxEasyX4::setInputHeight(_height);
	getInputParametersFromDevice();
	
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setInputRefreshRate(float _refreshRate){
	ofxEasyX4::setInputRefreshRate(_refreshRate);
	getInputParametersFromDevice();
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputTiming(int _index, int _width, int _height, float _refreshRate, bool _analogue) {
	ofxEasyX4::setOutputTiming(_index, _width, _height, _refreshRate, _analogue);
	getOutputParametersFromDevice(_index);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputWidth(int _index, int _width) {
	ofxEasyX4::setOutputWidth(_index, _width);
	getOutputParametersFromDevice(_index);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputHeight(int _index, int _height) {
	ofxEasyX4::setOutputHeight(_index, _height);
	getOutputParametersFromDevice(_index);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputRefreshRate(int _index, float _refreshRate) {
	ofxEasyX4::setOutputRefreshRate(_index, _refreshRate);
	getOutputParametersFromDevice(_index);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputAnalogue(int _index, bool _analogue) {
	ofxEasyX4::setOutputAnalogue(_index, _analogue);
	getOutputParametersFromDevice(_index);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputCrop(int _index, ofRectangle _rect) {
	ofxEasyX4::setOutputCrop(_index, _rect);
	ofRectangle rect = getOutputCrop(_index);
	pOutputCropX[_index].set(rect.x);
	pOutputCropY[_index].set(rect.y);
	pOutputCropWidth[_index].set(rect.width);
	pOutputCropHeight[_index].set(rect.height);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputCropNormalized(int _index, ofRectangle _rect) {
	ofxEasyX4::setOutputCropNormalized(_index, _rect);
	ofRectangle rect = getOutputCrop(_index);
	pOutputCropX[_index].set(rect.x);
	pOutputCropY[_index].set(rect.y);
	pOutputCropWidth[_index].set(rect.width);
	pOutputCropHeight[_index].set(rect.height);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputTransform(int _index, X4TRANSFORM _transformation) {
	ofxEasyX4::setOutputTransform(_index, _transformation);
	pOutputTransform[_index].set(_transformation);
}

//--------------------------------------------------------------------
void ofxEasyX4Gui::setOutputTestPattern(int _index, X4IMAGESOURCE _source) {
	ofxEasyX4::setOutputTestPattern(_index, _source);
	pOutputTestPattern[_index].set(_source);
}
