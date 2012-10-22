

struct null_deleter
{
    void operator()(void const *) const
    {
    }
};



class GUImodule{

    public:
        GUImodule(float _x, float _y, float _w, float _h){
            x=_x; y=_y; width=_w; height=_h;
            canvas = make_shared<ofxUICanvas>(_x, _y, _w, _h);
            canvas->setDrawBack(false); 
        
            canvas->setFont("franklinGothic.otf");                     
            canvas->setFontSize(OFX_UI_FONT_LARGE, 7);            
            canvas->setFontSize(OFX_UI_FONT_MEDIUM, 6);           
            canvas->setFontSize(OFX_UI_FONT_SMALL, 5);            
            
            canvasArray.push_back(canvas);


        }
        virtual ~GUImodule(){
            //canvas;
        }


        virtual void addWidget(shared_ptr<ofxUIWidget> _widget) {
            widgets.push_back(_widget);
        }

        virtual void addSlider(shared_ptr<ofxUISlider> _slider) {
            sliders.push_back(_slider);
        }

        virtual void addRangeSlider(shared_ptr<ofxUIRangeSlider> _rangeSlider) {
            rangeSliders.push_back(_rangeSlider);
        }

        virtual void addRotarySlider(shared_ptr<ofxUIRotarySlider> _rotarySlider) {
            rotarySliders.push_back(_rotarySlider);
        }

        virtual void addLabelToggle( shared_ptr<ofxUILabelToggle> _labelToggle ) {
            labelToggles.push_back( _labelToggle );
        }
    
        virtual void addDropDown( shared_ptr<ofxUIDropDownList> _dropdown ) {
            dropdowns.push_back( _dropdown );
        }
    
        virtual void addTextInput( shared_ptr<ofxUITextInput> _input ) {
            textInputs.push_back( _input );
        }
    
        virtual void addButton( shared_ptr<ofxUIButton> _button ) {
            buttons.push_back( _button );
        }

        virtual void addMatrix( shared_ptr<ofxUIToggleMatrix> _matrix ) {
            matrix.push_back( _matrix );
        }
    
        shared_ptr<ofxUISlider> getSlider( int _i ) {
            if(sliders.size())
                return sliders[_i];
        }

        shared_ptr<ofxUIRangeSlider> getRangeSlider( int _i ) {
            if(rangeSliders.size())
                return rangeSliders[_i];
        }

        shared_ptr<ofxUILabelToggle> getLabelToggle( int _i ) {
            if(labelToggles.size())
                return labelToggles[_i];
        }
    
        shared_ptr<ofxUIDropDownList> getDropDown( int _i ) {
            if(dropdowns.size())
                return dropdowns[_i];
        }
    
        shared_ptr<ofxUIButton> getButton( int _i ) {
            if(buttons.size())
                return buttons[_i];
        }
    
        shared_ptr<ofxUITextInput> getTextInput( int _i ) {
            if(textInputs.size())
                return textInputs[_i];
        }

        shared_ptr<ofxUIToggleMatrix> getMatrix( int _i ) {
            if(matrix.size())
                return matrix[_i];
        }
    
    
        int getCanvasNum( ) {
            if(canvasArray.size() )
                return canvasArray.size();
        }
    
        shared_ptr<ofxUICanvas> getCanvas( int _i ) {
            if(canvasArray.size())
                if(_i<canvasArray.size())
                    return canvasArray[_i];
        }



    protected:

        shared_ptr<ofxUICanvas> canvas;
        vector< shared_ptr<ofxUICanvas> > canvasArray;

        vector< shared_ptr<ofxUISlider> >           sliders;
        vector< shared_ptr<ofxUIRangeSlider> >      rangeSliders;
        vector< shared_ptr<ofxUIRotarySlider> >     rotarySliders;
        vector< shared_ptr<ofxUILabelToggle> >      labelToggles;
        vector< shared_ptr<ofxUIDropDownList> >     dropdowns;
        vector< shared_ptr<ofxUITextInput> >        textInputs;
        vector< shared_ptr<ofxUIButton> >           buttons;

        vector< shared_ptr<ofxUIToggleMatrix> >     matrix;

        vector< shared_ptr<ofxUIWidget> > widgets;

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
        
        canvasL = make_shared<ofxUICanvas>(x, y+60, width/2, height-20);
        canvasL->setDrawBack(false); 
        
        canvasL->setFont("franklinGothic.otf");                     
        canvasL->setFontSize(OFX_UI_FONT_LARGE, 7);            
        canvasL->setFontSize(OFX_UI_FONT_MEDIUM, 6);           
        canvasL->setFontSize(OFX_UI_FONT_SMALL, 5);     
        
        canvasArray.push_back( canvasL );
        canvasR = make_shared<ofxUICanvas>(x+width/2, y+60, width/2, height-20);
        canvasR->setDrawBack(false); 
        
        canvasR->setFont("franklinGothic.otf");                     
        canvasR->setFontSize(OFX_UI_FONT_LARGE, 7);            
        canvasR->setFontSize(OFX_UI_FONT_MEDIUM, 6);           
        canvasR->setFontSize(OFX_UI_FONT_SMALL, 5);     
        
