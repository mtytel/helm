/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "global_tool_tip.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define FRAMES_PER_SECOND 20
//[/MiscUserDefs]

//==============================================================================
GlobalToolTip::GlobalToolTip ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    startTimerHz(FRAMES_PER_SECOND);
    //[/Constructor]
}

GlobalToolTip::~GlobalToolTip()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GlobalToolTip::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff444444));
    g.fillRect(0, 0, getWidth(), getHeight() / 2);

    g.setColour(Colour(0xff424242));
    g.fillRect(0, getHeight() / 2, getWidth(), getHeight() / 2);

    g.setFont(Font(Font::getDefaultMonospacedFontName(), 13.0f, Font::plain));
    g.setColour(Colour(0xffffffff));
    g.drawText(parameter_text_, 0.0, 0.0,
               getWidth(), proportionOfHeight(0.5), Justification::centred);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 14.0f, Font::plain));
    g.drawText(value_text_, 0.0, proportionOfHeight(0.5),
               getWidth(), proportionOfHeight(0.5), Justification::centred);
    //[/UserPaint]
}

void GlobalToolTip::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GlobalToolTip::setText(String parameter, String value) {
    parameter_text_ = parameter;
    value_text_ = value;
}

void GlobalToolTip::timerCallback() {
    if (shown_parameter_text_ != parameter_text_ || shown_value_text_ != value_text_) {
        shown_value_text_ = value_text_;
        shown_parameter_text_ = parameter_text_;
        repaint();
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GlobalToolTip" componentName=""
                 parentClasses="public Component, public Timer" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
