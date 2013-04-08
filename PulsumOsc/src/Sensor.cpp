#include "Sensor.h"

Sensor::Sensor(){
	// initial end index
	//   "end" points to one after the last value in range
	end = LONG_TERM;

	// initial begin indices
	shortBegin = end - SHORT_TERM;
	mediumBegin = end - MEDIUM_TERM;
	longBegin = end - LONG_TERM;

	// init values
	for(int i=0; i<sizeOf(sensorValues); ++i){
		sensorValues[i] = 0;
	}
	minValue = 0xffff;
	maxValue = 0;
	
	name = "test";
	mFont.loadFont("verdana.ttf",16);
}

Sensor::~Sensor(){}

void Sensor::addValue(unsigned short val){
	// write value
	sensorValues[end] = val;

	// update the indices
	shortBegin = (shortBegin+1)%sizeOf(sensorValues);
	mediumBegin	= (mediumBegin+1)%sizeOf(sensorValues);
	longBegin = (longBegin+1)%sizeOf(sensorValues);
	end = (end+1)%sizeOf(sensorValues);

	// find min/max on shortTerm
	minValue = 0xffff;
	maxValue = 0;
	for(unsigned int i=shortBegin; i!=end; i=(i+1)%sizeOf(sensorValues)){
		if(sensorValues[i] > maxValue){
			maxValue = sensorValues[i];
		}
		if(sensorValues[i] < minValue){
			minValue = sensorValues[i];
		}
	}
}

void Sensor::draw(float height){
	// background rectangle
	ofFill();
	ofSetColor(100);
	ofRect(0,0,300,height);

	// sensor title
	ofSetColor(255);
	mFont.drawString(name,10,mFont.getLineHeight());
	
	// graph
	ofPushMatrix();
	ofTranslate(0,height/4);
	drawShortTermGraph(height/2);
	ofTranslate(0,height/2);
	
	// min/max
	stringstream ss;
	ss << "min: " << minValue << "  " << "max: " << maxValue;
	mFont.drawString(ss.str(), 0, mFont.getLineHeight());
	ofPopMatrix();
}

void Sensor::drawShortTermGraph(float height){
	ofFill();
	int x = 0;
	ofBeginShape();
	ofVertex(x,height);
	for(unsigned int i=shortBegin; i!=end; i=(i+1)%sizeOf(sensorValues)){
		unsigned short y0 = ofMap(sensorValues[i], 1023, 0, 0,height);
		ofVertex(x, y0);
		x++;
	}
	ofVertex(x-1, height);
	// background rectangle
	ofSetColor(90);
	ofRect(0,0,x-1, height);
	ofSetColor(255);
	ofEndShape();
	
	// for smoothing
	ofNoFill();
	ofSetLineWidth(2);
	x = 0;
	ofBeginShape();
	ofVertex(x,height);
	for(unsigned int i=shortBegin; i!=end; i=(i+1)%sizeOf(sensorValues)){
		unsigned short y0 = ofMap(sensorValues[i], 1023, 0, 0,height);
		ofVertex(x, y0);
		x++;
	}
	ofVertex(x-1, height);
	ofSetColor(255);
	ofEndShape();
}

