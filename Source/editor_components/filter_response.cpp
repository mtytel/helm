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
#include "utils.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define MAG_TO_DB_CONSTANT 20.0f
#define MIN_GAIN_DB -30.0f
#define MAX_GAIN_DB 24.0f
#define MIN_RESONANCE 0.5
#define MAX_RESONANCE 16.0
//[/MiscUserDefs]

//==============================================================================
FilterResponse::FilterResponse (int resolution)
{

    //[UserPreSize]
    resolution_ = resolution;
    frequency_control_ = nullptr;
    resonance_control_ = nullptr;
    filter_type_control_ = nullptr;
    resonance_skew_ = 1.0f;

    filter_.setSampleRate(44100);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    resetResponsePath();
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
    static const PathStrokeType stroke(2.0f, PathStrokeType::beveled, PathStrokeType::rounded);
    //[/UserPrePaint]

    g.fillAll (Colour (0xff303030));

    //[UserPaint] Add your own custom painting code here..
    g.setGradientFill(ColourGradient(Colour(0xff888888), 0.0f, 0.0f,
                                     Colour(0x00000000), 0.0f, getHeight(), false));
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
    resetResponsePath();
    //[/UserResized]
}

void FilterResponse::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    if (e.mods.isRightButtonDown() && filter_type_control_) {
        filter_type_control_->increment(true);
        mopo::Filter::Type type =
            static_cast<mopo::Filter::Type>(filter_type_control_->current_value());

        if (resonance_control_) {
            if (type == mopo::Filter::kLowShelf || type == mopo::Filter::kHighShelf) {
                resonance_control_->setMin(mopo::utils::dbToGain(MIN_GAIN_DB));
                resonance_control_->setMax(mopo::utils::dbToGain(MAX_GAIN_DB));
            }
            else {
                resonance_control_->setMin(MIN_RESONANCE);
                resonance_control_->setMax(MAX_RESONANCE);
            }
        }
        
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
    static const float center = 0.5f;
    static const int wrap_size = 10;

    filter_response_path_.clear();
    filter_response_path_.startNewSubPath(-wrap_size, getHeight() + wrap_size);
    filter_response_path_.lineTo(-wrap_size, proportionOfHeight(1.0f - center));

    for (int i = 0; i < resolution_; ++i) {
        float t = (1.0f * i) / (resolution_ - 1);
        float midi = t * mopo::MAX_CENTS;
        if (frequency_control_)
            midi = INTERPOLATE(frequency_control_->min(), frequency_control_->max(), t);

        float frequency = mopo::MidiLookup::centsLookup(midi * mopo::CENTS_PER_NOTE);
        float response = fabs(filter_.getAmplitudeResponse(frequency));
        float gain_db = MAG_TO_DB_CONSTANT * log10(response);
        float percent = (gain_db - MIN_GAIN_DB) / (MAX_GAIN_DB - MIN_GAIN_DB);

        filter_response_path_.lineTo(getWidth() * t, getHeight() * (1.0f - percent));
    }

    filter_response_path_.lineTo(getWidth() + wrap_size, proportionOfHeight(1.0f - center));
    filter_response_path_.lineTo(getWidth() + wrap_size, getHeight() + wrap_size);
}

void FilterResponse::computeFilterCoefficients() {
    mopo::Filter::Type filter_type = mopo::Filter::kAllPass;
    float frequency = 0.0f;
    float resonance = 1.0f;

    if (frequency_control_) {
        float cents = frequency_control_->current_value() * mopo::CENTS_PER_NOTE;
        frequency = mopo::MidiLookup::centsLookup(cents);
    }
    if (resonance_control_)
        resonance = resonance_control_->current_value();

    if (filter_type_control_)
        filter_type = static_cast<mopo::Filter::Type>(filter_type_control_->current_value());

    filter_.computeCoefficients(filter_type, frequency, resonance);
    resetResponsePath();
}

void FilterResponse::setFilterSettingsFromPosition(Point<int> position) {
    mopo::Filter::Type type = mopo::Filter::kAllPass;
    if (filter_type_control_)
        type = static_cast<mopo::Filter::Type>(filter_type_control_->current_value());

    if (frequency_control_) {
        float percent = (1.0f * position.x) / getWidth();
        frequency_control_->setPercentage(percent);
    }
    if (resonance_control_) {
        float percent = CLAMP(1.0f - (1.0f * position.y) / getHeight(), 0.0f, 1.0f);

        if (type == mopo::Filter::kLowShelf || type == mopo::Filter::kHighShelf) {
            float decibals = INTERPOLATE(MIN_GAIN_DB, MAX_GAIN_DB, percent);
            resonance_control_->set(mopo::utils::dbToGain(decibals));
        }
        else
            resonance_control_->setPercentage(powf(percent, resonance_skew_));
    }

    computeFilterCoefficients();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FilterResponse" componentName=""
                 parentClasses="public Component" constructorParams="int resolution"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff303030"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
