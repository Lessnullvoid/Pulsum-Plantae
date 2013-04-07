#include "PulsumOsc.h"

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
void PulsumOsc::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();

	//////////////// the serial GUI
	mGui.setFont("verdana.ttf");
	////// Serial Port list
	guiSerialList = (ofxUIDropDownList*) mGui.addWidgetDown(new ofxUIDropDownList("Serial List", theSerialList));
	guiSerialList->setAutoClose(true);
	
	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	ofAddListener(mGui.newGUIEvent,this,&PulsumOsc::guiListener);
	
	////////////////
	bUpdateSerialList = true;
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
		if (mSerial.readByte() == 0xAB) {
			int upperByte = mSerial.readByte();
			int lowerByte = mSerial.readByte();
			
			int pinNumber = (upperByte>>0x04)&0x07;
			int value = ((upperByte<<8)&0x0F00) | (lowerByte&0xFF);

			cout << "val["<< pinNumber << "] = " << value << endl;
		}
	}
	
}

//--------------------------------------------------------------
void PulsumOsc::draw(){
	ofBackground(0);

	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, ofGetHeight()-20);
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