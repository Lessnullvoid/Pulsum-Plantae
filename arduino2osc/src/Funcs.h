    


class aFunc: public Functor{
public:
    aFunc(shared_ptr<Input> _input, shared_ptr<ofxUISlider> _slider,shared_ptr<PresetManager> _presetManager, shared_ptr<OscSender>  _oscSender, int _index){
        input = _input; slider = _slider; presetManager=_presetManager; oscSender=_oscSender; index=_index;
    }
    void execute() {
        float val = input->getFloatValue();
        float mapval = input->getMappedFloatValue();
        slider->setValue(val);
        oscSender->sendMessage( "/"+presetManager->getAddress( index ), mapval );
    }
    
    int index;
    shared_ptr<Input> input;
    shared_ptr<ofxUISlider> slider;
    shared_ptr<OscSender> oscSender;
    shared_ptr<PresetManager> presetManager;
};
