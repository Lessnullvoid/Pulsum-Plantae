#pragma once

#include "ofMain.h"

class VideoManager{

public:
	VideoManager();
	~VideoManager();
	void update(int v);
	void draw(const ofVec2f dimensions);

private:
	void populateVideoNames();

	ofVideoPlayer mVideoPlayer;
	int currentVideoIndex, currentVideoCounter;
	int fadeValue;
	vector<string> videoNames;
};
