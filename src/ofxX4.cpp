#include "ofxX4.h"

ofxX4::ofxX4() {
	hDLL = 0;
	hDevice = 0;
	deviceNum = -1;
	deviceInited = false;
	// for some reason it is not nessecary on OSX to load the API
	X4EasyLoad(&hDLL);
}

//--------------------------------------------------------------------
ofxX4::~ofxX4() {
	close();
	X4ERROR Error = X4EasyFree(hDLL);
	
	if (Error) {
		ofLogError("ofxX4") << "unable to free X4";
		printError(Error);
		return false;
	}
	
}

//--------------------------------------------------------------------
void ofxX4::setup(int _deviceNum) {
	ofLogNotice("ofxX4") << "number of devices found: " << getNumDevices();

	if (open(_deviceNum)) {
		printInfo();
	}
}

//--------------------------------------------------------------------
bool ofxX4::open(int _deviceNum) {
	ofLogNotice("ofxX4") << "opening x4 " << _deviceNum;
	
	if (deviceInited) close();
	
	if (_deviceNum >= getNumDevices()) {
		ofLogWarning("ofxX4") << "requested devicenum " << _deviceNum << " not in range ";
		return false;
	}
	else {
		X4ERROR Error = 0;
		Error = X4EasyDeviceOpen(hDLL, _deviceNum, &hDevice);
		if (Error) {
			ofLogError("ofxX4") << "unable to open x4 " << _deviceNum;
			printError(Error);
			return false;
		}
		else {
			ofLogNotice("ofxX4") << "opened x4 " << _deviceNum;
			deviceNum = _deviceNum;
			deviceInited = true;
			return true;
		}
	}
}

//--------------------------------------------------------------------
void ofxX4::close() {
	if (deviceInited) {
		X4ERROR Error = 0;
		Error = X4EasyDeviceClose(hDevice);
		if (Error) {
			ofLogError("ofxX4") << "unable to close x4 " << deviceNum;
			printError(Error);
		}
	}
	ofLogNotice("ofxX4") << "closed x4 " << deviceNum;
	hDevice = 0;
	deviceNum = -1;
	deviceInited = false;
}

//--------------------------------------------------------------------
void ofxX4::updateFlash() {
#ifdef DISABLE_UPDATE_TO_FLASH
	ofLogWarning("ofxX4SL") << "update to flash disabled";
#else
	X4ERROR Error = 0;
	Error = X4EasyDeviceUpdateFlash(hDevice);
	if (Error) {
		ofLogError("ofxX4") << "unable to update flash";
		printError(Error);
		return 0;
	}
	else {
		ofLogVerbose("ofxX4") << "flash updated";
	}
#endif
}

//-- GET FUNCTIONS ---------------------------------------------------
//--------------------------------------------------------------------
int ofxX4::getNumDevices() {
	// There is a bug in OSX where getNumDevices will always return the number of devices found on start of the application
	X4ERROR Error = 0;
	unsigned long X4NumDevices;
	Error = X4EasyDeviceGetCount(hDLL, &X4NumDevices);
	if (Error) {
		ofLogError("ofxX4") << "unable to get number of devices";
		printError(Error);
		return 0;
	}
	return (int)X4NumDevices;
}

//-- GET FUNCTIONS -- DEVICE -----------------------------------------
//--------------------------------------------------------------------
string ofxX4::getDeviceName() {
	X4ERROR Error = 0;
	char* name = new char[255];
	strcpy(name, "unknown");
	Error = X4EasyDeviceGetFriendlyNameA(hDevice, name);
	if (Error) {
		ofLogError("ofxX4") << "unable to get device name";
		printError(Error);
		return "nameless";
	}
	else {
		return (string)name;
	}
}

//--------------------------------------------------------------------
string ofxX4::getDeviceSerial() {
	X4ERROR Error = 0;
	char* serial = new char[255];
	strcpy(serial, "unknown");
	Error = X4EasyDeviceGetSerialNumberA(hDLL, deviceNum, serial);
	if (Error) {
		ofLogError("ofxX4") << "unable to get device serial";
		printError(Error);
		return "serialless";
	}
	else {
		string ss = (string)serial;
		ss.erase(0, 2);
		ss.erase(ss.length() - 1, 1);
		return ss;
	}
	
}

//--------------------------------------------------------------------
X4GENLOCK ofxX4::getDeviceFrameLockStatus() {
	X4ERROR Error = 0;
	X4GENLOCK genLock = X4GENLOCK_UNKNOWN;
	Error = X4EasyDeviceGetGenlockStatus(hDevice, &genLock);
	if (Error) {
		ofLogError("ofxX4") << "unable to get device genlock status";
		printError(Error);
	}
	return genLock;
}

//--------------------------------------------------------------------
X4SMOOTHNESS ofxX4::getDeviceSmoothness() {
	X4ERROR Error = 0;
	X4SMOOTHNESS smoothness = X4SMOOTHNESS_LEVEL0;
	Error = X4EasyDeviceGetSmoothness(hDevice, &smoothness);
	if (Error) {
		ofLogError("ofxX4") << "unable to get device smoothness";
		printError(Error);
	}
	return smoothness;
}

