/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "oscilloscope.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define FRAMES_PER_SECOND 30
#define MAX_RESOLUTION 128
#define GRID_CELL_WIDTH 10
//[/MiscUserDefs]

//==============================================================================
Oscilloscope::Oscilloscope (int num_samples)
{

    //[UserPreSize]
    output_memory_ = nullptr;
    setOpaque(true);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    samples_to_show_ = num_samples;
    setFramesPerSecond(FRAMES_PER_SECOND);
    //[/Constructor]
}

Oscilloscope::~Oscilloscope()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Oscilloscope::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff246370));

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff347380));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    g.setColour(Colour(0xff448390));
    g.fillPath(wave_path_);
    g.setColour(Colour(0xffaaaaaa));
    g.strokePath(wave_path_, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    //[/UserPaint]
}

void Oscilloscope::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    resetWavePath();
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void Oscilloscope::resetWavePath() {
    static const float padding = 5.0f;

    if (output_memory_ == nullptr)
        return;

    wave_path_.clear();

    float draw_width = getWidth() - 2.0f * padding;
    float draw_height = getHeight() - 2.0f * padding;

    wave_path_.startNewSubPath(padding, getHeight() / 2.0f);
    int inc = samples_to_show_ / MAX_RESOLUTION;
    for (int i = samples_to_show_; i >= 0; i -= inc) {
        float t = (samples_to_show_ - 1.0f * i) / samples_to_show_;
        float val = output_memory_->get(i);
        if (val != val)
            val = 0.0f;
        wave_path_.lineTo(padding + t * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
    }

    wave_path_.lineTo(getWidth() - padding, getHeight() / 2.0f);
}

void Oscilloscope::update() {
    resetWavePath();
    repaint();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Oscilloscope" componentName=""
                 parentClasses="public AnimatedAppComponent" constructorParams="int num_samples"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff246370"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
