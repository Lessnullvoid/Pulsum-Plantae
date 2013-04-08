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
	ofEnableSmoothing();

	verticalUnit = ofGetHeight()/18;
	
	//////////////// the serial GUI
	mGui.setFont("verdana.ttf");
	////// Serial Port list
	guiSerialList = (ofxUIDropDownList*) mGui.addWidgetDown(new ofxUIDropDownList(0, 0, 300, "Serial List", theSerialList,0));
	guiSerialList->setAutoClose(true);
	
	mGui.autoSizeToFitWidgets();
	mGui.setColorBack(ofColor(100,200));
	mGui.setPosition(90, verticalUnit);
	ofAddListener(mGui.newGUIEvent,this,&PulsumOsc::guiListener);
	
	////////////////
	bUpdateSerialList = true;
	for(int i=0; i<6;i++){
		theSensors.push_back(Sensor());
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
			
			//cout << "val["<< pinNumber << "] = " << value << endl;
			theSensors.at(pinNumber).addValue(value);
		}
	}
}

//--------------------------------------------------------------
void PulsumOsc::draw(){
	ofBackground(0);

	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, ofGetHeight()-20);
	
	for(int i=0; i<2; i++){
		ofPushMatrix();
		ofTranslate(390*i+90,3*verticalUnit);
		for(int j=0; j<3;j++){
			ofPushMatrix();
			ofTranslate(0, 5*verticalUnit*j);
			theSensors.at(i*3+j).draw(verticalUnit*4);
			ofPopMatrix();
		}
		ofPopMatrix();
	}
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