        canvasArray.push_back( canvasR );


        dropdown = new ofxUIDropDownList(width/2, "SERIALPORT", serialList, OFX_UI_FONT_SMALL);
        canvas->addWidgetLeft(dropdown);
        shared_ptr<ofxUIDropDownList> shrdropdown(dropdown,null_deleter());
        addDropDown( shrdropdown );
        dropdown->setDrawBack( true ); 
        dropdown->setAutoClose( true );

        button = new ofxUIButton( width/2, 20, false, "SAVERANGES" );
        button->setLabelVisible(true);
        canvas->addWidgetRight(button);
        shared_ptr<ofxUIButton> shrbutton(button,null_deleter());
        addButton( shrbutton );
        button->setDrawBack( true ); 
        
       
        
        canvas->addWidgetDown(new ofxUISpacer(width, 1));         

        for (int i=0; i<6; i++)
        {
            
            input = new ofxUITextInput(width*0.45f, "nombre", "osc"+ofToString(i), OFX_UI_FONT_SMALL);
            shared_ptr<ofxUITextInput> shrinput(input,null_deleter());
            addTextInput( shrinput );
            input->setAutoClear(false); 

            
            rslider = new ofxUIRangeSlider(width*0.45f,10, 0.0, 1023.0, 0.0, 1034.0, "RANGE"+ofToString(i));
            //rslider->setLabelVisible(false);
            rslider->setPadding(0);
            
            shared_ptr<ofxUIRangeSlider> shrrslider(rslider,null_deleter());
            addRangeSlider( shrrslider );
            
            slider = new ofxUISlider(width*0.45f,10, 0.0, 1023.0, 0.0, "SLIDER"+ofToString(i));
            //slider->setLabelVisible(false);
            slider->setPadding(0);
            
            shared_ptr<ofxUISlider> shrslider(slider,null_deleter());
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
    shared_ptr<ofxUICanvas> canvasL,canvasR;


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
        shared_ptr<ofxUIToggleMatrix> shrmatrix(matrix,null_deleter());
        
        addMatrix(shrmatrix);
        
        matrix=new ofxUIToggleMatrix(width/10, width/10, 4,8, "presets");
        matrix->setAllowMultiple(false);
        canvas->addWidgetDown(matrix); 
        shared_ptr<ofxUIToggleMatrix>shrmatrix2(matrix,null_deleter());
        
        
        addMatrix(shrmatrix2);
        
        //slider = new ofxUISlider((_w*0.15f), _h*0.5f, 0.0, 1.0, 0.4f, "faderChan1");
        //slider->setPadding(_w*0.15f);
        //canvas->addWidgetSouthOf(slider,"spacer");
        
        
        
        
    }
    
    virtual ~Presets(){}
    
};


