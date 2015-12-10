#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "ofxX4.h"


class ofxX4SL : public ofxX4{
public:
	void	saveSettings(string _fileName);
	void	loadSettings(string _fileName);
	
	void	loadVQS(string _fileName);

private:
	void	saveTimings(ofxXmlSettings _xml, X4TIMINGS _timings);
	X4TIMINGS	loadTimings(ofxXmlSettings _xml);
	bool	checkTag(ofxXmlSettings &_xml,  string _tag, string _defaultvalue = "");
	
	void	loadVQSTimings(ofBuffer::Line &_it, X4TIMINGS &_timings, int &_mode, bool _talk);
	void	loadVQSImage(ofBuffer::Line &_it, X4CROP &_crop, X4TRANSFORM &_transformation, X4IMAGESOURCE &_source, bool _talk);
	void	loadVQSSource(ofBuffer::Line &_it, X4TIMINGSOURCE &_source, int &_reference, bool _talk);
	
};