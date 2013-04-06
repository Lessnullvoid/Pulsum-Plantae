struct null_deleter {
  void operator()(void const *) const{}
};


class GUImodule{
 public:
  GUImodule(float _x, float _y, float _w, float _h){
    x=_x; y=_y; width=_w; height=_h;
    canvas = ofPtr<ofxUICanvas>(new ofxUICanvas(_x, _y, _w, _h));
    canvas->setDrawBack(false); 
        
    canvas->setFont("verdana.ttf");                     
    canvas->setFontSize(OFX_UI_FONT_LARGE, 7);            
    canvas->setFontSize(OFX_UI_FONT_MEDIUM, 6);           
    canvas->setFontSize(OFX_UI_FONT_SMALL, 5);            
            
    canvasArray.push_back(canvas);
  }

  virtual ~GUImodule(){ }

  virtual void addWidget(ofPtr<ofxUIWidget> _widget) {
    widgets.push_back(_widget);
  }

  virtual void addSlider(ofPtr<ofxUISlider> _slider) {
    sliders.push_back(_slider);
  }

  virtual void addRangeSlider(ofPtr<ofxUIRangeSlider> _rangeSlider) {
    rangeSliders.push_back(_rangeSlider);
  }

  virtual void addRotarySlider(ofPtr<ofxUIRotarySlider> _rotarySlider) {
    rotarySliders.push_back(_rotarySlider);
  }

  virtual void addLabelToggle( ofPtr<ofxUILabelToggle> _labelToggle ) {
    labelToggles.push_back( _labelToggle );
  }
    
  virtual void addDropDown( ofPtr<ofxUIDropDownList> _dropdown ) {
    dropdowns.push_back( _dropdown );
  }
    
  virtual void addTextInput( ofPtr<ofxUITextInput> _input ) {
    textInputs.push_back( _input );
  }
    
  virtual void addButton( ofPtr<ofxUIButton> _button ) {
    buttons.push_back( _button );
  }

  virtual void addMatrix( ofPtr<ofxUIToggleMatrix> _matrix ) {
    matrix.push_back( _matrix );
  }
    
  ofPtr<ofxUISlider> getSlider( int _i ) {
    if(_i < sliders.size())
      return sliders[_i];
    else
      return ofPtr<ofxUISlider>();
  }

  ofPtr<ofxUIRangeSlider> getRangeSlider( int _i ) {
    if(_i < rangeSliders.size())
      return rangeSliders[_i];
    else
      return ofPtr<ofxUIRangeSlider>();
  }

  ofPtr<ofxUILabelToggle> getLabelToggle( int _i ) {
    if(_i < labelToggles.size())
      return labelToggles[_i];
    else
      return ofPtr<ofxUILabelToggle>();
  }
    
  ofPtr<ofxUIDropDownList> getDropDown( int _i ) {
    if(_i < dropdowns.size())
      return dropdowns[_i];
    else
      return ofPtr<ofxUIDropDownList>();
	      
  }
    
  ofPtr<ofxUIButton> getButton( int _i ) {
    if(_i < buttons.size())
      return buttons[_i];
    else
      return ofPtr<ofxUIButton>();
  }
    
  ofPtr<ofxUITextInput> getTextInput( int _i ) {
    if(_i < textInputs.size())
      return textInputs[_i];
    else
      return ofPtr<ofxUITextInput>();
  }

  ofPtr<ofxUIToggleMatrix> getMatrix( int _i ) {
    if(_i < matrix.size())
      return matrix[_i];
    else
      return ofPtr<ofxUIToggleMatrix>();
  }

  int getCanvasNum( ) {
    return canvasArray.size();
  }
    
  ofPtr<ofxUICanvas> getCanvas( int _i ) {
    if(_i < canvasArray.size())
      return canvasArray[_i];
    else
      return ofPtr<ofxUICanvas>();
  }

 protected:
  ofPtr<ofxUICanvas> canvas;
  vector< ofPtr<ofxUICanvas> > canvasArray;

  vector< ofPtr<ofxUISlider> >           sliders;
  vector< ofPtr<ofxUIRangeSlider> >      rangeSliders;
  vector< ofPtr<ofxUIRotarySlider> >     rotarySliders;
  vector< ofPtr<ofxUILabelToggle> >      labelToggles;
  vector< ofPtr<ofxUIDropDownList> >     dropdowns;
  vector< ofPtr<ofxUITextInput> >        textInputs;
  vector< ofPtr<ofxUIButton> >           buttons;

  vector< ofPtr<ofxUIToggleMatrix> >     matrix;

  vector< ofPtr<ofxUIWidget> > widgets;

  float x,y,width,height;
};

class SensorDialog: public GUImodule{
 public:
 SensorDialog(float _x, float _y, float _w, float _h):GUImodule(_x,_y,_w,_h){
    x=_x; y=_y; width=_w; height=_h;
  }
  void setSerialList( vector<string> _serialList ){
    serialList = _serialList;
  }
    