//-- GET FUNCTIONS -- INPUT ------------------------------------------
//--------------------------------------------------------------------
X4EQUALISATION ofxX4::getInputEqualisation() {
	X4ERROR Error = 0;
	X4EQUALISATION equalisation = X4EQUALISATION_LEVEL0;
	long isSupported = 0;
	Error = X4EasyInputIsEqualisationSupported(hDevice, &isSupported);
	if (Error) {
		ofLogError("ofxX4") << "unable to get if input equalisation is supported";
		printError(Error);
	}
	if (isSupported) {
		Error = X4EasyInputGetEqualisation(hDevice, &equalisation);
		if (Error) {
			ofLogError("ofxX4") << "unable to get input equalisation";
			printError(Error);
		}
	}
	return equalisation;
}

//--------------------------------------------------------------------
X4TIMINGS ofxX4::getInputPreferredTimings() {
#ifdef PREFFERED_INPUT_QUICK_FIX
	ofLogWarning("ofxX4") << "quick fix: getInputPreferredTimings() does not return values, returning getInputCurrentTimings() instead";
	return getInputCurrentTimings();
#else
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyInputGetPreferredTimingsA(hDevice, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get preferred input timings";
		printError(Error);
	}
	return timings;
#endif
}

//--------------------------------------------------------------------
int ofxX4::getInputPrefferedTimingsMode() {
#ifdef PREFFERED_INPUT_QUICK_FIX
	ofLogWarning("ofxX4") << "quick fix: getInputPreferredTimingsMode() does not return values, returning getInputCurrentTimingsMode() instead";
	return getInputCurrentTimingsMode();
#else
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyInputGetPreferredTimingsA(hDevice, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get preferred input timings";
		printError(Error);
	}
	return (int)timingsMode;
#endif
 
}

//--------------------------------------------------------------------
X4TIMINGS ofxX4::getInputCurrentTimings() {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyInputGetCurrentTimingsA(hDevice, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get current input timings";
		printError(Error);
	}
	return timings;
}

//--------------------------------------------------------------------
int ofxX4::getInputCurrentTimingsMode() {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyInputGetCurrentTimingsA(hDevice, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get current input timings";
		printError(Error);
	}
	return (int)timingsMode;
}

//--------------------------------------------------------------------
bool ofxX4::getInputDualLinkStatus() {
	X4ERROR Error = 0;
	int isDualLink = 0;
	Error = X4EasyInputGetDualLinkStatus(hDevice, &isDualLink, NULL);
	if (Error) {
		ofLogError("ofxX4") << "unable to get dual link input status";
		printError(Error);
	}
	return isDualLink;
}

//-- GET FUNCTIONS -- OUTPUT -----------------------------------------
//--------------------------------------------------------------------
int ofxX4::getOutputCount() {
	X4ERROR Error = 0;
	unsigned long count = 0;
	Error = X4EasyOutputGetCount(hDevice, &count);
	if (Error) {
		ofLogError("ofxX4") << "unable to get output count";
		printError(Error);
	}
	return count;
}

//--------------------------------------------------------------------
X4TIMINGS ofxX4::getOutputPreferredTimings(int _index) {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyOutputGetPreferredTimingsA(hDevice, _index, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get preferred timings for output " << _index;
		printError(Error);
	}
	return timings;
}

//--------------------------------------------------------------------
int ofxX4::getOutputPrefferedTimingsMode(int _index) {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyOutputGetPreferredTimingsA(hDevice, _index, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get preferred timings mode for output " << _index;
		printError(Error);
	}
	return (int)timingsMode;
}

//--------------------------------------------------------------------
X4TIMINGS ofxX4::getOutputCurrentTimings(int _index) {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyOutputGetCurrentTimingsA(hDevice, _index, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get current timings for output " << _index;
		printError(Error);
	}
	return timings;
}

//--------------------------------------------------------------------
int ofxX4::getOutputCurrentTimingsMode(int _index) {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyOutputGetCurrentTimingsA(hDevice, _index, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get current timings mode for output " << _index;
		printError(Error);
	}
	return (int)timingsMode;
}

//--------------------------------------------------------------------
X4TIMINGS ofxX4::getOutputDefaultTimings(int _index) {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyOutputGetDefaultTimingsA(hDevice, _index, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get default timings for output " << _index;
		printError(Error);
	}
	return timings;
}

//--------------------------------------------------------------------
int ofxX4::getOutputDefaultTimingsMode(int _index) {
	X4ERROR Error = 0;
	X4TIMINGS timings;
	clearTimings(timings);
	unsigned long timingsMode = 0;
	Error = X4EasyOutputGetDefaultTimingsA(hDevice, _index, &timings, &timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to get default timings mode for output " << _index;
		printError(Error);
	}
	int dinges = (int)timingsMode;
	return dinges;
}

//--------------------------------------------------------------------
X4TIMINGSOURCE ofxX4::getOutputTimingSource(int _index) {
	X4ERROR Error = 0;
	X4TIMINGSOURCE source = DEFAULT;
	unsigned long refOutput = 0;
	Error = X4EasyOutputGetTimingSource(hDevice, _index, &source, &refOutput);
	if (Error) {
		ofLogError("ofxX4") << "unable to get timing source for output " << _index;
		printError(Error);
	}
	return source;
}

