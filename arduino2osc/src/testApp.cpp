#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ctl = make_shared<Controller>();


    
    
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofBackground(125,125,125);

	font.loadFont("franklinGothic.otf", 20);

}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void testApp::updateArduino(){

	// update the arduino, get any data or messages.
//	ard.update();
	

	

}


//--------------------------------------------------------------
void testApp::draw(){
	//bgImage.draw(0,0);
/*
	if (!bSetupArduino){
		font.drawString("arduino not ready\n", 545, 40);
	} else {
		font.drawString("analog pin 0: " + ofToString(ard.getAnalog(0)) +
						"\nsending pwm: " + ofToString((int)(128 + 128 * sin(ofGetElapsedTimef()))), 545, 40);
        
        for(int i = 0; i<13; i++)
            font.drawString(ofToString(ard.getDigital(i)),55+(i*30), 110);


	}
 
 */
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//ard.sendDigital(13, ARD_HIGH);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	//ard.sendDigital(13, ARD_LOW);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


void testApp::exit(){ 
    
    ctl->exit();
}


