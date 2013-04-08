#pragma once

#include "ofMain.h"

#define SHORT_TERM 300
#define MEDIUM_TERM 3000
#define LONG_TERM 30000

class Sensor {
	
public:
	Sensor();
	~Sensor();
	void addValue(unsigned short val);
	void draw(ofVec2f dimensions);
	
private:
	void drawShortTermGraph(float width, float height);
	
	unsigned short sensorValues[LONG_TERM+1];
	// using vectors to keep track of begin/end indices
	unsigned int shortBegin, mediumBegin, longBegin, end;
	unsigned short maxValue, minValue;
	string name;
	ofTrueTypeFont mFont;
};


// leet templated function to get size of const arrays
template<typename T, size_t N>
inline int sizeOf(T(&a)[N]){
	return N;
}