//--------------------------------------------------------------------
int ofxX4::getOutputTimingSourceReference(int _index) {
	X4ERROR Error = 0;
	X4TIMINGSOURCE source = DEFAULT;
	unsigned long refOutput = 0;
	Error = X4EasyOutputGetTimingSource(hDevice, _index, &source, &refOutput);
	if (Error) {
		ofLogError("ofxX4") << "unable to get timing source for output " << _index;
		printError(Error);
	}
	return refOutput;
}

//--------------------------------------------------------------------
X4CROP ofxX4::getOutputCropModeIndependent(int _index) {
	X4ERROR Error = 0;
	X4CROP crop;
	crop.top = 0;
	crop.left = 0;
	crop.right = 0;
	crop.bottom = 0;
	Error = X4EasyOutputGetCropping(hDevice, _index, &crop.top, &crop.left, &crop.right, &crop.bottom);
	if (Error) {
		ofLogError("ofxX4") << "unable to get cropping for output  " << _index;
		printError(Error);
	}
	return crop;
}

//--------------------------------------------------------------------
X4CROP ofxX4::getOutputCrop(int _index) {
	X4OUTPUT MIOutput, output;;
	X4CROP MICrop, crop;
	MICrop = getOutputCropModeIndependent(_index);
	MIOutput.Size = 0;
	MIOutput.top = MICrop.top;
	MIOutput.left = MICrop.left;
	MIOutput.right = MICrop.right;
	MIOutput.bottom = MICrop.bottom;
	MIOutput.transform = ROTATION_NONE;
	output = convertModeIndipendentToInput(MIOutput);
	crop.top = output.top;
	crop.left = output.left;
	crop.right = output.right;
	crop.bottom = output.bottom;
	return crop;
}

//--------------------------------------------------------------------
X4TRANSFORM ofxX4::getOutputTransform(int _index) {
	X4ERROR Error = 0;
	X4TRANSFORM transform = ROTATION_NONE;
	Error = X4EasyOutputGetTransformation(hDevice, _index, &transform);
	if (Error) {
		ofLogError("ofxX4") << "unable to get transform for output " << _index;
		printError(Error);
	}
	return transform;
	
}

//--------------------------------------------------------------------
X4IMAGESOURCE ofxX4::getOutputTestPattern(int _index) {
	X4ERROR Error = 0;
	X4IMAGESOURCE source = X4IMGSRC_CROPPING;
	Error = X4EasyGetImageSource(hDevice, _index, &source);
	if (Error) {
		ofLogError("ofxX4") << "unable to get image source for output " << _index;
		printError(Error);
	}
	return source;
}


//-- SET FUNCTIONS ---------------------------------------------------
//--------------------------------------------------------------------
void ofxX4::setDeviceName(string _name) {
	X4ERROR Error = 0;
	char* cstr = new char[_name.length() + 1];
	strcpy(cstr, _name.c_str());
	Error = X4EasyDeviceSetFriendlyNameA(hDevice, cstr);	if (Error) {
		ofLogError("ofxX4") << "unable to set device name to " << cstr;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "device name set to " << cstr;
	}
	delete [] cstr;
	
}

//--------------------------------------------------------------------
void ofxX4::setDeviceSmoothness(X4SMOOTHNESS _smoothness) {
	X4ERROR Error = 0;
	Error = X4EasyDeviceSetSmoothness(hDevice, _smoothness);
	if (Error) {
		ofLogError("ofxX4") << "unable to set device smoothness to " << _smoothness;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "device smoothness set to " << _smoothness;
	}
}

//--------------------------------------------------------------------
void ofxX4::setInputPreferredTimings(X4TIMINGS _timings, int _mode) {
	X4ERROR Error = 0;
	
	Error = X4EasyInputSetPreferredTimingsA(hDevice, &_timings, (unsigned long)_mode);
	if (Error) {
		ofLogError("ofxX4") << "unable to set preffered input timings to " << (string)_timings.Description << " using mode " << _mode;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "input timings set to " << (string)_timings.Description << " using mode " << _mode;
	}
}

//--------------------------------------------------------------------
void ofxX4::setInputPreferredTimings(int _width, int _height, float _refreshRate, int _mode) {
	X4TIMINGS timings;
	clearTimings(timings);
	timings.HorAddrTime = _width;
	timings.VerAddrTime = _height;
	timings.VerFrequency = _refreshRate * 1000;
	
	X4TIMINGS calcTimings = calculateVideoTimings(timings, _mode);
	setInputPreferredTimings(calcTimings, _mode);
}

//--------------------------------------------------------------------
void ofxX4::setInputEqualisation(X4EQUALISATION _equalisation) {
	X4ERROR Error = 0;
	Error = X4EasyInputSetEqualisation(hDevice, _equalisation);
	if (Error) {
		ofLogError("ofxX4") << "unable to set input equalisation to level " << _equalisation;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "input equalisation set to level " << _equalisation;
	}
}

