#include "ofMain.h"
#include "Controller.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 300,768, OF_WINDOW);
	ofRunApp(new Controller());
}
