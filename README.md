# README #

openframeworks addon for controlling the datapath X4

* created for of 0.9.0
* ofxX4 for hardcore access
* ofxX4SL extention of ofxX4 for load and save
* ofxEasyX4 for easy of use
* ofxEasyX4Gui for easy of use width ofxGui

* NOTE: in include/X4.h two lines are commnted out: line 17 #include <pshpack1.h> and line 176 #include <poppack.h>  

# KNOWN BUGS
* There is a bug in the OSX API where getNumDevices will always return the number of devices found on start of the application
* There is a bug in the OSX API where getInputPreferredTimings() does not return values, the addon returns getInputCurrentTimings() instead";

# TODO ofxX4#
* better load and unload of DLL
* open from deviceNum and serial
* make Threaded

# TODO ofxEasyX4#
* make reconnect working
* better crop interface