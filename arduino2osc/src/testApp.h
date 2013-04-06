#pragma once
#define HOST "localhost"
#define PORT 8001

#define BAUD_RATE 57600

#define ANALOG_INPUTS 6


#define SENSOR_VALUE_COUNT 12
#define SENSOR_VALUE_THRESHOLD 5


//#include <tr1/memory>

#include "ofMain.h"
#include "ofEvents.h"

#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#include "gui.h"
#include "sistema.h"
#include "Funcs.h"
#include "Controller.h"

class testApp : public ofSimpleApp{

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
	ofPtr<Controller> ctl;
};

