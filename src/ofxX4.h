#pragma once

#include "ofMain.h"

//#include "errno.h"
//#include "wchar.h"

#ifndef WIN32
#include "datatypes.h"
#endif
//#include "api.h"
#include "vdif.h"
#include "vdifDefs.h"
#include "X4.h"
#include "X4Error.h"
#include "X4Easy.h"

//#define PREFFERED_INPUT_QUICK_FIX
//#define UPDATE_TO_FLASH				// disable when testing



#define X4TIMINGS VDIFA

typedef struct _X4CROP
{
	unsigned long  top;
	unsigned long  left;
	unsigned long  right;
	unsigned long  bottom;
}  X4CROP;

class ofxX4{
public:
	ofxX4();
	virtual ~ofxX4();

	void			setup(int _deviceNum = 0);
	bool			open(int _deviceNum);
	void			close();
	void			updateFlash();  // saves current settings to the flash memory of device,
									// according to the documentation this should be used sparingly to avoid wearing out the flash memory.
	// GET FUNCTIONS
	int				getNumDevices();
	
	int				getDeviceNum() { return deviceNum; }
	bool			getDeviceInited() { return deviceInited; }
	string			getDeviceSerial();
	string			getDeviceName();
	X4GENLOCK		getDeviceFrameLockStatus();
	X4SMOOTHNESS	getDeviceSmoothness();
	
	X4TIMINGS		getInputPreferredTimings();
	int				getInputPrefferedTimingsMode();
	X4EQUALISATION	getInputEqualisation();
//	string			getInputEDID(); // this makes no sense
	X4TIMINGS		getInputCurrentTimings();
	int				getInputCurrentTimingsMode();
	bool			getInputDualLinkStatus();
	
	int				getOutputCount();
	X4TIMINGS		getOutputPreferredTimings(int _index);
	int				getOutputPrefferedTimingsMode(int _index);
	X4TIMINGS		getOutputCurrentTimings(int _index);
	int				getOutputCurrentTimingsMode(int _index);
	X4TIMINGS		getOutputDefaultTimings(int _index);
	int				getOutputDefaultTimingsMode(int _index);
	X4TIMINGSOURCE	getOutputTimingSource(int _index);
	int				getOutputTimingSourceReference(int _index);
	X4CROP			getOutputCrop(int _index);
	X4CROP			getOutputCropModeIndependent(int _index);
	X4TRANSFORM		getOutputTransform(int _index);
	X4IMAGESOURCE	getOutputTestPattern(int _index);
	
	
	// SET FUNCTIONS
	void			setDeviceName(string _name);
	void			setDeviceSmoothness(X4SMOOTHNESS _smoothness);
	
	void			setInputPreferredTimings(X4TIMINGS _timings, int _mode);
	void			setInputPreferredTimings(int _width, int _height, float _refreshRate, int _mode = 2);
	void			setInputEqualisation(X4EQUALISATION _equalisation);
//	void			setInputEDID(string _EDID); // this makes no sense
	void			setInputDVILinkStatus(X4DVILINKOVERRIDE _override);  // what is this function for?
	
	void			setOutputDefaultTimings(int _index, X4TIMINGS _timings, int _mode);
	void			setOutputDefaultTimings(int _index, int _width, int _height, float _refreshRate, bool _analoge = false, int _mode = 2);
	void			setOutputTimingSource(int _index, X4TIMINGSOURCE _source, int _refIndex);
	void			setOutputCrop(int _index, X4CROP _crop);
	void			setOutputCropModeIndependent(int _index, X4CROP _crop);
	void			setOutputTransformation(int _index, X4TRANSFORM _transformation);
	void			setOutputTestPattern(int _index, X4IMAGESOURCE _source);
	
	
	// HELPER FUNCTIONS
	X4TIMINGS		calculateVideoTimings(X4TIMINGS _timings, int _mode);
	X4OUTPUT		convertModeIndipendentToInput(X4OUTPUT _modeIndipendentCropping);
	X4OUTPUT		convertInputToModeIndipendent(X4OUTPUT _inputCropping);

	void			printInfo();
	void			printSettings(bool expanded = false);
		
protected:
	
	HX4DLL			hDLL;
	HX4DEVICE		hDevice;
	int				deviceNum;
	bool			deviceInited;
	
	void			printError(unsigned long _Error);
	void			printTimings(X4TIMINGS _timings, int _timingsMode, const char* _name);
	void			printTimingsExpanded(X4TIMINGS _timings, int _timingsMode, const char* _name);
	
	void			clearTimings(X4TIMINGS &_timings);
	void			clearOutput(X4OUTPUT &_output);
	
};