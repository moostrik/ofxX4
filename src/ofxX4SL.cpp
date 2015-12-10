//
//  ofxX4SL.cpp
//  ofxX4
//
//  Created by Ties East on 23/9/15.
//
//

#include "ofxX4SL.h"


void ofxX4SL::saveSettings(string _fileName) {
	ofLogNotice("ofxX4SL") << "save to file " << _fileName ;
	
	ofxXmlSettings	xml;
	
	xml.addValue("name", getDeviceName());
	xml.addValue("serial", getDeviceSerial());
	xml.addTag("input");
	xml.pushTag("input");
	 xml.addValue("smoothness", getDeviceSmoothness());
	 xml.addValue("equalisation", getInputEqualisation());
	 xml.addTag("timings");
	 xml.pushTag("timings");
	  xml.addValue("mode", getInputPrefferedTimingsMode());
	  saveTimings(xml, getInputPreferredTimings());
  	 xml.popTag();
	xml.popTag();
	
	for (int i=0; i<4; i++) {
		xml.addTag("output_" + ofToString(i));
		xml.pushTag("output_" + ofToString(i));
		 xml.addValue("test_pattern", getOutputTestPattern(i));
		 xml.pushTag("timings_source");
		  xml.addValue("source", getOutputTimingSource(i));
		  xml.addValue("reference", getOutputTimingSourceReference(i));
		 xml.popTag();
		 xml.addTag("timings");
		 xml.pushTag("timings");
		  xml.addValue("mode", getOutputDefaultTimingsMode(i));
		  saveTimings(xml, getOutputDefaultTimings(i));
		 xml.popTag();
		 xml.addTag("crop");
		 xml.pushTag("crop");
		  X4CROP crop = getOutputCrop(i);
		  xml.addValue("top", (int)crop.top);
		  xml.addValue("left", (int)crop.left);
		  xml.addValue("right", (int)crop.right);
		  xml.addValue("bottom", (int)crop.bottom);
		 xml.popTag();
		 xml.addValue("transform", getOutputTransform(i));
		xml.popTag();
	}
	xml.save(_fileName);
}

//--------------------------------------------------------------------
void ofxX4SL::loadSettings(string _fileName) {
	ofLogNotice("ofxX4SL") << "load from file " << _fileName ;
	
	ofxXmlSettings	xml;
	if (!xml.loadFile(_fileName)){
		ofLogWarning("ogxX4SL") << "can not load from file: " << _fileName;
		return;
	}
	
	if(checkTag(xml, "name")) setDeviceName(xml.getValue("name", "noname"));
	if(checkTag(xml, "input")) {
		xml.pushTag("input");
		if(checkTag(xml, "smoothness"))  setDeviceSmoothness((X4SMOOTHNESS)xml.getValue("smoothness", 1));
		
		if(checkTag(xml, "equalisation")) setInputEqualisation((X4EQUALISATION)xml.getValue("equalisation", 0));
		
		if(checkTag(xml, "timings")) {
			xml.pushTag("timings");
			checkTag(xml, "mode", "2");
			setInputPreferredTimings(loadTimings(xml), xml.getValue("mode", 2));
			xml.popTag();
		}
		xml.popTag();
	}
	for (int i=0; i<4; i++) {
		string outputName = "output_" + ofToString(i);
		
		if(checkTag(xml, outputName)) {
			xml.pushTag(outputName);
			
			if(checkTag(xml, "timings_source")) {
				xml.pushTag("timings_source");
				checkTag(xml, "source", "1");
				checkTag(xml, "reference", ofToString(i));
				setOutputTimingSource(i, (X4TIMINGSOURCE)xml.getValue("source", 1), xml.getValue("reference", i));
				xml.popTag();
			}
			
			if(checkTag(xml, "timings")) {
				xml.pushTag("timings");
				checkTag(xml, "mode", "2");
				setOutputDefaultTimings(i, loadTimings(xml), xml.getValue("mode", 2));
				xml.popTag();
			}
			
			if(checkTag(xml, "crop")) {
				xml.pushTag("crop");
				X4CROP crop;
				if(checkTag(xml, "top")) crop.top = xml.getValue("top", 0);
				if(checkTag(xml, "left")) crop.left = xml.getValue("left", 0);
				if(checkTag(xml, "right")) crop.right = xml.getValue("right", 65535);
				if(checkTag(xml, "bottom"))	crop.bottom = xml.getValue("bottom", 65535);
				setOutputCrop(i, crop);
				xml.popTag();
			}
			if(checkTag(xml, "transform")) setOutputTransformation(i, (X4TRANSFORM)xml.getValue("transform", 0));
			if(checkTag(xml, "test_pattern")) setOutputTestPattern(i, (X4IMAGESOURCE)xml.getValue("test_pattern", 0));
			xml.popTag();
		}
	}
	updateFlash();
}

