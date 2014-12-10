#include "Sensor.h"

Sensor::Sensor(string name_){
  // initial end indices
  //   "end" points to one after the last value in range
  rawEnd = sizeOf(rawValues)-1;
  averageEnd = sizeOf(averageValues)-1;

  // init values
  for(int i=0; i<sizeOf(rawValues); ++i){
    rawValues[i] = 0;
  }
  for(int i=0; i<sizeOf(averageValues); ++i){
    averageValues[i] = 0;
  }
  for(int i=0; i<sizeOf(currentRunningAverage); ++i){
    currentRunningAverage[i] = 0;
  }
  
  minValue = 0xffff;
  maxValue = 0;
  averageSum = 0;
  averageIndex = 0;
  
  name = name_;
  mFont.loadFont("Dekar.otf",20);
}

Sensor::~Sensor(){}

const string Sensor::getName() const{
  return name;
}
const unsigned short Sensor::getMin() const{
  return minValue;
}
const unsigned short Sensor::getMax() const{
  return maxValue;
}
const unsigned short Sensor::getRawValue() const{
  // last written value is at end-1
  // make sure index is positive
  int getFromIndex = (rawEnd > 0)?(rawEnd-1):(sizeOf(rawValues)-1);
  return rawValues[getFromIndex];
}
const unsigned short Sensor::getAverageValue() const{
  // last written value is at end-1
  // make sure index is positive
  int getFromIndex = (averageEnd > 0)?(averageEnd-1):(sizeOf(averageValues)-1);
  return averageValues[getFromIndex];
}
const unsigned short Sensor::getAverageValueNormalized() const{
  return ofMap(getAverageValue(), getMin(), getMax(), 0, 1023, true);
}


void Sensor::addValue(const unsigned short val){
  // write value to raw
  rawValues[rawEnd] = val;
  // update the indices
  rawEnd = (rawEnd+1)%sizeOf(rawValues);
  
  // update running average
  averageSum -= currentRunningAverage[averageIndex];
  currentRunningAverage[averageIndex] = val;
  averageSum += currentRunningAverage[averageIndex];
  averageIndex = (averageIndex+1)%sizeOf(currentRunningAverage);
  
  // write to average values
  averageValues[averageEnd] = averageSum/sizeOf(currentRunningAverage);
  averageEnd = (averageEnd+1)%sizeOf(averageValues);

  // find min/max of current averages
  unsigned short thisMinValue = 0xffff;
  unsigned short thisMaxValue = 0;
  for(unsigned int i=0; i<sizeOf(averageValues); ++i){
    if(averageValues[i] > thisMaxValue){
      thisMaxValue = averageValues[i];
    }
    if(averageValues[i] < thisMinValue){
      thisMinValue = averageValues[i];
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
  ofSetColor(100,128);
  ofRect(0,0,dimensions.x*11/12,dimensions.y);

  // sensor title
  ofSetColor(255);
  mFont.drawString(name,10,mFont.getLineHeight());

  // raw graph
  ofPushMatrix();
  ofTranslate(0,dimensions.y/4);
  drawGraph(rawValues, sizeOf(rawValues), rawEnd, dimensions.x/2, dimensions.y/2);
  ofPopMatrix();

  // second graph
  ofPushMatrix();
  ofTranslate(dimensions.x/2+10, dimensions.y/4);
  drawGraph(averageValues, sizeOf(averageValues), averageEnd, dimensions.x*5/12-10, dimensions.y/2);
  ofPopMatrix();
  
  // info string
  ofPushMatrix();
  ofTranslate(0,dimensions.y*3/4);
  stringstream ss;
  ss << "current: " << getRawValue() << " min: " << minValue << " max: " << maxValue;
  mFont.drawString(ss.str(), 10, mFont.getLineHeight());
  ofPopMatrix();
}

void Sensor::drawGraph(const unsigned short values[], const int sizeOfValues, const unsigned int lastIndex, const float width, const float height) const{
  // background rectangle
  ofFill();
  ofSetColor(90,128);
  ofRect(0,0,width, height);
  
  ofSetColor(255);
  ofBeginShape();
  ofVertex(0,height);
  for(unsigned int x=0, i=(lastIndex-width); x<width; ++x, ++i){
    int yIndex = i;
    while(yIndex<0){
      yIndex += sizeOfValues;
    }
    yIndex = yIndex%sizeOfValues;
    
    unsigned short y0 = ofMap(values[yIndex], 1023, 0, 0, height);
    ofVertex(x, y0);
  }
  ofVertex(width, height);
  ofEndShape();
  
  // for smoothing
  ofNoFill();
  ofSetLineWidth(2);
  ofSetColor(255);
  ofBeginShape();
  ofVertex(0,height);
  for(unsigned int x=0, i=(lastIndex-width); x<width; ++x, ++i){
    int yIndex = i;
    while(yIndex<0){
      yIndex += sizeOfValues;
    }
    yIndex = yIndex%sizeOfValues;
    
    unsigned short y0 = ofMap(values[yIndex], 1023, 0, 0, height);
    ofVertex(x, y0);
  }
  ofVertex(width, height);
  ofEndShape();
}

