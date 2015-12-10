#pragma once

#include "ofMain.h"
#include "ofxEasyX4.h"

const int	resWidth[] = { 1024, 1280, 1440, 1600, 1680, 1920, 2048, 2560, 3440, 3840, 4096 };
const int	resWidthCount = 11;
const int	resHeight[] = { 720, 768, 960, 1024, 1050, 1080, 1200, 1440, 1536, 1600, 2048, 2160 };
const int	resHeightCount = 12;
const float	resRefreshRate[] = { 25, 30, 50, 60, 100, 120 };
const int	resRefreshRateCount = 6;


class ofxEasyX4Gui : public ofxEasyX4{
	
public:
	
	void		setup(int _deviceNum = 0);
	
	void		setName(string _name);
	void		setSmoothText();
	void		setSmoothVideo();
	void		setEqualisationOff();
	void		setEqualisationLevel(X4EQUALISATION _level);
	
	void		setInputTiming(int _width, int _height, float _refreshRate);
	void		setInputWidth(int _width);
	void		setInputHeight(int _height);
	void		setInputRefreshRate(float _refreshRate);
	
	void		setOutputTiming(int _index, int _width, int _height, float _refreshRate, bool _analogue);
	void		setOutputWidth(int _index, int _width);
	void		setOutputHeight(int _index, int _height);
	void		setOutputRefreshRate(int _index, float _refreshRate);
	void		setOutputCrop(int _index, ofRectangle _crop);
	void		setOutputCropNormalized(int _index, ofRectangle _crop);
	void		setOutputAnalogue(int _index, bool _analogue);
	void		setOutputTransform(int _index, X4TRANSFORM _transformation);
	void		setOutputTestPattern(int _index, X4IMAGESOURCE _source);
	
	ofParameterGroup		parameters;
	
private:
	
	void		setupParameters(int _deviceNum);

	ofParameter<bool>		pScan;
	ofParameter<string>		pNumberOfDevices;
	ofParameter<int>		pDeviceSelector;
	ofParameter<bool>		pConnect;
	ofParameter<string>		pName;
	ofParameter<string>		pSerial;
	ofParameter<bool>		pPrintSettings;
	
	ofParameter<bool>		pInputUpdate;
	ofParameter<string>		pInputDualLinkStatus;
	ofParameter<int>		pInputWidth;
	ofParameter<int>		pInputHeight;
	ofParameter<float>		pInputRefreshRate;
	ofParameter<bool>		pSmoothText;
	ofParameter<int>		pEqualisationLevel;
	ofParameter<bool>		pInputApply;
	
	ofParameter<int>*		pOutputWidth;
	ofParameter<int>*		pOutputHeight;
	ofParameter<float>*		pOutputRefreshRate;
	ofParameter<bool>*		pOutputAnalogue;
	ofParameter<int>*		pOutputCropX;
	ofParameter<int>*		pOutputCropY;
	ofParameter<int>*		pOutputCropWidth;
	ofParameter<int>*		pOutputCropHeight;
	ofParameter<int>*		pOutputTransform;
	ofParameter<string>*	pOutputTransformName;
	ofParameter<int>*		pOutputTestPattern;
	ofParameter<string>*	pOutputTestPatternName;
	ofParameter<bool>*		pOutputApply;
	ofParameter<bool>		pUpdateFlash;
	
	ofParameterGroup		InputParameters;
	ofParameterGroup*		OutputParameters;
	ofParameterGroup*		OutputCropParameters;
	ofParameterGroup		saveParameters;
	
	void		getInfoParametersFromDevice();
	void		getInputParametersFromDevice();
	void		getOutputParametersFromDevice() { for (int i=0; i<4; i++) { getOutputParametersFromDevice(i); }; }
	void		getOutputParametersFromDevice(int _index);
	
	void		setInputParametersToDevice();
	void		setOutputParametersToDevice() { for (int i=0; i<4; i++) { setOutputParametersToDevice(i); }; }
	void		setOutputParametersToDevice(int _index);

	void		pScanListner(bool &_value);
	void		pConnectListner(bool &_value);
	void		pPrintSettingsListner(bool &_value);
	void		pInputUpdateListner(bool &_value);
	void		pInputApplyListner(bool &_value);
	void		pUpdateFlashListner(bool &_value);
	
	void		pOutputApplyListner0(bool &_value) { pOutputApplyListner(0, _value); }
	void		pOutputApplyListner1(bool &_value) { pOutputApplyListner(1, _value); }
	void		pOutputApplyListner2(bool &_value) { pOutputApplyListner(2, _value); }
	void		pOutputApplyListner3(bool &_value) { pOutputApplyListner(3, _value); }
	void		pOutputApplyListner(int _index, bool &_value);
		
	void		pWidthListner(int &_value);
	void		pHeightListner(int &_value);
	void		pRefreshRateListner(float &_value);
	void		pCropListner(int &_value);
	
	int			getClosestValue(int _value, const int* _values, int _valueSize);
	float		getClosestValue(float _value, const float* _values, int _valueSize);
	
	bool		blockListners;
};