//--------------------------------------------------------------------
void ofxX4SL::saveTimings(ofxXmlSettings _xml, X4TIMINGS _timings) {
	
	string dinges = (string)_timings.Description;
	cout << dinges << endl;
	
	_xml.addValue("description", (string)_timings.Description);
	_xml.addValue("HorFrequency", (int)_timings.HorFrequency);
	_xml.addValue("VerFrequency", (int)_timings.VerFrequency);
	_xml.addValue("PixelClock", (int)_timings.PixelClock);
	
	_xml.addValue("HorAddrTime", (int)_timings.HorAddrTime);
	_xml.addValue("HorRightBorder", (int)_timings.HorRightBorder);
	_xml.addValue("HorFrontPorch", (int)_timings.HorFrontPorch);
	_xml.addValue("HorSyncTime", (int)_timings.HorSyncTime);
	_xml.addValue("HorBackPorch", (int)_timings.HorBackPorch);
	_xml.addValue("HorLeftBorder", (int)_timings.HorLeftBorder);
	
	_xml.addValue("VerAddrTime", (int)_timings.VerAddrTime);
	_xml.addValue("VerBottomBorder", (int)_timings.VerBottomBorder);
	_xml.addValue("VerFrontPorch", (int)_timings.VerFrontPorch);
	_xml.addValue("VerSyncTime", (int)_timings.VerSyncTime);
	_xml.addValue("VerBackPorch", (int)_timings.VerBackPorch);
	_xml.addValue("VerTopBorder", (int)_timings.VerTopBorder);
	
	unsigned short bitwiseFlag = _timings.Flags;
	if ((bitwiseFlag & VDIF_FLAG_INTERLACED) == VDIF_FLAG_INTERLACED)
		_xml.addValue("flag", "INTERLACED");
	if ((bitwiseFlag & VDIF_FLAG_HSYNC_POSITIVE) == VDIF_FLAG_HSYNC_POSITIVE)
		_xml.addValue("flag", "HSYNC_POSITIVE");
	if ((bitwiseFlag & VDIF_FLAG_VSYNC_POSITIVE) == VDIF_FLAG_VSYNC_POSITIVE)
		_xml.addValue("flag", "VSYNC_POSITIVE");
	if ((bitwiseFlag & VDIF_FLAG_SDI) == VDIF_FLAG_SDI)
		_xml.addValue("flag", "SDI");
	if ((bitwiseFlag & VDIF_FLAG_DVI) == VDIF_FLAG_DVI)
		_xml.addValue("flag", "DVI");
	if ((bitwiseFlag & VDIF_FLAG_DVI_DUAL_LINK) == VDIF_FLAG_DVI_DUAL_LINK)
		_xml.addValue("flag", "DVI_DUAL_LINK");
	if ((bitwiseFlag & VDIF_FLAG_VIDEO) == VDIF_FLAG_VIDEO)
		_xml.addValue("flag", "VIDEO");
	
}

