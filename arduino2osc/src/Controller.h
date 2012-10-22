


class Controller{
    public:
        Controller(){
            init();
        }
        ~Controller(){
            arduino->disconnect();
            ofRemoveListener(ofEvents.update,this,&Controller::update);
        }

        void init(){
        
            arduino = make_shared<Arduino>();
            
            inputManager = make_shared<InputManager>( );
            setupModes();
            inputManager->setCurrentModule(module);
            
            ofAddListener(ofEvents.update,this,&Controller::update);

            
            
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


            mode = make_shared<Mode>();
            module = make_shared<Module>();

            gui = make_shared<GUI>( );
            guiMode = make_shared<GUImode>();

            presetManager = make_shared<PresetManager>();

            presets = make_shared<Presets>( 0, 0, 300, 320 );

            guiModule = presets;
            guiMode->addModule(guiModule);
            
            sensors = make_shared<SensorDialog>( 0, 320, 300, 450 );
            
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

            oscsender  =   make_shared < OscSender > ();

            for (int i=0; i<inputNum; i++) {
                
                if(inputManager->getInput(i))
                    input   =   inputManager->getInput(i);
                else createRouting=false;

                if(guiModule->getSlider(i))
                    slider  =   guiModule->getSlider(i);
                else createRouting=false;

                //pdsend  =   make_shared < PdSend > ("send"+ofToString(i),"");

                if(createRouting)
                    module->addRouting(
                        input, make_shared<aFunc>(input,slider,presetManager,oscsender,i)
                    );

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
                
            //if(arduino->bSetupArduino ) {

                values = arduino->getNewValues();

                for (int i=0; i<values.size(); i++)
                {
                    if(values[i]!=-1) {
                        inputManager->getInput(i)->setFloatValue(values[i]);
                        inputManager->execute(i);
                    }
                }
                
            //}

        }

        void guiEvent(ofxUIEventArgs &e) {
            int index;
            int bank;
            string str;

            string name = e.widget->getName(); 
            int kind = e.widget->getKind(); 
            cout << "got event from: " << name << endl; 
            
        if(name == "guardar")
        {

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

                
                
                //lastTagNumber	= XML.addTag("STROKE");
                
                int numBank = presetManager->getCurrentBank();
                
                XML.pushTag( "BANK", numBank );
                    
                    //int numPreset = presetManager->getBankPresetNum(numBank)-1;
                                                
                    int newTagNum = XML.addTag("PRESET");
                
                    XML.pushTag( "PRESET", newTagNum );
                    //XML.setValue("PT:X", x, newTagNum);
                                
                        for (int j=0; j<ANALOG_INPUTS; j++) {
                            XML.addTag("ADDRESS");
                            str = sensors->getTextInput(j)->getTextString();
                            XML.setValue( "ADDRESS", str, j );
                        }
                    
                    XML.popTag();

                                
                        //XML.popTag();
                    //}

                XML.popTag();
                
                
                XML.saveFile("xml/presets.xml");

                            
            }
            
            
            
            

        }
            
        if(name.substr(0,7) == "presets")
        {
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
        
        else if(name == "SERIALPORT")
        {
            ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
            vector<ofxUIWidget *> &selected = ddlist->getSelected();
            for(int i = 0; i < selected.size(); i++)
            {
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
        else if(name == "SAVERANGES")
        {
                    
            XML.pushTag( "RANGES", 0 );
            float val;
            for (int j=0; j<ANALOG_INPUTS; j++) {
                //int newTagNum = XML.addTag("RANGE");                
                XML.pushTag( "RANGE", j );

                XML.addTag("MIN");
                val = sensors->getRangeSlider(j)->getScaledValueLow();
                XML.setValue( "MIN", val );
                XML.addTag("MAX");
                val = sensors->getRangeSlider(j)->getScaledValueHigh();
                XML.setValue( "MAX", val );
                
                XML.popTag();

            }
            
            
            
            //XML.popTag();
            //}
            
            XML.popTag();
            
            
            XML.saveFile("xml/presets.xml");

        }
                
        }
    
    
        void exit(){
            arduino->disconnect();
        }



        shared_ptr< Input > input;
        shared_ptr< PresetManager > presetManager;

        shared_ptr< OscSender > oscsender;
        shared_ptr< ofxUISlider > slider;
        shared_ptr< ofxUIRangeSlider > rslider;

        shared_ptr<GUI>  gui;
        shared_ptr<Arduino> arduino;
        vector< shared_ptr<Mode> > modes;
        shared_ptr<InputManager> inputManager;

        //shared_ptr<AppCore> pd;
        shared_ptr< GUImode >  guiMode;
        shared_ptr< GUImodule > guiModule;
        shared_ptr< Mode > mode;
        shared_ptr< Module > module;
    
    
        shared_ptr<Presets> presets;
        shared_ptr<SensorDialog> sensors;
    
        ofxXmlSettings XML;

        
        //shared_ptr<FileLoader> fileLoader;



};