//--------------------------------------------------------------------
 void ofxX4::setInputDVILinkStatus(X4DVILINKOVERRIDE _override) {
	// not mentioned in documentation
	X4ERROR Error = 0;
	Error = X4EasyInputSetDVILinkState(hDevice, _override);
	if (Error) {
		ofLogError("ofxX4") << "unable to override dual link status";
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "input dual link status overrode to " << _override;
	}
}

//--------------------------------------------------------------------
void ofxX4::setOutputDefaultTimings(int _index, X4TIMINGS _timings, int _mode) {
	X4ERROR Error = 0;
//	X4TIMINGS newTimings = calculateVideoTimings(_timings, _mode);
	Error = X4EasyOutputSetDefaultTimingsA(hDevice, _index, &_timings, _mode);
	if (Error) {
		ofLogError("ofxX4") << "unable to set default timings for output " << _index  << " to " << (string)_timings.Description << " using mode " << _mode;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "output " << _index << " timings set to " << (string)_timings.Description << " using mode " << _mode;
	}
}

//--------------------------------------------------------------------
void ofxX4::setOutputDefaultTimings(int _index, int _width, int _height, float _refreshRate, bool _analoge, int _mode) {
	X4TIMINGS timings;
	clearTimings(timings);
	timings.HorAddrTime = _width;
	timings.VerAddrTime = _height;
	timings.VerFrequency = _refreshRate * 1000;
	if(!_analoge)timings.Flags |= VDIF_FLAG_DVI;
	
	X4TIMINGS calcTimings = calculateVideoTimings(timings, _mode);
	setOutputDefaultTimings(_index, calcTimings, _mode);
}

//--------------------------------------------------------------------
void ofxX4::setOutputTimingSource(int _index, X4TIMINGSOURCE _source, int _refIndex) {
	X4ERROR Error = 0;
	Error = X4EasyOutputSetTimingSource(hDevice, _index, _source, _refIndex);
	if (Error) {
		ofLogError("ofxX4") << "unable to set timing source for output " << _index << " to " << _source;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "output " << _index << " timing source set to " << _source << " with reference " << _refIndex;
	}
}

//--------------------------------------------------------------------
void ofxX4::setOutputCropModeIndependent(int _index, X4CROP _crop) {
	X4ERROR Error = 0;
	Error = X4EasyOutputSetCropping(hDevice, _index, _crop.top, _crop.left, _crop.right, _crop.bottom);
	if (Error) {
		ofLogError("ofxX4") << "unable to set cropping for output " << _index << " to " << _crop.top << " " << _crop.left << " " <<_crop.right << " " <<_crop.bottom;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "output " << _index << " mode independent crop set to " << _crop.top << " " << _crop.left << " " <<_crop.right << " " <<_crop.bottom;
	}
}

//--------------------------------------------------------------------
void ofxX4::setOutputCrop(int _index, X4CROP _crop){
	X4OUTPUT MIOutput, output;
	X4CROP MICrop;
	output.Size = 0;
	output.top = _crop.top;
	output.left = _crop.left;
	output.right = _crop.right;
	output.bottom = _crop.bottom;
	output.transform = ROTATION_NONE;
	MIOutput = convertInputToModeIndipendent(output);
	MICrop.top = MIOutput.top;
	MICrop.left = MIOutput.left;
	MICrop.right = MIOutput.right;
	MICrop.bottom = MIOutput.bottom;
	ofLogVerbose("ofxX4") << "output " << _index << " try to set crop to " << _crop.top << " " << _crop.left << " " <<_crop.right << " " <<_crop.bottom;
	setOutputCropModeIndependent(_index, MICrop);
}

//--------------------------------------------------------------------
void ofxX4::setOutputTransformation(int _index, X4TRANSFORM _transformation) {
	X4ERROR Error = 0;
	Error = X4EasyOutputSetTransformation(hDevice, _index, _transformation);
	if (Error) {
		ofLogError("ofxX4") << "unable to set cropping for output " << _index << " to " << _transformation;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "output " << _index << " transformation set to " << _transformation;
	}
}

//--------------------------------------------------------------------
void ofxX4::setOutputTestPattern(int _index, X4IMAGESOURCE _source) {
	X4ERROR Error = 0;
	Error = X4EasySetImageSource(hDevice, _index, _source);
	if (Error) {
		ofLogError("ofxX4") << "unable to set image source for output " << _index;
		printError(Error);
	}
	else {
		ofLogVerbose("ofxX4") << "output " << _index << " test pattern set to " << _source;
	}
}


//-- HELPER FUNCTIONS ------------------------------------------------
//--------------------------------------------------------------------
X4TIMINGS ofxX4::calculateVideoTimings(X4TIMINGS _timings, int _timingsMode) {
	X4ERROR Error = 0;
	Error = X4EasyCalculateVideoTimingsA(&_timings, _timingsMode);
	if (Error) {
		ofLogError("ofxX4") << "unable to calculate video timings";
		printError(Error);
	}
	return _timings;
}

