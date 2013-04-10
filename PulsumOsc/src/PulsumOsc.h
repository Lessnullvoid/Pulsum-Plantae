#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "Sensor.h"

#define OSC_OUT_HOST "localhost"
#define OSC_OUT_PORT 8888
#define OSC_PERIOD 100

///////////////////////////////
//// TODO:
////	- figure out video interaction
////	- figure out min/max stuff with Leslie
////	- figure out osc stuff with Leslie
////	- write xml

class PulsumOsc : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void thresholdCam(ofVideoGrabber &in, ofImage &out);
	void blowUpPolyline(ofPolyline &pl);

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	//////////////////////////////////////////////////////
	vector<string>& updateSerialList();
	vector<string> theSerialList;
	ofxUIDropDownList* guiSerialList;
	bool bUpdateSerialList;

	ofxUICanvas mGui;
	void guiListener(ofxUIEventArgs &args);
	float verticalUnit, horizontalUnit;
	ofVec2f sensorGraphSize;

	ofSerial mSerial;
	ofxOscSender mOscSender;
	unsigned long long lastOscTime;
	vector<Sensor> theSensors;
};
