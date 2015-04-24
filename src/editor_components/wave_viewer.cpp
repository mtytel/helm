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
#include "synth_gui_interface.h"
//[/Headers]

#include "wave_viewer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define GRID_CELL_WIDTH 8
#define FRAMES_PER_SECOND 24
#define PADDING 5.0f
//[/MiscUserDefs]

//==============================================================================
WaveViewer::WaveViewer (int resolution)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    wave_slider_ = nullptr;
    amplitude_slider_ = nullptr;
    resolution_ = resolution;
    wave_state_ = nullptr;
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

WaveViewer::~WaveViewer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WaveViewer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));
    //[/UserPrePaint]

    g.fillAll (Colour (0xff424242));

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff4a4a4a));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    shadow.drawForPath(g, wave_path_);

    g.setColour(Colour(0xff565656));
    g.fillPath(wave_path_);
    g.setColour(Colour(0xff03a9f4));
    g.strokePath(wave_path_, PathStrokeType(1.5f, PathStrokeType::beveled, PathStrokeType::rounded));

    if (wave_state_) {
        float phase = wave_state_->buffer[0];
        float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;

        if (phase >= 0.0 && phase < 1.0) {
            float x = PADDING + phase * (getWidth() - 2 * PADDING);
            g.setColour(Colour(0x66ffffff));
            g.fillRect(x, 0.0f, 1.0f, (float)getHeight());

            mopo::Wave::Type type = static_cast<mopo::Wave::Type>(static_cast<int>(wave_slider_->getValue()));
            float value = amplitude * mopo::Wave::wave(type, phase);
            float y = PADDING + (getHeight() - 2 * PADDING) * (1.0f - value) / 2.0f;
            g.setColour(Colour(0xffffffff));
            g.fillEllipse(x - 2.0f, y - 2.0f, 4.0f, 4.0f);
        }
    }
    //[/UserPaint]
}

void WaveViewer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    resetWavePath();
    //[/UserResized]
}

void WaveViewer::mouseDown (const MouseEvent& e)
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

void WaveViewer::update() {
}

void WaveViewer::setWaveSlider(Slider* slider) {
    if (wave_slider_)
        wave_slider_->removeListener(this);
    wave_slider_ = slider;
    wave_slider_->addListener(this);
    resetWavePath();
    repaint();
}

void WaveViewer::setAmplitudeSlider(Slider* slider) {
    if (amplitude_slider_)
        amplitude_slider_->removeListener(this);
    amplitude_slider_ = slider;
    amplitude_slider_->addListener(this);
    resetWavePath();
}

void WaveViewer::resetWavePath() {
    wave_path_.clear();

    if (wave_slider_ == nullptr)
        return;

    float amplitude = amplitude_slider_ ? amplitude_slider_->getValue() : 1.0f;
    float draw_width = getWidth() - 2.0f * PADDING;
    float draw_height = getHeight() - 2.0f * PADDING;

    wave_path_.startNewSubPath(PADDING, getHeight() / 2.0f);
    mopo::Wave::Type type = static_cast<mopo::Wave::Type>(static_cast<int>(wave_slider_->getValue()));
    for (int i = 1; i < resolution_ - 1; ++i) {
        float t = (1.0f * i) / resolution_;
        float val = amplitude * mopo::Wave::wave(type, t);
        wave_path_.lineTo(PADDING + t * draw_width, PADDING + draw_height * ((1.0f - val) / 2.0f));
    }

    wave_path_.lineTo(getWidth() - PADDING, getHeight() / 2.0f);
}

void WaveViewer::sliderValueChanged(Slider* sliderThatWasMoved) {
    resetWavePath();
}

void WaveViewer::showRealtimeFeedback() {
    if (wave_state_ == nullptr) {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        if (parent) {
            wave_state_ = parent->getModSource(getName().toStdString());
            setFramesPerSecond(FRAMES_PER_SECOND);
        }
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WaveViewer" componentName=""
                 parentClasses="public AnimatedAppComponent, SliderListener" constructorParams="int resolution"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff424242"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
