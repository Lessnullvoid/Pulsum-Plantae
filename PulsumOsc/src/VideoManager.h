#pragma once

#include "ofMain.h"

class VideoManager{

public:
	VideoManager();
	~VideoManager();
	void update();
	void draw(const ofVec2f dimensions);
	void playVideo(int i=-1);

private:
	void populateVideoNames();

	ofVideoPlayer mVideoPlayer;
	int currentVideo, nextVideo;
	vector<string> videoNames;
};