//--------------------------------------------------------------------
X4TIMINGS ofxX4SL::loadTimings(ofxXmlSettings _xml) {
	X4TIMINGS timings;
	if(checkTag(_xml, "description")) strcpy(timings.Description, _xml.getValue("description", "unknown").c_str());
	
	if(checkTag(_xml, "HorFrequency")) timings.HorFrequency = _xml.getValue("HorFrequency", 0);
	if(checkTag(_xml, "VerFrequency")) timings.VerFrequency = _xml.getValue("VerFrequency", 0);
	if(checkTag(_xml, "PixelClock")) timings.PixelClock = _xml.getValue("PixelClock", 0);
	
	if(checkTag(_xml, "HorAddrTime")) timings.HorAddrTime = _xml.getValue("HorAddrTime", 0);
	if(checkTag(_xml, "HorRightBorder")) timings.HorRightBorder = _xml.getValue("HorRightBorder", 0);
	if(checkTag(_xml, "HorFrontPorch")) timings.HorFrontPorch = _xml.getValue("HorFrontPorch", 0);
	if(checkTag(_xml, "HorSyncTime")) timings.HorSyncTime = _xml.getValue("HorSyncTime", 0);
	if(checkTag(_xml, "HorBackPorch")) timings.HorBackPorch = _xml.getValue("HorBackPorch", 0);
	if(checkTag(_xml, "HorLeftBorder")) timings.HorLeftBorder = _xml.getValue("HorLeftBorder", 0);
	
	if(checkTag(_xml, "VerAddrTime")) timings.VerAddrTime = _xml.getValue("VerAddrTime", 0);
	if(checkTag(_xml, "VerBottomBorder")) timings.VerBottomBorder = _xml.getValue("VerBottomBorder", 0);
	if(checkTag(_xml, "VerFrontPorch")) timings.VerFrontPorch = _xml.getValue("VerFrontPorch", 0);
	if(checkTag(_xml, "VerSyncTime")) timings.VerSyncTime = _xml.getValue("VerSyncTime", 0);
	if(checkTag(_xml, "VerBackPorch")) timings.VerBackPorch = _xml.getValue("VerBackPorch", 0);
	if(checkTag(_xml, "VerTopBorder")) timings.VerTopBorder = _xml.getValue("VerTopBorder", 0);
	
	timings.Flags = 0x0000;
	
	for (int f=0; f<_xml.getNumTags("flag"); f++) {
		string foundFlag = _xml.getValue("flag", "", f);
		if (foundFlag == "INTERLACED")
			timings.Flags |= VDIF_FLAG_INTERLACED;
		if (foundFlag == "HSYNC_POSITIVE")
			timings.Flags |= VDIF_FLAG_HSYNC_POSITIVE;
		if (foundFlag == "VSYNC_POSITIVE")
			timings.Flags |= VDIF_FLAG_VSYNC_POSITIVE;
		if (foundFlag == "SDI")
			timings.Flags |= VDIF_FLAG_SDI;
		if (foundFlag == "DVI")
			timings.Flags |= VDIF_FLAG_DVI;
		if (foundFlag == "DVI_DUAL_LINK")
			timings.Flags |= VDIF_FLAG_DVI_DUAL_LINK;
		if (foundFlag == "VIDEO")
			timings.Flags |= VDIF_FLAG_VIDEO;
	}
	
	return timings;
}

//--------------------------------------------------------------------
bool ofxX4SL::checkTag(ofxXmlSettings &_xml, string _tag, string _defaultvalue) {
	bool tE = _xml.tagExists(_tag);
	if (!tE) {
		if (_defaultvalue == "") ofLogWarning("ofxX4SL") << "can not find tag: " << _tag;
		else ofLogWarning("ofxX4SL") << "can not find tag: " << _tag << ", defaulting to: " << _defaultvalue;
	}
	return tE;
}

