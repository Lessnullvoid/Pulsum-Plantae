#include "VideoManager.h"

VideoManager::VideoManager(){
	populateVideoNames();
	currentVideoIndex = 0;
	currentVideoCounter = 1;
	fadeValue = 0;
	mVideoPlayer.loadMovie(videoNames.at(0));
	mVideoPlayer.setVolume(0);
	mVideoPlayer.setLoopState(OF_LOOP_NONE);
	mVideoPlayer.play();
}

VideoManager::~VideoManager(){
	mVideoPlayer.stop();
	mVideoPlayer.close();
}

void VideoManager::update(int v){
	// update current video
	mVideoPlayer.update();

	// if number of frames left is around 255, set fadeout based on this number
	if(mVideoPlayer.getTotalNumFrames() - mVideoPlayer.getCurrentFrame() < 128){
		fadeValue = 255 - 2*(mVideoPlayer.getTotalNumFrames() - mVideoPlayer.getCurrentFrame());
	}
	// if fading in
	else if(fadeValue < 0) {
		fadeValue++;
	}

	// if fully opaque, change video
	if(fadeValue > 254){
		fadeValue = -fadeValue;
		// switch video
		int nextVideoIndex = ofMap(v, 0, 1023, 0, videoNames.size()-1,true);

		// if new video, restart counter
		if(nextVideoIndex != currentVideoIndex){
			currentVideoCounter = 1;
		}
		// if we haven't played this video 3 times, play it again
		else if(currentVideoCounter < 3){
			currentVideoCounter++;
		}
		// if we already played current video 3 times in a row, pick a random video
		else{
			// avoid picking same video
			nextVideoIndex = (currentVideoIndex+(int)ofRandom(1,videoNames.size()))%videoNames.size();
			currentVideoCounter = 1;
		}
		
		// should have a valid nextVideoIndex
		mVideoPlayer.stop();
		mVideoPlayer.close();
		mVideoPlayer.loadMovie(videoNames.at(nextVideoIndex));
		mVideoPlayer.setVolume(0);
		mVideoPlayer.setLoopState(OF_LOOP_NONE);
		mVideoPlayer.play();

		// update currentVideoIndex
		currentVideoIndex = nextVideoIndex;
	}
}

void VideoManager::draw(const ofVec2f dimensions){
	ofSetColor(255);
	mVideoPlayer.draw(0,0, dimensions.x, dimensions.y);
	ofFill();
	ofSetColor(0,0,0,abs(fadeValue));
	ofRect(0,0, dimensions.x, dimensions.y);
}

void VideoManager::populateVideoNames(){
	ofDirectory moviesDir("videos");
	moviesDir.listDir();
	for(int i=0; i<moviesDir.numFiles(); ++i){
		videoNames.push_back("videos/"+ofToString(moviesDir.getName(i)));
	}
}
