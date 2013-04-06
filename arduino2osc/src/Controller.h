class Controller{
 public:
  ~Controller(){
    arduino->disconnect();
    ofRemoveListener(ofEvents().update,this,&Controller::update);
  }

  Controller(){
    arduino = ofPtr<Arduino>(new Arduino());
	  
    inputManager = ofPtr<InputManager>(new InputManager());
    setupModes();
    inputManager->setCurrentModule(module);
            
    ofAddListener(ofEvents().update,this,&Controller::update);
            
    XML.loadFile("xml/presets.xml");
            
    int numBanks = XML.getNumTags("BANK");
            
    if(numBanks > 0){
      XML.pushTag("BANK", numBanks-1);

      int numPresets = XML.getNumTags("PRESET");
                
      if(numPresets > 0){
	for(int h=0; h<numPresets; h++){
	  XML.pushTag("PRESET", h);

	  presetManager->addPreset(0);
                    
	  int numAddressTags = XML.getNumTags("ADDRESS");
                
	  if(numAddressTags > 0){
	    for(int i = 0; i < numAddressTags; i++){
	      string str = XML.getValue("ADDRESS", "str", i);
	      addAddress(str);
	    }
	  }
	  XML.popTag();
	}
      }
      XML.popTag();
    }
            
    XML.pushTag("RANGES", 0);
          
    int numRanges = XML.getNumTags("RANGE");

    if(numRanges > 0){
      for(int i=0; i<numRanges; i++){
	XML.pushTag("RANGE", i);
                        
	float min = XML.getValue("MIN", 0.0f);
	float max = XML.getValue("MAX", 0.0f);
              
	sensors->getRangeSlider(i)->setValueLow(min);
	sensors->getRangeSlider(i)->setValueHigh(max);
                        
	inputManager->getInput(i)->setRange(min,max);

	XML.popTag();
      }
                    
    }
    XML.popTag();
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

    
  void update(ofEventArgs & args){
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
                
	  XML.pushTag( "BANK", numBank );
                    
	  int newTagNum = XML.addTag("PRESET");
                
	  XML.pushTag( "PRESET", newTagNum );
                                
	  for (int j=0; j<ANALOG_INPUTS; j++) {
	    XML.addTag("ADDRESS");
	    str = sensors->getTextInput(j)->getTextString();
	    XML.setValue( "ADDRESS", str, j );
	  }
                    
	  XML.popTag();
	  XML.popTag();
	  XML.saveFile("xml/presets.xml");
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
      XML.pushTag( "RANGES", 0 );
      float val;
      for (int j=0; j<ANALOG_INPUTS; j++) {
	XML.pushTag( "RANGE", j );
	
	XML.addTag("MIN");
	val = sensors->getRangeSlider(j)->getScaledValueLow();
	XML.setValue( "MIN", val );
	XML.addTag("MAX");
	val = sensors->getRangeSlider(j)->getScaledValueHigh();
	XML.setValue( "MAX", val );
        
	XML.popTag();
	
      }
      
      XML.popTag();
      XML.saveFile("xml/presets.xml");
    }
  }
  
  
  void exit(){
    arduino->disconnect();
  }
  
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
    
  ofxXmlSettings XML;

};
