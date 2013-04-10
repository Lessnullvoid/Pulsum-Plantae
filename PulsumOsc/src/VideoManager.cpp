#include "VideoManager.h"

VideoManager::VideoManager(){
	populateVideoNames();
	mVideoPlayer.loadMovie(videoNames.at(0));
	currentVideo = 0;
	nextVideo = 0;
	mVideoPlayer.setVolume(0);
	mVideoPlayer.setLoopState(OF_LOOP_NORMAL);
	mVideoPlayer.play();
}

VideoManager::~VideoManager(){
	mVideoPlayer.stop();
	mVideoPlayer.close();
}

void VideoManager::update(){
	mVideoPlayer.update();
	if(nextVideo != currentVideo){
		// TODO: fade??
	}
}

void VideoManager::draw(const ofVec2f dimensions){
	mVideoPlayer.draw(0,0, dimensions.x, dimensions.y);
}

void VideoManager::playVideo(int i){
	// already playing this video
	if(i == currentVideo){
		return;
	}

	// change video
	if(i < 0){
		nextVideo = (int)ofRandom(videoNames.size());
	}
	else{
		nextVideo = i%videoNames.size();
	}
}

void VideoManager::populateVideoNames(){
	// TODO: fill this out (either manually, or by parsing the bin/data directory)
	videoNames.push_back("mov.mov");
}