//-- VQS FILES (from windows X4 app) ---------------------------------
//--------------------------------------------------------------------
void ofxX4SL::loadVQS(string _fileName) {
	ofLogNotice("ofxX4SL") << "load from file " << _fileName ;
	
	ofBuffer buffer = ofBufferFromFile(_fileName);
	
	if(buffer.size()) {
		//	string lastLine;
		for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
			
			string line = *it;
			string findvalue;
			int eq;
			bool talk = false;
			
			line = *it;
			if(talk) cout << line << endl;
			
			
			if (line.find("Input Smoothness 1") + 1 > 0 ) {
				line = *(++it);
				findvalue = line;
				eq = findvalue.find("=");
				findvalue.erase(0, eq+1);
				setDeviceSmoothness((X4SMOOTHNESS)ofToInt(findvalue));
				if(talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
			}
			
			if (line.find("Input Equalisation Timings 1") + 1 > 0 ) {
				line = *(++it);
				findvalue = line;
				eq = findvalue.find("=");
				findvalue.erase(0, eq+1);
				setInputEqualisation((X4EQUALISATION)ofToInt(findvalue));
				if(talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
			}
			
			if (line.find("Input Preferred Timings 1") + 1 > 0 ) {
				X4TIMINGS timings;
				clearTimings(timings);
				int mode;
				loadVQSTimings(it, timings, mode, talk);
				setInputPreferredTimings(timings, mode);
			}
			
			for (int i=0; i<4; i++) {
				if (line.find("Output Settings " + ofToString(i)) + 1 > 0 ) {
					X4TIMINGSOURCE source;
					int reference;
					loadVQSSource(it, source, reference, talk);
					setOutputTimingSource(i, source, i); // enable is not reference
				}
				
				if (line.find("Output Default Timings " + ofToString(i)) + 1 > 0 ) {
					X4TIMINGS timings;
					clearTimings(timings);
					int mode;
					loadVQSTimings(it, timings, mode, talk);
					setOutputDefaultTimings(i, timings, mode);
				}
				
				if (line.find("Output Image " + ofToString(i)) + 1 > 0 ) {
					X4CROP crop;
					X4TRANSFORM transformation;
					X4IMAGESOURCE source;
					loadVQSImage(it, crop, transformation, source, talk);
					setOutputCropModeIndependent(i, crop);
					setOutputTransformation(i, transformation);
					setOutputTestPattern(i, source);
				}
			}
		}
		updateFlash();
	}
}

//--------------------------------------------------------------------
void ofxX4SL::loadVQSTimings(ofBuffer::Line &_it, X4TIMINGS &_timings, int &_mode, bool _talk) {
	string line;
	string findvalue;
	int eq;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorFrequency = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.PixelClock = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerFrequency = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	unsigned short bitwiseFlag = ofToInt(findvalue);
	if ((bitwiseFlag & VDIF_FLAG_HSYNC_POSITIVE) == VDIF_FLAG_HSYNC_POSITIVE);
	if ((bitwiseFlag & VDIF_FLAG_VSYNC_POSITIVE) == VDIF_FLAG_VSYNC_POSITIVE);
	if ((bitwiseFlag & VDIF_FLAG_DVI) == VDIF_FLAG_DVI);
	_timings.Flags = bitwiseFlag;
//	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_mode = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorAddrTime = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorRightBorder = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorFrontPorch = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorSyncTime = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorBackPorch = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.HorLeftBorder = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerAddrTime = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerBottomBorder = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerFrontPorch = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerSyncTime = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerBackPorch = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_timings.VerTopBorder = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
}

//--------------------------------------------------------------------
void ofxX4SL::loadVQSImage(ofBuffer::Line &_it, X4CROP &_crop, X4TRANSFORM &_transformation, X4IMAGESOURCE &_source, bool _talk) {
	string line;
	string findvalue;
	int eq;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_crop.top = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_crop.left = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_crop.right = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_crop.bottom = (unsigned long)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_source = (X4IMAGESOURCE)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_transformation = (X4TRANSFORM)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
}

//--------------------------------------------------------------------
void ofxX4SL::loadVQSSource(ofBuffer::Line &_it, X4TIMINGSOURCE &_source, int &_reference, bool _talk) {
	string line;
	string findvalue;
	int eq;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_source = (X4TIMINGSOURCE)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
	
	line = *(++_it);
	findvalue = line;
	eq = findvalue.find("=");
	findvalue.erase(0, eq+1);
	_reference = (X4TIMINGSOURCE)ofToInt(findvalue);
	if(_talk) ofLogVerbose ("ofxX4") << line << " " << findvalue;
}