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

const unsigned short Sensor::getMin() const{
	return minValue;
}
const unsigned short Sensor::getMax() const{
	return maxValue;
}
const unsigned short Sensor::getValue() const{
	// last written value is at end-1
	// make sure index is positive
	int getFromIndex = (end > 0)?(end-1):(sizeOf(sensorValues)-1);
	return sensorValues[getFromIndex];
}

void Sensor::addValue(const unsigned short val){
	// write value
	sensorValues[end] = val;

	// update the indices
	shortBegin = (shortBegin+1)%sizeOf(sensorValues);
	mediumBegin	= (mediumBegin+1)%sizeOf(sensorValues);
	longBegin = (longBegin+1)%sizeOf(sensorValues);
	end = (end+1)%sizeOf(sensorValues);

	// find min/max on current samples
	unsigned short thisMinValue = 0xffff;
	unsigned short thisMaxValue = 0;
	for(unsigned int i=shortBegin; i!=end; i=(i+1)%sizeOf(sensorValues)){
		if(sensorValues[i] > thisMaxValue){
			thisMaxValue = sensorValues[i];
		}
		if(sensorValues[i] < thisMinValue){
			thisMinValue = sensorValues[i];
		}
	}
	
	// if there's a new min/max, update immediately
	//    else, slowly approach current min/max
	if(thisMaxValue > maxValue){
		maxValue = thisMaxValue;
	}
	else{
		maxValue = (unsigned short)(0.99*maxValue + 0.01*thisMaxValue);
	}
	
	if(thisMinValue < minValue){
		minValue = thisMinValue;
	}
	else{
		minValue += (unsigned short)(0.01*thisMinValue);
		minValue = (thisMinValue<minValue)?thisMinValue:minValue;
	}
}

void Sensor::draw(const ofVec2f dimensions){
	// background rectangle
	ofFill();
	ofSetColor(100);
	ofRect(0,0,dimensions.x*11/12,dimensions.y);

	// sensor title
	ofSetColor(255);
	mFont.drawString(name,10,mFont.getLineHeight());

	// graph
	ofPushMatrix();
	ofTranslate(0,dimensions.y/4);
	drawShortTermGraph(dimensions.x/2, dimensions.y/2);
	ofTranslate(0,dimensions.y/2);

	// min/max
	stringstream ss;
	ss << "min: " << minValue << "  " << "max: " << maxValue;
	mFont.drawString(ss.str(), 0, mFont.getLineHeight());
	ofPopMatrix();

	// second column
	ofPushMatrix();
	ofTranslate(dimensions.x/2+10, 0);

	// graph
	ofPushMatrix();
	ofTranslate(0,dimensions.y/4);
	drawShortTermGraph(dimensions.x*5/12-10, dimensions.y/2);
	ofTranslate(0,dimensions.y/2);

	// time running
	int now = ofGetElapsedTimeMillis()/1000;
	int hours = now/3600;
	int minutes = (now%3600)/60;
	int seconds = (now%60);

	ss.str("");
	ss << "Lectura de " << setfill('0') << setw(2) << hours << ":";
	ss << setfill('0') << setw(2) << minutes << ":" << setfill('0') << setw(2)<< seconds;
	mFont.drawString(ss.str(), 0, mFont.getLineHeight());
	ofPopMatrix();
	ofPopMatrix();
}

void Sensor::drawShortTermGraph(const float width, const float height) const{
	ofFill();
	int x = 0;
	ofBeginShape();
	ofVertex(x,height);
	
	// slide the begining index depending on graph width
	int firstIndex = shortBegin;
	if(width < SHORT_TERM){
		// grab less samples by moving start point forward
		firstIndex = (firstIndex+(int)(SHORT_TERM-width))%sizeOf(sensorValues);
	}
	else {
		// grab more earlier samples by moving start point back
		firstIndex = (firstIndex-(int)(width-SHORT_TERM))%sizeOf(sensorValues);
		// watch out for wrap-around
		firstIndex = (firstIndex<0)?(sizeOf(sensorValues)-firstIndex):firstIndex;
	}
	
	for(unsigned int i=firstIndex; i!=end; i=(i+1)%sizeOf(sensorValues)){
		unsigned short y0 = ofMap(sensorValues[i], 1023, 0, 0, height);
		ofVertex(x, y0);
		x++;
	}
	ofVertex(x-1, height);
	// background rectangle
	ofSetColor(90);
	ofRect(0,0,width, height);
	ofSetColor(255);
	ofEndShape();
	
	// for smoothing
	ofNoFill();
	ofSetLineWidth(2);
	x = 0;
	ofBeginShape();
	ofVertex(x,height);
	for(unsigned int i=firstIndex; i!=end; i=(i+1)%sizeOf(sensorValues)){
		unsigned short y0 = ofMap(sensorValues[i], 1023, 0, 0, height);
		ofVertex(x, y0);
		x++;
	}
	ofVertex(x-1, height);
	ofSetColor(255);
	ofEndShape();
}
