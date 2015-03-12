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

#include "wave_form_selector.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define GRID_CELL_WIDTH 10
//[/MiscUserDefs]

//==============================================================================
WaveFormSelector::WaveFormSelector (int resolution)
{

    //[UserPreSize]
    wave_slider_ = nullptr;
    resolution_ = resolution;
    setOpaque(true);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WaveFormSelector::~WaveFormSelector()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WaveFormSelector::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff33064f));

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff43165f));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    g.setColour(Colour(0xff43165f));
    g.fillPath(wave_path_);
    g.setColour(Colour(0xffaaaaaa));
    g.strokePath(wave_path_, PathStrokeType(1.0f, PathStrokeType::beveled, PathStrokeType::rounded));
    //[/UserPaint]
}

void WaveFormSelector::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    resetWavePath();
    //[/UserResized]
}

void WaveFormSelector::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    if (wave_slider_) {
        int current_value = wave_slider_->getValue();
        if (e.mods.isRightButtonDown())
            current_value = current_value + wave_slider_->getMaximum();
        else
            current_value = current_value + 1;
        wave_slider_->setValue(current_value % static_cast<int>(wave_slider_->getMaximum() + 1));

        resetWavePath();
        repaint();
    }
    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void WaveFormSelector::setWaveSlider(Slider* slider) {
    if (wave_slider_)
        wave_slider_->removeListener(this);
    wave_slider_ = slider;
    wave_slider_->addListener(this);
    resetWavePath();
    repaint();
}

void WaveFormSelector::resetWavePath() {
    static const float padding = 5.0f;
    wave_path_.clear();

    if (wave_slider_ == nullptr)
        return;

    float draw_width = getWidth() - 2.0f * padding;
    float draw_height = getHeight() - 2.0f * padding;

    wave_path_.startNewSubPath(padding, getHeight() / 2.0f);
    mopo::Wave::Type type = static_cast<mopo::Wave::Type>(wave_slider_->getValue());
    for (int i = 1; i < resolution_ - 1; ++i) {
        float t = (1.0f * i) / resolution_;
        float val = mopo::Wave::wave(type, t);
        wave_path_.lineTo(padding + t * draw_width, padding + draw_height * ((1.0f - val) / 2.0f));
    }

    wave_path_.lineTo(getWidth() - padding, getHeight() / 2.0f);
}

void WaveFormSelector::sliderValueChanged(Slider* sliderThatWasMoved) {
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

<JUCER_COMPONENT documentType="Component" className="WaveFormSelector" componentName=""
                 parentClasses="public Component, SliderListener" constructorParams="int resolution"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff33064f"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
