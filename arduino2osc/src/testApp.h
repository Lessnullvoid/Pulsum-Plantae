#pragma once
#define HOST "localhost"
#define PORT 8001

#define BAUD_RATE 57600

#define ANALOG_INPUTS 6


#define SENSOR_VALUE_COUNT 12
#define SENSOR_VALUE_THRESHOLD 5


#include <tr1/memory>
//#include <boost/pointer_cast.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <boost/make_shared.hpp>


//using boost::shared_ptr;
//using boost::enable_shared_from_this;
//using boost::dynamic_pointer_cast;
//using boost::make_shared;

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

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
	void setupArduino(const int & version);
	void updateArduino();
    
    void exit();

	ofImage				bgImage;
	ofTrueTypeFont		font;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
    

	ofPtr<Controller> ctl;
};

