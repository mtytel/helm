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
#include "midi_lookup.h"
//[/Headers]

#include "filter_response.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define MAG_TO_DB_CONSTANT 20.0f
#define MIN_GAIN_DB -30.0f
#define MAX_GAIN_DB 24.0f
#define MIN_RESONANCE 0.5
#define MAX_RESONANCE 16.0
#define GRID_CELL_WIDTH 10
//[/MiscUserDefs]

//==============================================================================
FilterResponse::FilterResponse (int resolution)
{

    //[UserPreSize]
    resolution_ = resolution;
    cutoff_slider_ = nullptr;
    resonance_slider_ = nullptr;
    filter_type_slider_ = nullptr;
    setOpaque(true);

    filter_.setSampleRate(44100);
    resetResponsePath();
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

FilterResponse::~FilterResponse()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FilterResponse::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    static const PathStrokeType stroke(1.0f, PathStrokeType::beveled, PathStrokeType::rounded);
    //[/UserPrePaint]

    g.fillAll (Colour (0xff246370));

    //[UserPaint] Add your own custom painting code here..

    g.setColour(Colour(0xff347380));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    g.setGradientFill(ColourGradient(Colour(0xffffffff), 0.0f, getHeight() * 0.1f,
                                     Colour(0x55347380), 0.0f, getHeight(), false));
    g.fillPath(filter_response_path_);

    g.setColour(Colour(0xffaaaaaa));
    g.strokePath(filter_response_path_, stroke);
    //[/UserPaint]
}

void FilterResponse::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    computeFilterCoefficients();
    resetResponsePath();
    //[/UserResized]
}

void FilterResponse::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    if (e.mods.isRightButtonDown() && filter_type_slider_) {
        int max = filter_type_slider_->getMaximum();
        int current_value = filter_type_slider_->getValue();
        filter_type_slider_->setValue((current_value + 1) % (max + 1));

        computeFilterCoefficients();
    }
    else
        setFilterSettingsFromPosition(e.getPosition());
    repaint();
    //[/UserCode_mouseDown]
}

void FilterResponse::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    if (!e.mods.isRightButtonDown()) {
        setFilterSettingsFromPosition(e.getPosition());
        repaint();
    }
    //[/UserCode_mouseDrag]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void FilterResponse::resetResponsePath() {
    static const int wrap_size = 10;

    if (cutoff_slider_ == nullptr)
        return;

    filter_response_path_.clear();
    filter_response_path_.startNewSubPath(-wrap_size, getHeight() + wrap_size);
    filter_response_path_.lineTo(-wrap_size, getHeight() / 2.0f);

    for (int i = 0; i < resolution_; ++i) {
        float t = (1.0f * i) / (resolution_ - 1);
        float midi_note = cutoff_slider_->proportionOfLengthToValue(t);
        float frequency = mopo::utils::midiNoteToFrequency(midi_note);
        float response = fabs(filter_.getAmplitudeResponse(frequency));
        float gain_db = MAG_TO_DB_CONSTANT * log10(response);
        float percent = (gain_db - MIN_GAIN_DB) / (MAX_GAIN_DB - MIN_GAIN_DB);

        filter_response_path_.lineTo(getWidth() * t, getHeight() * (1.0f - percent));
    }

    filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() / 2.0f);
    filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() + wrap_size);
}

void FilterResponse::computeFilterCoefficients() {
    if (cutoff_slider_ == nullptr || resonance_slider_ == nullptr || filter_type_slider_ == nullptr)
        return;

    mopo::Filter::Type type = static_cast<mopo::Filter::Type>(static_cast<int>(filter_type_slider_->getValue()));
    double frequency = mopo::utils::midiNoteToFrequency(cutoff_slider_->getValue());
    double resonance = mopo::utils::magnitudeToQ(resonance_slider_->getValue());
    double decibals = INTERPOLATE(MIN_GAIN_DB, MAX_GAIN_DB, resonance_slider_->getValue());
    double gain = mopo::utils::dbToGain(decibals);
    filter_.computeCoefficients(type, frequency, resonance, gain);
    resetResponsePath();
}

void FilterResponse::setFilterSettingsFromPosition(Point<int> position) {
    mopo::Filter::Type type = mopo::Filter::kAllPass;
    if (filter_type_slider_)
        type = static_cast<mopo::Filter::Type>(static_cast<int>(filter_type_slider_->getValue()));

    if (cutoff_slider_) {
        double percent = CLAMP((1.0 * position.x) / getWidth(), 0.0, 1.0);
        double frequency = cutoff_slider_->proportionOfLengthToValue(percent);
        cutoff_slider_->setValue(frequency);
    }
    if (resonance_slider_) {
        double percent = CLAMP(1.0 - (1.0 * position.y) / getHeight(), 0.0, 1.0);
        resonance_slider_->setValue(resonance_slider_->proportionOfLengthToValue(percent));
    }

    computeFilterCoefficients();
}

void FilterResponse::sliderValueChanged(Slider* moved_slider) {
    computeFilterCoefficients();
    repaint();
}

void FilterResponse::setResonanceSlider(Slider* slider) {
    if (resonance_slider_)
        resonance_slider_->removeListener(this);
    resonance_slider_ = slider;
    resonance_slider_->addListener(this);
    computeFilterCoefficients();
    repaint();
}

void FilterResponse::setCutoffSlider(Slider* slider) {
    if (cutoff_slider_)
        cutoff_slider_->removeListener(this);
    cutoff_slider_ = slider;
    cutoff_slider_->addListener(this);
    computeFilterCoefficients();
    repaint();
}

void FilterResponse::setFilterTypeSlider(Slider* slider) {
    if (filter_type_slider_)
        filter_type_slider_->removeListener(this);
    filter_type_slider_ = slider;
    filter_type_slider_->addListener(this);
    computeFilterCoefficients();
    repaint();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterResponse" componentName=""
                 parentClasses="public Component, SliderListener" constructorParams="int resolution"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff246370"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