//--------------------------------------------------------------------
X4OUTPUT ofxX4::convertModeIndipendentToInput(X4OUTPUT _modeIndipendentCropping) {
	if (getInputCurrentTimings().VerAddrTime == 0) {
		ofLogWarning("ofxX4") << "no input signal detected, unable to convert mode independent to input cropping";
		_modeIndipendentCropping.top = 0;
		_modeIndipendentCropping.left = 0;
		_modeIndipendentCropping.right = 0;
		_modeIndipendentCropping.bottom = 0;
		return _modeIndipendentCropping;
	}
	X4ERROR Error = 0;
	X4OUTPUT output;
	clearOutput(output);
	Error = X4EasyCoordinateConvertMIToInput(hDevice, &_modeIndipendentCropping, &output);
	if (Error) {
		ofLogError("ofxX4") << "unable to convert mode independent to input cropping";
		printError(Error);
	}
	return output;
}

//--------------------------------------------------------------------
X4OUTPUT ofxX4::convertInputToModeIndipendent(X4OUTPUT _inputCropping) {
	if (getInputCurrentTimings().VerAddrTime == 0) {
		ofLogWarning("ofxX4") << "no input signal detected, unable to convert input to mode independent cropping";
		_inputCropping.top = 0;
		_inputCropping.left = 0;
		_inputCropping.right = 0;
		_inputCropping.bottom = 0;
		return _inputCropping;
	}
	X4ERROR Error = 0;
	X4OUTPUT modeIndipendentCropping;
	Error = X4EasyCoordinateConvertInputToMI(hDevice, &_inputCropping, &modeIndipendentCropping);
	if (Error) {
		ofLogError("ofxX4") << "unable to convert input to mode independent cropping";
		printError(Error);
	}
	return modeIndipendentCropping;
}


//-- PRINT FUNCTIONS -------------------------------------------------
//--------------------------------------------------------------------
void ofxX4::printInfo() {
	printf("serial: %s \n", getDeviceSerial().c_str());
	printf("name: %s \n", getDeviceName().c_str());
	
	/*	X4SYSTEMINFO systemInfo = getSystemInfo();
	 printf("size: %lu \n", systemInfo.Size);
	 printf("hardware version: %c \n", systemInfo.HardwareVersion);
	 printf("FPGA version: %c \n", systemInfo.FPGAVersion);
	 printf("firmware version: %hu \n", systemInfo.FirmwareVersion);
	 */
}

//--------------------------------------------------------------------
void ofxX4::printSettings(bool expanded) {
	ofLogLevel logLevel = ofGetLogLevel();
	ofSetLogLevel(OF_LOG_WARNING);
	
	printf("\n------------------------------------------------\n");
	printf("\n------------------------------------------------\n");
	printf("SETTINGS \n \n");
	
//	printf("input EDID: %s \n", getInputEDID().c_str());
	printf("input dual link status: %i \n", getInputDualLinkStatus());
	printf("input equalisation: %i \n", getInputEqualisation());
	
	switch (getDeviceFrameLockStatus()) {
		case X4GENLOCK_NONE:
			printf("framelock status: none \n");
			break;
		case X4GENLOCK_MONITOR_MASTER:
			printf("framelock status: outputs locked to eachother \n");
			break;
		case X4GENLOCK_INPUT_MASTER:
			printf("framelock status: outputs locked to input \n");
			break;
		case X4GENLOCK_UNKNOWN:
		default:
			printf("framelock status: unknown \n");
			break;
	}
	
	switch (getDeviceSmoothness()) {
		case X4SMOOTHNESS_LEVEL0:
			printf("smoothness level: text \n");
			break;
		default:
			printf("smoothness level: video \n");
			break;
	}
	
	printf("\n");
	(!expanded)?
	printTimings(getInputCurrentTimings(), getInputCurrentTimingsMode(), "input current"):
	printTimingsExpanded(getInputCurrentTimings(), getInputCurrentTimingsMode(), "input current");
	
	printf("\n");
	(!expanded)?
	printTimings(getInputPreferredTimings(), getInputPrefferedTimingsMode(), "input preferred"):
	printTimingsExpanded(getInputPreferredTimings(), getInputPrefferedTimingsMode(), "input preferred");
	
	printf("\n------------------------------------------------\n");
	printf("output count: %i \n \n", getOutputCount());
	for (int i=0; i<4; i++) {
		int iName = i;//+1;
//		printf("output %i enabled: %i \n", iName, getOutputEnabled(i));
		switch (getOutputTestPattern(i)) {
			case X4IMGSRC_CROPPING:
				printf("output %i test pattern: none \n", iName);
				break;
			case X4IMGSRC_TSTPAT_GREYS:
				printf("output %i test pattern: greys \n", iName);
				break;
			case X4IMGSRC_TSTPAT_COLOUR:
				printf("output %i test pattern: colour \n", iName);
				break;
			case X4IMGSRC_TSTPAT_GREYMOVE:
				printf("output %i test pattern: grey moving \n", iName);
				break;
			case X4IMGSRC_TSTPAT_COLOURMOVE:
				printf("output %i test pattern: colour moving \n", iName);
				break;
			default:
				printf("output %i test pattern: unknown \n", iName);
				break;
		}
		
		printf("\n");
		X4CROP crop = getOutputCrop(i);
		printf("output %i cropping: top: %lu \n", iName, crop.top);
		printf("output %i cropping: left: %lu \n", iName, crop.left);
		printf("output %i cropping: right: %lu \n", iName, crop.right);
		printf("output %i cropping: bottom: %lu \n", iName, crop.bottom);
		
		switch (getOutputTransform(i)) {
			case ROTATION_NONE:
				printf("output %i transform: none \n", iName);
				break;
			case ROTATION_90:
				printf("output %i transform: rotation 90  \n", iName);
				break;
			case ROTATION_180:
				printf("output %i transform: rotation 180  \n", iName);
				break;
			case ROTATION_270:
				printf("output %i transform: rotation 270 \n", iName);
				break;
			case FLIP_HORZ:
				printf("output %i transform: flip horizontal \n", iName);
				break;
			case FLIP_VERT:
				printf("output %i transform: flip vertical \n", iName);
				break;
			default:
				printf("output %i transform: unknown \n", iName);
				break;
		}
		
		printf("\n");
		printf("output %i timing source: %s \n", iName, (getOutputTimingSource(i) == MONITOR)? "monitor" : "default");
		printf("output %i timing source reference: %i \n", iName, getOutputTimingSourceReference(i));
	//	printf("output %i enabled: %i \n", iName, getOutputEnabled(i));
		
		string sName;
		const char* cName;
		
		printf("\n");
		sName = "output " + ofToString(i) + " preferred";
		cName = sName.c_str();
		(!expanded)?
		printTimings(getOutputPreferredTimings(i), getOutputPrefferedTimingsMode(i), cName):
		printTimingsExpanded(getOutputPreferredTimings(i), getOutputPrefferedTimingsMode(i), cName);
		
		printf("\n");
		sName = "output " + ofToString(i) + " current";
		cName = sName.c_str();
		(!expanded)?
		printTimings(getOutputCurrentTimings(i), getOutputCurrentTimingsMode(i), cName):
		printTimingsExpanded(getOutputCurrentTimings(i), getOutputCurrentTimingsMode(i), cName);
		
		printf("\n");
		sName = "output " + ofToString(i) + " default";
		cName = sName.c_str();
		(!expanded)?
		printTimings(getOutputDefaultTimings(i), getOutputDefaultTimingsMode(i), cName):
		printTimingsExpanded(getOutputDefaultTimings(i), getOutputDefaultTimingsMode(i), cName);
		
		printf("\n------------------------------------------------\n");
	}
	
	printf("------------------------------------------------\n");
	
	ofSetLogLevel(logLevel);
}