  void init () {
    ofxUIDropDownList * dropdown;
    ofxUISlider * slider;
    ofxUITextInput * input;
    ofxUIRangeSlider * rslider;
    ofxUIButton * button;
        
    canvasL = ofPtr<ofxUICanvas>(new ofxUICanvas(x, y+60, width/2, height-20));
    canvasL->setDrawBack(false); 
        
    canvasL->setFont("verdana.ttf");                     
    canvasL->setFontSize(OFX_UI_FONT_LARGE, 7);            
    canvasL->setFontSize(OFX_UI_FONT_MEDIUM, 6);           
    canvasL->setFontSize(OFX_UI_FONT_SMALL, 5);     
        
    canvasArray.push_back( canvasL );
    canvasR = ofPtr<ofxUICanvas>(new ofxUICanvas(x+width/2, y+60, width/2, height-20));
    canvasR->setDrawBack(false); 
        
    canvasR->setFont("verdana.ttf");                     
    canvasR->setFontSize(OFX_UI_FONT_LARGE, 7);            
    canvasR->setFontSize(OFX_UI_FONT_MEDIUM, 6);           
    canvasR->setFontSize(OFX_UI_FONT_SMALL, 5);     
        
    canvasArray.push_back( canvasR );

    dropdown = new ofxUIDropDownList(width/2, "SERIALPORT", serialList, OFX_UI_FONT_SMALL);
    canvas->addWidgetLeft(dropdown);
    ofPtr<ofxUIDropDownList> shrdropdown(dropdown,null_deleter());
    addDropDown( shrdropdown );
    dropdown->setDrawBack( true ); 
    dropdown->setAutoClose( true );

    button = new ofxUIButton( width/2, 20, false, "SAVERANGES" );
    button->setLabelVisible(true);
    canvas->addWidgetRight(button);
    ofPtr<ofxUIButton> shrbutton(button,null_deleter());
    addButton( shrbutton );
    button->setDrawBack( true ); 
        
    canvas->addWidgetDown(new ofxUISpacer(width, 1));         

    for (int i=0; i<6; i++) {
      input = new ofxUITextInput(width*0.45f, "nombre", "osc"+ofToString(i), OFX_UI_FONT_SMALL);
      ofPtr<ofxUITextInput> shrinput(input,null_deleter());
      addTextInput( shrinput );
      input->setAutoClear(false); 
            
      rslider = new ofxUIRangeSlider(width*0.45f,10, 0.0, 1023.0, 0.0, 1034.0, "RANGE"+ofToString(i));
      //rslider->setLabelVisible(false);
      rslider->setPadding(0);
            
      ofPtr<ofxUIRangeSlider> shrrslider(rslider,null_deleter());
      addRangeSlider( shrrslider );
            
      slider = new ofxUISlider(width*0.45f,10, 0.0, 1023.0, 0.0, "SLIDER"+ofToString(i));
      //slider->setLabelVisible(false);
      slider->setPadding(0);
            
      ofPtr<ofxUISlider> shrslider(slider,null_deleter());
      addSlider( shrslider );
            
      if(i<3) {
	canvasL->addWidgetDown(input);
	canvasL->addWidgetDown(rslider);
	canvasL->addWidgetDown(slider);
      }
      else {
	canvasR->addWidgetDown(input);
	canvasR->addWidgetDown(rslider);
	canvasR->addWidgetDown(slider);
      }
    }
  }
  virtual ~SensorDialog(){}
        
 protected:
  vector<string> serialList;
  ofPtr<ofxUICanvas> canvasL,canvasR;
};

class Presets: public GUImodule{    
 public:
 Presets(float _x, float _y, float _w, float _h):GUImodule(_x,_y,_w,_h){
    x=_x; y=_y; width=_w; height=_h;
        
    canvas->addWidgetDown(new ofxUIButton(30,30,30,10,false,"guardar"));
    canvas->addWidgetRight( new ofxUITextInput(width/2, "nombre", "preset", OFX_UI_FONT_SMALL ));

        
    //canvas->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM));
    ofxUIToggleMatrix * matrix;
    canvas->addWidgetDown(new ofxUISpacer(width, 2));             
    canvas->addWidgetDown(new ofxUILabel("presets", OFX_UI_FONT_MEDIUM)); 				
    matrix=new ofxUIToggleMatrix(width/10, width/10, 1, 8, "bancos");
    matrix->setAllowMultiple(false);
    canvas->addWidgetDown(matrix); 
    ofPtr<ofxUIToggleMatrix> shrmatrix(matrix,null_deleter());
        
    addMatrix(shrmatrix);
        
    matrix=new ofxUIToggleMatrix(width/10, width/10, 4,8, "presets");
    matrix->setAllowMultiple(false);
    canvas->addWidgetDown(matrix); 
    ofPtr<ofxUIToggleMatrix>shrmatrix2(matrix,null_deleter());
        
        
    addMatrix(shrmatrix2);
  }

  virtual ~Presets(){}
};

class GUImode{
 public:
  GUImode(){}
  virtual ~GUImode(){}

  void addModule(ofPtr<GUImodule> _module ) {
    modules.push_back(_module);
  }
    
  int getModuleNum(){
      return modules.size();
  }
    
  ofPtr<GUImodule> getModule( int _i ) {
    if(_i < modules.size())
      return modules[_i];
    else
      return ofPtr<GUImodule>();
  }

 protected:
  vector< ofPtr<GUImodule> > modules;

};




class GUI{
 public:
  GUI(){ }
  ~GUI(){}

  void addMode(ofPtr<GUImode> _mode ) {
    modes.push_back(_mode);
  }
  vector< ofPtr<GUImode> > modes;
};
