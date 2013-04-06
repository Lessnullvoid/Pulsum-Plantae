

class Functor {
    public:
        virtual void execute() {};
        virtual ~Functor(){}
};


class FunctorHolder: public Functor {
    public:
        virtual void execute() {
            for (int i=0; i<functors.size(); i++)
            {
            	functors[i]->execute();
            }
        }

        void addFunc(ofPtr<Functor> _func) {
            functors.push_back(_func);
        }
        vector< ofPtr<Functor> > functors;
};


class Arduino{
    public:
        Arduino(){
            arduino.disconnect();
            serialDevices = port.getDeviceList();

            string deviceLine;
            if(serialDevices.size()) {
                for(int i=0; i<serialDevices.size();i++){

                    deviceLine = (string)serialDevices[i].getDeviceName().c_str();
                    
                    serialDeviceNames.push_back( serialDevices[i].getDeviceName() );

                    /*
                    if(deviceLine.substr(0,12) == "tty.usbmodem"){
                        string serialID = "/dev/" +deviceLine;
                         cout<<"arduino serial = "<<serialID<<endl;
                    }
                    */
                }
            }


            vector<float> tmpvector;
            for (int i=0; i<ANALOG_INPUTS; i++) {
            	tmpvector.clear();
            	for (int j=0; j<SENSOR_VALUE_COUNT; j++)
                    tmpvector.push_back( -1 );
                valueCaches.push_back(tmpvector);
            }
            
        
            
            bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
        }
    
        ~Arduino(){
            arduino.disconnect();
        }


        void connect(string _deviceName, int _baudrate) {
            arduino.connect( _deviceName, _baudrate );
            ofAddListener(arduino.EInitialized, this, &Arduino::setupArduino);

        }
    
   
        void setupArduino(const int & version) {
            // remove listener because we don't need it anymore
            ofRemoveListener(arduino.EInitialized, this, &Arduino::setupArduino);

            // this is where you setup all the pins and pin modes, etc
            for (int i = 0; i < 13; i++){
                arduino.sendDigitalPinMode(i, ARD_INPUT);
            }
            
            for (int i = 0; i < ANALOG_INPUTS; i++){
                arduino.sendAnalogPinReporting(i, ARD_ANALOG);
            }

            
            arduino.sendDigitalPinMode(13, ARD_INPUT);
            //arduino.sendAnalogPinReporting(0, ARD_ANALOG);	// AB: report data
            //ard.sendDigitalPinMode(11, ARD_PWM);		// on diecimelia: 11 pwm?*/
        
            bSetupArduino = true;
        }

    
        void disconnect(){
            arduino.disconnect();
        }


        vector<float> getNewValues(){

            vector<float> newValues;

            float value;

            for (int i=0; i<ANALOG_INPUTS; i++) {
                
                if(bSetupArduino){
                    
                value = arduino.getAnalog(i);
                
                valueCaches[i].erase( valueCaches[i].begin() );
                valueCaches[i].push_back(value);
/*
 
 
*/
                if(i<3||i==4){
                    
                    //if( checkTouch(i) )
                        newValues.push_back( fabs(value-1023) );
                    //else
                      //  newValues.push_back( -1 );
                }
                else if(i==3){
                    //flex
                    newValues.push_back(fabs(value-200));
                }
                else if(i==5){
                    
                    //acelerometro
                    newValues.push_back(fabs(value-500));
                    
                }
                
                }
                else
                    newValues.push_back( -1 );

            }

            return newValues;

        }

        bool checkTouch(int _index) {
            bool touching=true;
            for (int i=0; i < SENSOR_VALUE_COUNT; i++)
                for (int j=0; j < SENSOR_VALUE_COUNT; j++)
                    if( fabs( valueCaches[_index][i] - valueCaches[_index][j] ) > SENSOR_VALUE_THRESHOLD ) {
                        touching=false;
                    };

            return touching;
        }
    
        void update(){
            arduino.update();        
        }
    
    
        
    
        vector<string> getSerialList(){
            
            return serialDeviceNames;
        
        }

        vector<ofSerialDeviceInfo> serialDevices;
        vector<string> serialDeviceNames;
        vector< vector<float> > valueCaches;

        ofArduino arduino;
        ofSerial port;
        bool bSetupArduino;

};






class Input{
    public:
        Input(string _name, string _type){
            name=_name;
            type=_type;
            init();
        }
        virtual ~Input(){}

        void init() {
            setRange(0,1023);
            floatValue=0;
            intValue=0;
            boolValue=false;
        }
    
        void setRange(int _min, int _max){
            min=_min;
            max=_max;
        }
    
        void setType(string _type) {
            type = _type;
        }

        void setName(string _name) {
            name = _name;
        }

        float setFloatValue(float _float)   {   
            if(_float<min) floatValue=min;
            else if(_float>max) floatValue=max;
            else floatValue = _float;

            //floatValue = ofMap(floatValue,min,max,min,max);
            
        }    
        float setBoolValue(bool _bool)      {   boolValue = _bool;      }
        float setIntValue(int _int)         {   intValue = _int;        }
        float setCharValue(char _char)      {   charValue = _char; }

        float getFloatValue()   {   
            //cout<<floatValue<<endl;
            return floatValue;
    
        }
    
        float getMappedFloatValue()   {   
            //cout<<floatValue<<endl;
            return ofMap(floatValue,min,max,0,127);
        
        }
        
        float getBoolValue()    {   return boolValue;   }
        float getIntValue()     {   return intValue;    }
        float getCharValue()    {   return charValue;   }


    protected:
    