void ofxX4::printTimings(X4TIMINGS _timings, int _mode, const char* _name) {
	printf("%s timings: width: %lu \n", _name, _timings.HorAddrTime);
	printf("%s timings: height: %lu \n", _name, _timings.VerAddrTime);
	printf("%s timings: refresh: rate %f Hz \n", _name, _timings.VerFrequency / 1000.0);
	switch (_mode) {
		case CUSTOM_TIMING_MODE:
			printf("%s timings: mode: custom \n", _name);
			break;
		case CVT_TIMING_MODE:
			printf("%s timings: mode: CVT \n", _name);
			break;
		case CVTRB_TIMING_MODE:
			printf("%s timings: mode: CVT reduced blanking \n", _name);
			break;
		case GTF_TIMING_MODE:
			printf("%s timings: mode: GTF \n", _name);
			break;
		case SMPTE_TIMING_MODE:
			printf("%s timings: mode: SMPTE \n", _name);
			break;
		default:
			printf("%s timings: mode: unknown \n", _name);
			break;
	}
	
	unsigned short bitwiseFlag = _timings.Flags;
	if ((bitwiseFlag & VDIF_FLAG_INTERLACED) == VDIF_FLAG_INTERLACED)
		printf("%s timings: flag: interlaced \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_HSYNC_POSITIVE) == VDIF_FLAG_HSYNC_POSITIVE)
		printf("%s timings: flag: horizontal positive \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_VSYNC_POSITIVE) == VDIF_FLAG_VSYNC_POSITIVE)
		printf("%s timings: flag: vertical positive \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_READ_ONLY) == VDIF_FLAG_READ_ONLY)
		printf("%s timings: flag: read only \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_NOSIGNAL) == VDIF_FLAG_NOSIGNAL)
		printf("%s timings: flag: no signal \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_OUTOFRANGE) == VDIF_FLAG_OUTOFRANGE)
		printf("%s timings: flag: out of range \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_UNRECOGNISABLE) == VDIF_FLAG_UNRECOGNISABLE)
		printf("%s timings: flag: unrecognisable \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_NONVESA) == VDIF_FLAG_NONVESA)
		printf("%s timings: flag: non VESA \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_SDI) == VDIF_FLAG_SDI)
		printf("%s timings: flag: SDI \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_DVI) == VDIF_FLAG_DVI)
		printf("%s timings: flag: DVI \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_DVI_DUAL_LINK) == VDIF_FLAG_DVI_DUAL_LINK)
		printf("%s timings: flag: DVI dual link \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_VIDEO) == VDIF_FLAG_VIDEO)
		printf("%s timings: flag: video \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_HD_MODE) == VDIF_FLAG_HD_MODE)
		printf("%s timings: flag: HD mode \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_YPRPB_CANDIDATE) == VDIF_FLAG_YPRPB_CANDIDATE)
		printf("%s timings: flag: YPRPB canidate \n", _name);
}

void ofxX4::printTimingsExpanded(X4TIMINGS _timings, int _mode, const char* _name) {
	
	printf("%s timings: Description %s \n", _name, _timings.Description);
	printf("%s timings: horizontal frequency: %f kHz \n", _name, _timings.HorFrequency / 1000.0);
	printf("%s timings: vertical frequency: %f Hz \n", _name, _timings.VerFrequency / 1000.0);
	printf("%s timings: pixel clock: %f kHz \n", _name, _timings.PixelClock / 1000.0);
	
	switch (_mode) {
		case CUSTOM_TIMING_MODE:
			printf("%s timings: mode: custom \n", _name);
			break;
		case CVT_TIMING_MODE:
			printf("%s timings: mode: CVT \n", _name);
			break;
		case CVTRB_TIMING_MODE:
			printf("%s timings: mode: CVT reduced blanking \n", _name);
			break;
		case GTF_TIMING_MODE:
			printf("%s timings: mode: GTF \n", _name);
			break;
		case SMPTE_TIMING_MODE:
			printf("%s timings: mode: SMPTE \n", _name);
			break;
		default:
			printf("%s timings: mode: unknown \n", _name);
			break;
	}
	
	unsigned short bitwiseFlag = _timings.Flags;
	if ((bitwiseFlag & VDIF_FLAG_INTERLACED) == VDIF_FLAG_INTERLACED)
		printf("%s timings: flag: interlaced \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_HSYNC_POSITIVE) == VDIF_FLAG_HSYNC_POSITIVE)
		printf("%s timings: flag: horizontal positive \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_VSYNC_POSITIVE) == VDIF_FLAG_VSYNC_POSITIVE)
		printf("%s timings: flag: vertical positive \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_READ_ONLY) == VDIF_FLAG_READ_ONLY)
		printf("%s timings: flag: read only \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_NOSIGNAL) == VDIF_FLAG_NOSIGNAL)
		printf("%s timings: flag: no signal \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_OUTOFRANGE) == VDIF_FLAG_OUTOFRANGE)
		printf("%s timings: flag: out of range \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_UNRECOGNISABLE) == VDIF_FLAG_UNRECOGNISABLE)
		printf("%s timings: flag: unrecognisable \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_NONVESA) == VDIF_FLAG_NONVESA)
		printf("%s timings: flag: non VESA \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_SDI) == VDIF_FLAG_SDI)
		printf("%s timings: flag: SDI \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_DVI) == VDIF_FLAG_DVI)
		printf("%s timings: flag: DVI \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_DVI_DUAL_LINK) == VDIF_FLAG_DVI_DUAL_LINK)
		printf("%s timings: flag: DVI dual link \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_VIDEO) == VDIF_FLAG_VIDEO)
		printf("%s timings: flag: video \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_HD_MODE) == VDIF_FLAG_HD_MODE)
		printf("%s timings: flag: HD mode \n", _name);
	if ((bitwiseFlag & VDIF_FLAG_YPRPB_CANDIDATE) == VDIF_FLAG_YPRPB_CANDIDATE)
		printf("%s timings: flag: YPRPB canidate \n", _name);
	
	printf("\n");
	printf("%s timings: horizontal address time: %lu \n", _name, _timings.HorAddrTime);
	printf("%s timings: horizontal right border: %lu \n", _name, _timings.HorRightBorder);
	printf("%s timings: horizontal front porch: %lu \n", _name, _timings.HorFrontPorch);
	printf("%s timings: horizontal sync time: %lu \n", _name, _timings.HorSyncTime);
	printf("%s timings: horizontal back porch: %lu \n", _name, _timings.HorBackPorch);
	printf("%s timings: horizontal left border: %lu \n", _name, _timings.HorLeftBorder);
	printf("\n");
	printf("%s timings: vertical address time: %lu \n", _name, _timings.VerAddrTime);
	printf("%s timings: vertical right border: %lu \n", _name, _timings.VerBottomBorder);
	printf("%s timings: vertical front porch: %lu \n", _name, _timings.VerFrontPorch);
	printf("%s timings: vertical sync time: %lu \n", _name, _timings.VerSyncTime);
	printf("%s timings: vertical back porch: %lu \n", _name, _timings.VerBackPorch);
	printf("%s timings: vertical left border: %lu \n", _name, _timings.VerTopBorder);
}

//--------------------------------------------------------------------
void ofxX4::printError(unsigned long _Error) {
	switch (_Error) {
		case X4ERROR_OUT_OF_MEMORY:
			ofLogError("ofxX4") << "X4ERROR_OUT_OF_MEMORY";
			break;
		case X4ERROR_BUFFER_TOO_SMALL:
			ofLogError("ofxX4") << "X4ERROR_BUFFER_TOO_SMALL";
			break;
		case X4ERROR_INVALID_HANDLE:
			ofLogError("ofxX4") << "X4ERROR_INVALID_HANDLE";
			break;
		case X4ERROR_INVALID_DEVICE:
			ofLogError("ofxX4") << "X4ERROR_INVALID_DEVICE";
			break;
		case X4ERROR_INVALID_INPUT:
			ofLogError("ofxX4") << "X4ERROR_INVALID_INPUT";
			break;
		case X4ERROR_INVALID_OUTPUT:
			ofLogError("ofxX4") << "X4ERROR_INVALID_OUTPUT";
			break;
		case X4ERROR_INVALID_METHOD:
			ofLogError("ofxX4") << "X4ERROR_INVALID_METHOD";
			break;
		case X4ERROR_MUTEX_FAIL:
			ofLogError("ofxX4") << "X4ERROR_MUTEX_FAIL";
			break;
		case X4ERROR_EDID_READ_FAIL:
			ofLogError("ofxX4") << "X4ERROR_EDID_READ_FAIL";
			break;
		case X4ERROR_EDID_WRITE_FAIL:
			ofLogError("ofxX4") << "X4ERROR_EDID_WRITE_FAIL";
			break;
		case X4ERROR_INVALID_EDID:
			ofLogError("ofxX4") << "X4ERROR_INVALID_EDID";
			break;
		case X4ERROR_UNSUPPORTED:
			ofLogError("ofxX4") << "X4ERROR_UNSUPPORTED";
			break;
			
		case X4USBERROR_INVALID_HANDLE :
			ofLogError("ofxX4") << "USB_INVALID_HANDLE";
			break;
		case X4USBERROR_READ_ERROR :
			ofLogError("ofxX4") << "USB_READ_ERROR";
			break;
		case X4USBERROR_RX_QUEUE_NOT_READY :
			ofLogError("ofxX4") << "USB_RX_QUEUE_NOT_READY";
			break;
		case X4USBERROR_WRITE_ERROR :
			ofLogError("ofxX4") << "USB_WRITE_ERROR";
			break;
		case X4USBERROR_RESET_ERROR :
			ofLogError("ofxX4") << "USB_RESET_ERROR";
			break;
		case X4USBERROR_INVALID_PARAMETER :
			ofLogError("ofxX4") << "USB_INVALID_PARAMETER";
			break;
		case X4USBERROR_INVALID_REQUEST_LENGTH :
			ofLogError("ofxX4") << "USB_INVALID_REQUEST_LENGTH";
			break;
		case X4USBERROR_DEVICE_IO_FAILED :
			ofLogError("ofxX4") << "USB_DEVICE_IO_FAILED";
			break;
		case X4USBERROR_INVALID_BAUDRATE :
			ofLogError("ofxX4") << "USB_INVALID_BAUDRATE";
			break;
		case X4USBERROR_FUNCTION_NOT_SUPPORTED :
			ofLogError("ofxX4") << "USB_FUNCTION_NOT_SUPPORTED";
			break;
		case X4USBERROR_GLOBAL_DATA_ERROR :
			ofLogError("ofxX4") << "USB_GLOBAL_DATA_ERROR";
			break;
		case X4USBERROR_SYSTEM_ERROR_CODE :
			ofLogError("ofxX4") << "USB_SYSTEM_ERROR_CODE";
			break;
		case X4USBERROR_READ_TIMED_OUT :
			ofLogError("ofxX4") << "USB_READ_TIMED_OUT";
			break;
		case X4USBERROR_WRITE_TIMED_OUT :
			ofLogError("ofxX4") << "USB_WRITE_TIMED_OUT";
			break;
		case X4USBERROR_IO_PENDING :
			ofLogError("ofxX4") << "USB_IO_PENDING";
			break;
		case X4USBERROR_DEVICE_NOT_FOUND :
			ofLogError("ofxX4") << "USB_DEVICE_NOT_FOUND";
			break;
		case X4USBERROR_NACK :
			ofLogError("ofxX4") << "USB_NACK";
			break;
			
		case X4ERROR_NO_ERROR:
		default:
			ofLogVerbose("ofxX4") << "X4ERROR_NO_ERROR";
			break;
	}
}

void ofxX4::clearTimings(X4TIMINGS &_timings) {

	strcpy(_timings.Description, "unknown");
	
	_timings.HorFrequency = 0;
	_timings.VerFrequency = 0;
	_timings.PixelClock = 0;
	_timings.Flags = 0x0000;
	
	_timings.HorAddrTime = 0;
	_timings.HorRightBorder =  0;
	_timings.HorFrontPorch =  0;
	_timings.HorSyncTime =  0;
	_timings.HorBackPorch =  0;
	_timings.HorLeftBorder =  0;
	
	_timings.VerAddrTime =  0;
	_timings.VerBottomBorder =  0;
	_timings.VerFrontPorch =  0;
	_timings.VerSyncTime =  0;
	_timings.VerBackPorch =  0;
	_timings.VerTopBorder =  0;
}

void ofxX4::clearOutput(X4OUTPUT &_output) {
	_output.Size = 0;
	_output.top = 0;
	_output.left = 0;
	_output.right = 0;
	_output.bottom = 0;
	_output.transform = 0;
	
}
