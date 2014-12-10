#pragma once

#include "ofMain.h"

#define AVGSIZE 20
#define DISPLAYSIZE 300
#define RAWSIZE 30000

class Sensor {
  
public:
  Sensor(string name_);
  ~Sensor();
  void addValue(const unsigned short val);
  void draw(const ofVec2f dimensions);
  const string getName() const;
  const unsigned short getMin() const;
  const unsigned short getMax() const;
  const unsigned short getRawValue() const;
  const unsigned short getAverageValue() const;
  const unsigned short getAverageValueNormalized() const;
  
private:
  void drawGraph(const unsigned short values[], const int sizeOfValues, const unsigned int lastIndex, const float width, const float height) const;
  
  unsigned short rawValues[RAWSIZE+1];
  unsigned short averageValues[DISPLAYSIZE*2+1];
  unsigned short currentRunningAverage[AVGSIZE];
  unsigned int averageSum;
  unsigned short averageIndex;

  // begin/end indices for different things
  unsigned int averageEnd, rawEnd;
  unsigned short maxValue, minValue;
  string name;
  ofTrueTypeFont mFont;
};


// leet templated function to get size of const arrays
template<typename T, size_t N>
inline int sizeOf(T(&a)[N]){
  return N;
}