        string name, type;
        float min,max;
        float floatValue;
        int intValue;
        bool boolValue;
        char charValue;


};




class OscSender{
public:
    OscSender(){
        init();
    }
    ~OscSender(){}
    
    void init(){
        sender.setup( HOST, PORT );

    }
    
    void sendMessage(string _address, float _value) {
		ofxOscMessage m;
		m.setAddress( _address );
		m.addFloatArg( _value );
		sender.sendMessage( m );
	}
    
protected:
    ofxOscSender sender;    
    
};


class InputGroup{
    public:
        InputGroup(){}

        void addInputs(int _no, string _type, string _prefix){
            for (int i=0; i<_no; i++)
	      //inputs.push_back(make_shared<Input>(_prefix+ofToString(i),_type));
	      inputs.push_back(ofPtr<Input>(new Input(_prefix+ofToString(i),_type)));
        }

        void setType(string _type) {
            type = _type;
        }

        void setName(string _name) {
            name = _name;
        }

        string type, name;
        vector< ofPtr<Input> > inputs;

};


class Module{

    public:
        Module(){}

        void addRouting(ofPtr<Input> _input, ofPtr<Functor> _func) {
            routings.insert( make_pair( _input, _func ) );
        }

        void setValue( ofPtr<Input>  _input ) {

            Routing::const_iterator iter;
	        iter = routings.find( _input );
	        if( iter != routings.end() )
	            return iter->second->execute();
        }

        void setGUI(ofPtr<GUImodule> _module){
            gui = _module;
        }

        typedef map< ofPtr<Input>, ofPtr<Functor> > Routing;
        Routing routings;

        ofPtr<GUImodule> gui;


//routing [inputarray,funcs]
//	guimodule
//		activate (highlight)
//
//	func(input[i],guiproperty[j],pdsend)

};


class Mode{
    public:
        Mode(){}

        void addModule ( ofPtr<Module> _module ) {
            modules.push_back( _module );
        }

    vector< ofPtr<Module> > modules;

};




class InputManager{
    public:
        InputManager(){
            setupInputs();
        }

        void setupInputs(){
            for (int i=0; i<ANALOG_INPUTS; i++) {
	      ofPtr<Input > input = ofPtr<Input>(new Input("analog"+ofToString(i), "float"));//make_shared<Input >("analog"+ofToString(i), "float");
                inputs.push_back( input );
            }
        }

        ofPtr< Input > getInput(int _i) {
            if(inputs.size())
                return inputs[_i];
        }


        int getInputNum() {
            if(inputs.size())
                return (int) inputs.size();
            else
                return 0;
        }

        void setCurrentModule( ofPtr<Module> _module ) {
            currentModule = _module;
        }


        void execute(int _i){
                        
            if(currentModule)
                currentModule->setValue( inputs[_i] );

        }



        vector< ofPtr< Input  > > inputs;
        vector< ofPtr< Module > > modules;
        ofPtr< Module > currentModule;



};






class PresetManager{
public:
    PresetManager(){
        init();
    }
    ~PresetManager(){}
    
    void init(){
        vector< vector<string> > strvecvec;
        presets.push_back( strvecvec );
        currentBank=0;
        currentPreset=0;


    }
    
    int getBankNum() {
        if(presets.size())
            return (int) presets.size();
        else
            return 0;
    }
    
    int getCurrentBank() {
        return currentBank;
    }
    
    void setCurrentBank(int _bank) {
        currentBank=_bank;
    }
    
    int getCurrentPreset() {
        return currentPreset;
    }
    
    void setCurrentPreset(int _preset) {
        currentPreset=_preset;
    }
    
    int getBankPresetNum(int _bank) {
        if(presets[_bank].size())
            return (int) presets[_bank].size();
        else
            return 0;
    }
    
    int getBankPresetKeyNum(int _bank, int _preset) {
        if(presets[_bank][_preset].size())
            return (int) presets[_bank][_preset].size();
        else
            return 0;
    }
    
    
    void addPreset( int _bank ){
        if(_bank<presets.size()) {
            vector<string> strvec;
            presets[_bank].push_back( strvec );
            
        }
    }
    
    
     void addPreset( int _bank, int _preset, string _address ) {
        if(_bank<presets.size()) {
            if(_preset<presets[_bank].size()) {
                presets[_bank][_preset].push_back( _address );
            }
            else {
                vector<string> strvec;
                strvec.push_back(_address);
                presets[_bank].push_back( strvec );
            }
        }
        else{
            vector< vector<string> > strvecvec;
            presets.push_back( strvecvec );
            presets.back().back().push_back(_address);
            
        }
        
    }
    

    void addAddress( int _bank, int _preset, string _address ){
        presets[_bank][_preset].push_back(_address);
    }

    
    void addAddress( string _address ){
        presets[currentBank][currentPreset].push_back(_address);
    }
    
    
    string getAddress( int _i ) {
        if(currentBank<presets.size())
            if(currentPreset<presets[currentBank].size())
                if(_i<presets[currentBank][currentPreset].size())
                    return presets[currentBank][currentPreset][_i];
    }

    
    string getAddress( int _bank, int _preset, int _i ) {
        if(_bank<presets.size())
            if(_preset<presets[_bank].size())
                if(_i<presets[_bank][_preset].size())
                    return presets[_bank][_preset][_i];
    }
    
    vector< ofPtr< Input  > > inputs;
    vector< ofPtr< Module > > modules;
    //shared_ptr< vector<string> > currentPreset;
    
    vector< vector< vector<string> > > presets;
    int currentBank, currentPreset;
    
};



