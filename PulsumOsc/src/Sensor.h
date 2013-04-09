#pragma once

#include "ofMain.h"

#define SHORT_TERM 300
#define MEDIUM_TERM 3000
#define LONG_TERM 30000

class Sensor {
	
public:
	Sensor(string name_);
	~Sensor();
	void addValue(const unsigned short val);
	void draw(const ofVec2f dimensions);
	const unsigned short getMin() const;
	const unsigned short getMax() const;
	const unsigned short getValue() const;
	
private:
	void drawShortTermGraph(const float width, const float height) const;
	
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
