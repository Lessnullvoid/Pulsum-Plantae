#include "PulsumOsc.h"

//--------------------------------------------------------------
void PulsumOsc::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	// grids for drawing objects
	verticalUnit = ofGetHeight()/18;
	horizontalUnit = ofGetWidth()/25;
	sensorGraphSize = ofVec2f(horizontalUnit*12,verticalUnit*4);
	videoSize = ofVec2f(ofGetWidth(), ofGetHeight());
	
	//////////////// the serial GUI
	mGui.setFont("verdana.ttf");
	////// Serial Port list
	guiSerialList = (ofxUIDropDownList*) mGui.addWidgetDown(new ofxUIDropDownList(0, 0, horizontalUnit*6, "Serial List", theSerialList,0));
	guiSerialList->setAutoClose(true);
	
	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	mGui.setPosition(horizontalUnit, verticalUnit);
	ofAddListener(mGui.newGUIEvent,this,&PulsumOsc::guiListener);
	
	//////////////// start osc
	mOscSender.setup(OSC_OUT_HOST, OSC_OUT_PORT);
	lastOscTime = ofGetElapsedTimeMillis();
	
	//////////////// init some variables
	bUpdateSerialList = true;
	for(int i=0; i<6;i++){
		theSensors.push_back(Sensor("sensor "+ofToString(i)));
	}
}

//--------------------------------------------------------------
void PulsumOsc::update(){
	// check to see if serial list has to be updated
	if(bUpdateSerialList){
		updateSerialList();
		guiSerialList->clearToggles();
		for(int i=0; i<theSerialList.size(); i++){
			guiSerialList->addToggle(theSerialList.at(i));
		}
		bUpdateSerialList = false;
	}

	// read serial port
	while((mSerial.isInitialized()) && (mSerial.available() > 2)){
		if(mSerial.readByte() == 0xAB){
			int upperByte = mSerial.readByte();
			int lowerByte = mSerial.readByte();
			
			unsigned short pinNumber = (upperByte>>0x04)&0x07;
			unsigned short value = ((upperByte<<8)&0x0F00) | (lowerByte&0xFF);
			
			theSensors.at(pinNumber).addValue(value);
		}
	}
	
	// send osc
	if(ofGetElapsedTimeMillis()-lastOscTime > OSC_PERIOD){
		for(int i=0; i<theSensors.size(); ++i){
			string addPat = "/osc"+ofToString(i)+"/";
			ofxOscMessage mOscMessage;
			// min
			mOscMessage.setAddress(addPat+"min");
			mOscMessage.addFloatArg((float)theSensors.at(i).getMin());
			mOscSender.sendMessage(mOscMessage);
			// max
			mOscMessage.clear();
			mOscMessage.setAddress(addPat+"max");
			mOscMessage.addFloatArg((float)theSensors.at(i).getMax());
			mOscSender.sendMessage(mOscMessage);
			// filtered value
			mOscMessage.clear();
			mOscMessage.setAddress(addPat+"filtrado");
			mOscMessage.addFloatArg((float)theSensors.at(i).getAverageValueNormalized());
			mOscSender.sendMessage(mOscMessage);
			// raw value
			mOscMessage.clear();
			mOscMessage.setAddress(addPat+"crudo");
			mOscMessage.addFloatArg((float)theSensors.at(i).getRawValue());
			mOscSender.sendMessage(mOscMessage);
		}
		lastOscTime = ofGetElapsedTimeMillis();
	}
	
	// update video
	mVideo.update(theSensors.back().getAverageValueNormalized());
}

//--------------------------------------------------------------
void PulsumOsc::draw(){
	ofBackground(0);

	// display video
	mVideo.draw(videoSize);
	
	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, ofGetHeight()-20);
	
	for(int i=0; i<2; i++){
		ofPushMatrix();
		ofTranslate(12*horizontalUnit*i+horizontalUnit,3*verticalUnit);
		for(int j=0; j<3;j++){
			ofPushMatrix();
			ofTranslate(0, 5*verticalUnit*j);
			theSensors.at(i*3+j).draw(sensorGraphSize);
			ofPopMatrix();
		}
		ofPopMatrix();
	}
	
	// maybe draw time somewhere
	/*
	 int now = ofGetElapsedTimeMillis()/1000;
	 int hours = now/3600;
	 int minutes = (now%3600)/60;
	 int seconds = (now%60);
	 stringstream ss;
	 ss << "Lectura de " << setfill('0') << setw(2) << hours << ":";
	 ss << setfill('0') << setw(2) << minutes << ":" << setfill('0') << setw(2)<< seconds;
	 */
}

//--------------------------------------------------------------
void PulsumOsc::guiListener(ofxUIEventArgs &args){
	string name = args.widget->getName();
	// Serial list bureaucracy
	if(name.compare("Serial List") == 0) {
		ofxUIDropDownList *ddlist = (ofxUIDropDownList *) args.widget;
		if(ddlist->getSelected().size()) {
			string selection = ddlist->getSelected()[0]->getName();
			if(selection.compare("Refresh List") == 0){
				bUpdateSerialList = true;
				ddlist->clearSelected();
			}
			// else, if clicked on an item, try to setup a serial connection
			// TODO: add some feedback as to whether we're connected and to what
			else if(!ddlist->isOpen()) {
				mSerial.close();
				mSerial.setup(selection, 57600);
			}
		}
	}
}

//--------------------------------------------------------------
vector<string>& PulsumOsc::updateSerialList(){
	theSerialList.clear();
	theSerialList.push_back("Refresh List");
	
	ofSerial tSerial;
	vector<ofSerialDeviceInfo> serialList = tSerial.getDeviceList();
	for(int i=0; i<serialList.size(); i++){
		string thisDevicePath = serialList.at(i).getDevicePath();
		theSerialList.push_back(thisDevicePath);
	}
	return theSerialList;
}

//--------------------------------------------------------------
void PulsumOsc::keyPressed(int key){
	
}

//--------------------------------------------------------------
void PulsumOsc::keyReleased(int key){

}

//--------------------------------------------------------------
void PulsumOsc::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void PulsumOsc::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void PulsumOsc::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void PulsumOsc::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void PulsumOsc::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void PulsumOsc::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void PulsumOsc::dragEvent(ofDragInfo dragInfo){
	
}
