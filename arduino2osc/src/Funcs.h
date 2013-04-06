#pragma once

class aFunc: public Functor{
 public:
  aFunc(ofPtr<Input> _input, ofPtr<ofxUISlider> _slider,ofPtr<PresetManager> _presetManager, ofPtr<OscSender>  _oscSender, int _index){
    input = _input; 
    slider = _slider; 
    presetManager=_presetManager; 
    oscSender=_oscSender; 
    index=_index;
  }
  void execute() {
    float val = input->getFloatValue();
    float mapval = input->getMappedFloatValue();
    slider->setValue(val);
    oscSender->sendMessage( "/"+presetManager->getAddress( index ), mapval );
  }
    
  int index;
  ofPtr<Input> input;
  ofPtr<ofxUISlider> slider;
  ofPtr<OscSender> oscSender;
  ofPtr<PresetManager> presetManager;
};