/*
class Player: public GUImodule{
    
public:
    Player(float _x, float _y, float _w, float _h):GUImodule(_x,_y,_w,_h){
        
        x=_x; y=_y; width=_w; height=_h;
        
        canvas->addWidgetDown(new ofxUISpacer(_w, 2));
        
        canvas->addWidgetDown(new ofxUILabel("player", OFX_UI_FONT_MEDIUM));
        //            ofxUISlider * slider;
        ofxUIDropDownList * dropdown;
        
        ofxUILabelToggle * button;

        ofxUISlider * slider;
        
        ofxUIRangeSlider * rslider;
        
         
         vector<string> strvec;
         //strvec.push_back("hola");
         //strvec.push_back("lex");
         
         dropdown = new ofxUIDropDownList(width, "SERIALPORT", strvec, 30);
         canvas->addWidgetDown(dropdown);
         shared_ptr<ofxUIDropDownList> shrdropdown(dropdown,null_deleter());
         addDropDown( shrdropdown );
         
        for (int i=0; i<6; i++)
        {
            
            button = new ofxUILabelToggle(30, false, "rola"+ofToString(i), OFX_UI_FONT_SMALL);
            
            
            canvas->addWidgetRight(button);
            
            
            shared_ptr<ofxUILabelToggle> shrbutton(button,null_deleter());
            addLabelToggle( shrbutton );
            
        }
        
        slider = new ofxUISlider(width, 20, 0.0, 1.0, 0.4f, "playheadSlider");
        //slider->setPadding(_w*0.15f);
        canvas->addWidgetDown(slider);

        rslider = new ofxUIRangeSlider(width, 20, 0.0, 1.0, 0.0, 0.0,  "playRangeSlider");
        //rslider->setPadding(_w*0.15f);
        canvas->addWidgetDown(rslider);

        
        
    }
    
    virtual ~Player(){}
    
};


class SongListGUI: public GUImodule{

    public:
        SongListGUI(float _x, float _y, float _w, float _h):GUImodule(_x,_y,_w,_h){

            x=_x; y=_y; width=_w; height=_h;
            scrollCanvas = make_shared<ofxUIScrollableCanvas>(_x, _y, _w, _h);
            scrollCanvas->setDrawBack(false); 

            
            scrollCanvas->addWidgetDown(new ofxUISpacer(_w, 2));

            scrollCanvas->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM));
//            ofxUISlider * slider;
            ofxUILabelToggle * button;

            arrangeCanvas();


        }
    
        void addSong(string _songName){
            ofxUILabelToggle * button = new ofxUILabelToggle(width, false, _songName, OFX_UI_FONT_SMALL);
            
            
            scrollCanvas->addWidgetDown(button);
            
            shared_ptr<ofxUILabelToggle> shrbutton(button,null_deleter());
            addLabelToggle( shrbutton );
        
        
            arrangeCanvas();
    
        }
    
        void arrangeCanvas() {
            //scrollCanvas->setScrollAreaToScreen();
            scrollCanvas->setScrollableDirections(false,true);
            //scrollCanvas->autoSizeToFitWidgets();
            scrollCanvas->getRect()->setWidth(width);
            scrollCanvas->getRect()->setHeight(height);
        
        }


        virtual ~SongListGUI(){}
        
        shared_ptr<ofxUIScrollableCanvas> scrollCanvas;
};




class EqChannel: public GUImodule{

    public:
        EqChannel(float _x, float _y, float _w, float _h):GUImodule(_x,_y,_w,_h){

            x=_x; y=_y; width=_w; height=_h;

            canvas->addWidgetDown(new ofxUISpacer(_w, 2));

            canvas->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM));
            ofxUIRotarySlider * slider;
            ofxUILabelToggle * button;


            button = new ofxUILabelToggle(width, false, "on", OFX_UI_FONT_SMALL);
            canvas->addWidgetDown(button);

            for (int i=0; i<4; i++)
            {
                canvas->addWidgetDown(new ofxUISpacer(width, 1));
                slider = new ofxUIRotarySlider(width/3, 0.0, 100.0, 25.0, "rslider"+ofToString(i));
                slider->setPadding(0);
                canvas->addWidgetDown(slider);
                shared_ptr<ofxUIRotarySlider>  shrslider (slider,null_deleter());
                addRotarySlider( shrslider );
            }

            shared_ptr<ofxUILabelToggle> shrbutton(button,null_deleter());
            addLabelToggle( shrbutton );



        }

        virtual ~EqChannel(){}

};





class StepSeq: public GUImodule{
    
public:
    StepSeq(float _x, float _y, float _w, float _h):GUImodule(_x,_y,_w,_h){
        
        x=_x; y=_y; width=_w; height=_h;

        canvas->addWidgetDown(new ofxUIButton(30,30,10,10,false,"play"));
        canvas->addWidgetRight(new ofxUIButton(30,30,10,10,false,"play2"));
        canvas->addWidgetRight(new ofxUIButton(30,30,10,10,false,"play3"));
        canvas->addWidgetRight(new ofxUIButton(30,30,10,10,false,"play4"));
        
        canvas->addWidgetDown(new ofxUISpacer(_w, 2, "spacer"));
        
        //canvas->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM));
        ofxUIToggleMatrix * matrix;
        canvas->addWidgetDown(new ofxUISpacer(width, 2));             
        canvas->addWidgetDown(new ofxUILabel("MATRIX", OFX_UI_FONT_MEDIUM)); 				
        matrix=new ofxUIToggleMatrix(width*0.125f, width*0.125f, 4, 4, "MATRIX");
        canvas->addWidgetDown(matrix); 
        shared_ptr<ofxUIToggleMatrix> shrmatrix(matrix,null_deleter());
        
        addMatrix(shrmatrix);

        matrix=new ofxUIToggleMatrix(width/20, width/20, 6, 16, "MATRIX");
        canvas->addWidgetDown(matrix); 
        shared_ptr<ofxUIToggleMatrix>shrmatrix2(matrix,null_deleter());
        
        
        addMatrix(shrmatrix2);
        
        //slider = new ofxUISlider((_w*0.15f), _h*0.5f, 0.0, 1.0, 0.4f, "faderChan1");
        //slider->setPadding(_w*0.15f);
        //canvas->addWidgetSouthOf(slider,"spacer");
        
        
        
        
    }
    
    virtual ~StepSeq(){}
    
};


*/


class GUImode{

    public:
        GUImode(){}
        virtual ~GUImode(){}

        void addModule(shared_ptr<GUImodule> _module ) {
            modules.push_back(_module);
        }
    
        int getModuleNum(){
            if(modules.size())
                return modules.size();
            else
                return 0;
        }
    
        shared_ptr<GUImodule> getModule( int _i ) {
            if(modules.size())
                return modules[_i];
        }

    protected:
        vector< shared_ptr<GUImodule> > modules;

};




class GUI{
    public:
        GUI(){ init(); }
        ~GUI(){}

        void init(){
        }

        void addMode(shared_ptr<GUImode> _mode ) {
            modes.push_back(_mode);
        }

    vector< shared_ptr<GUImode> > modes;

};
