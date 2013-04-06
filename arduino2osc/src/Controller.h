#pragma once
#define HOST "localhost"
#define PORT 8001

#define BAUD_RATE 57600

#define ANALOG_INPUTS 6

#define SENSOR_VALUE_COUNT 12
#define SENSOR_VALUE_THRESHOLD 5

#include "ofxXmlSettings.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#include "gui.h"
#include "sistema.h"
#include "Funcs.h"


class Controller : public ofBaseApp{
public:
	Controller(){}
	~Controller(){
		arduino->disconnect();
	}
	
	void setup(){
		ofSetVerticalSync(true);
		ofSetFrameRate(60);
		ofBackground(125,125,125);
		
		arduino = ofPtr<Arduino>(new Arduino());
		inputManager = ofPtr<InputManager>(new InputManager());
		setupModes();
		inputManager->setCurrentModule(module);
				
		presetManager->addPreset(0);
		
		string str = "ADDRESS";
		addAddress(str);

		int i = 0;
		float min = 0.0;
		float max = 1023.0;
		sensors->getRangeSlider(i)->setValueLow(min);
		sensors->getRangeSlider(i)->setValueHigh(max);
		inputManager->getInput(i)->setRange(min,max);
	}
	
    
	void setupModes(){
		mode = ofPtr<Mode>(new Mode());
		module = ofPtr<Module>(new Module());
		
		gui = ofPtr<GUI>(new GUI());
		guiMode = ofPtr<GUImode>(new GUImode());
		
		presetManager = ofPtr<PresetManager>(new PresetManager());
		presets = ofPtr<Presets>(new Presets(0,0,300,320));
		
		guiModule = presets;
		guiMode->addModule(guiModule);
		
		sensors = ofPtr<SensorDialog>(new SensorDialog(0,320,300,450));
		
		sensors->setSerialList( arduino->getSerialList() );
		sensors->init();
		
		guiModule = sensors;
		guiMode->addModule(guiModule);
		
		for(int i = 0; i<guiMode->getModuleNum(); i++){
			for(int j = 0; j<guiMode->getModule(i)->getCanvasNum(); j++){
				ofAddListener(guiMode->getModule(i)->getCanvas(j)->newGUIEvent,this,&Controller::guiEvent);
			}
		}
		
		gui->addMode(guiMode);
		
		
		int inputNum = inputManager->getInputNum();
		bool createRouting=true;
		
		oscsender  =   ofPtr<OscSender>(new OscSender());
		
		for (int i=0; i<inputNum; i++) {
			if(inputManager->getInput(i))
				input   =   inputManager->getInput(i);
			else createRouting=false;
			
			if(guiModule->getSlider(i))
				slider  =   guiModule->getSlider(i);
			else createRouting=false;
			
			if(createRouting)
				module->addRouting(input, ofPtr<aFunc>(new aFunc(input,slider,presetManager,oscsender,i)));
		}
		
		mode->addModule( module );
		modes.push_back( mode );
	}
	
    
	void addAddress(string _address){
		
		int bank = presetManager->getCurrentBank();
		int newPreset = presetManager->getBankPresetNum(bank)-1;
        
		presetManager->setCurrentPreset(newPreset);
        
		presetManager->addAddress( _address );
		vector<ofxUIToggle*> toggles = presets->getMatrix(1)->getToggles();
        
		if(toggles[newPreset]) {
			toggles[newPreset]->setColorBack(ofColor(ofRandomuf()*255,ofRandomuf()*255,ofRandomuf()*255));
		}
        
	}
	
    
	void update(){
		vector<float> values;
		
		arduino->update();
		
		values = arduino->getNewValues();
		
		for (int i=0; i<values.size(); i++){
			if(values[i]!=-1) {
				inputManager->getInput(i)->setFloatValue(values[i]);
				inputManager->execute(i);
			}
		}
	}
	
	void guiEvent(ofxUIEventArgs &e) {
		int index;
		int bank;
		string str;
		
		string name = e.widget->getName();
		//int kind = e.widget->getKind();
		cout << "got event from: " << name << endl;
		
		if(name == "guardar") {
			ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            
			if(toggle->getValue()){
				bank = presetManager->getCurrentBank();
				int newPreset = presetManager->getBankPresetNum(bank);
				
				presetManager->setCurrentPreset(newPreset);
				
				for (int i=0; i<ANALOG_INPUTS; i++) {
					str = sensors->getTextInput(i)->getTextString();
					presetManager->addPreset(bank,newPreset,str);
				}
                
				vector<ofxUIToggle*> toggles = presets->getMatrix(1)->getToggles();
                
				if(toggles[newPreset]) {
					toggles[newPreset]->setColorBack(ofColor(ofRandomuf()*255,ofRandomuf()*255,ofRandomuf()*255));
				}
                
				int numBank = presetManager->getCurrentBank();
				
				for (int j=0; j<ANALOG_INPUTS; j++) {
					str = sensors->getTextInput(j)->getTextString();
				}
			}
		}
		
		if(name.substr(0,7) == "presets"){
			bank = presetManager->getCurrentBank();
			
			index = ( ofToInt(name.substr(8,1))*8 ) + ofToInt(name.substr(10,1));
			string str;
			
			if(index< presetManager->getBankPresetNum(bank)) {
				for (int i=0; i<ANALOG_INPUTS; i++) {
					str = presetManager->getAddress(bank,index,i);
					sensors->getTextInput(i)->setTextString(str);
				}
				presetManager->setCurrentPreset(index);
			}
		}
		
		else if(name == "SERIALPORT"){
			ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
			vector<ofxUIWidget *> &selected = ddlist->getSelected();
			for(int i = 0; i < selected.size(); i++){
				arduino->connect( "/dev/"+selected[i]->getName(),57600);
			}
		}
		else if(name.substr(0,5)=="RANGE"){
			ofxUIRangeSlider *rangeSlider = (ofxUIRangeSlider *) e.widget;
			
			index = ofToInt(name.substr(5,6));
			
			float min = rangeSlider->getScaledValueLow();
			float max = rangeSlider->getScaledValueHigh();
			
			inputManager->getInput(index)->setRange(min,max);
		}
		else if(name == "SAVERANGES"){
			float val;
			for (int j=0; j<ANALOG_INPUTS; j++) {
				val = sensors->getRangeSlider(j)->getScaledValueLow();
				val = sensors->getRangeSlider(j)->getScaledValueHigh();
			}
		}
	}
	
	void exit(){
		arduino->disconnect();
	}
	
	////// to inherit ofBaseApp properly
	void keyPressed  (int key){}
	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	void windowResized(int w, int h){}
	void dragEvent(ofDragInfo dragInfo){}
	void gotMessage(ofMessage msg){}
	
	ofPtr< Input > input;
	ofPtr< PresetManager > presetManager;
	
	ofPtr< OscSender > oscsender;
	ofPtr< ofxUISlider > slider;
	ofPtr< ofxUIRangeSlider > rslider;
	
	ofPtr<GUI>  gui;
	ofPtr<Arduino> arduino;
	vector< ofPtr<Mode> > modes;
	ofPtr<InputManager> inputManager;
	
	ofPtr< GUImode >  guiMode;
	ofPtr< GUImodule > guiModule;
	ofPtr< Mode > mode;
	ofPtr< Module > module;
    
    
	ofPtr<Presets> presets;
	ofPtr<SensorDialog> sensors;
};
