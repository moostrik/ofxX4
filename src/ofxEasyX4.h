#pragma once

#include "ofMain.h"
#include "ofxX4.h"

class ofxEasyX4{
public:
	
	void		setup(int _deviceNum = 0)	{ X4.setup(_deviceNum); }
	void		open(int _deviceNum)		{ X4.open(_deviceNum); }
	void		close()						{ X4.close(); }
	//			updateFlash() saves current settings to the flash memory of device. According to the documentation this should be used sparingly to avoid wearing out the flash memory.
	void		updateFlash()				{ X4.updateFlash(); }

	
	// GET
	bool		getDeviceInited()			{ return X4.getDeviceInited(); }
	string		getName()					{ return X4.getDeviceName(); }
	string		getSerial()					{ return X4.getDeviceSerial(); }
	bool		getSmoothText()				{ return (X4.getDeviceSmoothness() == X4SMOOTHNESS_LEVEL0); }
	bool		getSmoothVideo()			{ return (X4.getDeviceSmoothness() != X4SMOOTHNESS_LEVEL0); }
	bool		getEqualisation()			{ return (X4.getInputEqualisation() != X4EQUALISATION_LEVEL0); }
	X4EQUALISATION getEqualisationLevel()	{ return X4.getInputEqualisation(); }

	bool		getInputDualLinkStatus()	{ return X4.getInputDualLinkStatus(); }
	float		getInputWidth()				{ return X4.getInputCurrentTimings().HorAddrTime; }
	float		getInputHeight()			{ return X4.getInputCurrentTimings().VerAddrTime; }
	float		getInputRefreshRate()		{ return X4.getInputCurrentTimings().VerFrequency / 1000.0; }
	float		getInputPreferredWidth()	{ return X4.getInputPreferredTimings().HorAddrTime; }
	float		getInputPreferredHeight()	{ return X4.getInputPreferredTimings().VerAddrTime; }
	float		getInputPreferredRefreshRate() { return X4.getInputPreferredTimings().VerFrequency / 1000.0; }
	
	float		getOutputWidth(int _index)					{ return X4.getOutputCurrentTimings(_index).HorAddrTime; }
	float		getOutputHeight(int _index)					{ return X4.getOutputCurrentTimings(_index).VerAddrTime; }
	float		getOutputRefreshRate(int _index)			{ return X4.getOutputCurrentTimings(_index).VerFrequency / 1000.0; }
	bool		getOutputAnalogueStatus(int _index)			{ return (X4.getOutputCurrentTimings(_index).Flags & VDIF_FLAG_DVI) != VDIF_FLAG_DVI; }
	
	float		getOutputDefaultWidth(int _index)			{ return X4.getOutputDefaultTimings(_index).HorAddrTime; }
	float		getOutputDefaultHeight(int _index)			{ return X4.getOutputDefaultTimings(_index).VerAddrTime; }
	float		getOutputDefaultRefreshRate(int _index)		{ return X4.getOutputDefaultTimings(_index).VerFrequency / 1000.0; }
	bool		getOutputDefaultAnalogueStatus(int _index)	{ return (X4.getOutputDefaultTimings(_index).Flags & VDIF_FLAG_DVI) != VDIF_FLAG_DVI; }
	
	ofRectangle	getOutputCrop(int _index);
	ofRectangle	getOutputCropNormalized(int _index);
	X4TRANSFORM	getOutputTransform(int _index)				{ return X4.getOutputTransform(_index); }
	X4IMAGESOURCE getOutputTestPatternMode(int _index)		{ return X4.getOutputTestPattern(_index); } // 0 = testpattern off
//	X4IMAGESOURCE is a weird name for the testpattern enum
	
	// SET
	void		setName(string _name)						{ X4.setDeviceName(_name); }
	void		setSmoothText()								{ X4.setDeviceSmoothness(X4SMOOTHNESS_LEVEL0); }
	void		setSmoothVideo()							{ X4.setDeviceSmoothness(X4SMOOTHNESS_LEVEL1); }
	void		setEqualisationOff()						{ X4.setInputEqualisation(X4EQUALISATION_LEVEL0); }
	void		setEqualisationLevel(X4EQUALISATION _level)	{ X4.setInputEqualisation(_level); }
	
	void		setInputTiming(int _width, int _height, float _refreshRate) { X4.setInputPreferredTimings(_width, _height, _refreshRate); }
	void		setInputWidth(int _width)					{ X4.setInputPreferredTimings(_width, getInputWidth(), getInputRefreshRate()); }
	void		setInputHeight(int _height)					{ X4.setInputPreferredTimings(getInputWidth(), _height, getInputRefreshRate()); }
	void		setInputRefreshRate(float _refreshRate)		{ X4.setInputPreferredTimings(getInputWidth(), getInputWidth(), _refreshRate); }
	
	void		setOutputTiming(int _index, int _width, int _height, float _refreshRate, bool _analogue);
	void		setOutputWidth(int _index, int _width);
	void		setOutputHeight(int _index, int _height);
	void		setOutputRefreshRate(int _index, float _refreshRate);
	void		setOutputCrop(int _index, ofRectangle _crop);
	void		setOutputCropNormalized(int _index, ofRectangle _crop);
	void		setOutputAnalogue(int _index, bool _analogue);
	void		setOutputTransform(int _index, X4TRANSFORM _transformation);
	void		setOutputTestPattern(int _index, X4IMAGESOURCE _source);
	
protected:
	ofxX4		X4;
};