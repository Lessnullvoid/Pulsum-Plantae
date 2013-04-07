#include "PulsumOsc.h"

//--------------------------------------------------------------
void PulsumOsc::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void PulsumOsc::update(){

}

//--------------------------------------------------------------
void PulsumOsc::draw(){
	ofBackground(0);

	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
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