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

#include "graphical_envelope.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

namespace {
    const float ATTACK_RANGE_PERCENT = 0.33f;
    const float DECAY_RANGE_PERCENT = 0.33f;
    const float HOVER_DISTANCE = 20.0f;
    const int GRID_CELL_WIDTH = 10;
} // namespace

//[/MiscUserDefs]

//==============================================================================
GraphicalEnvelope::GraphicalEnvelope ()
{

    //[UserPreSize]
    attack_hover_ = false;
    decay_hover_ = false;
    sustain_hover_ = false;
    release_hover_ = false;
    mouse_down_ = false;

    attack_slider_ = nullptr;
    decay_slider_ = nullptr;
    sustain_slider_ = nullptr;
    release_slider_ = nullptr;
    setOpaque(true);
    //[/UserPreSize]

    setSize (200, 100);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GraphicalEnvelope::~GraphicalEnvelope()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GraphicalEnvelope::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    static const PathStrokeType stroke(1.0f, PathStrokeType::beveled, PathStrokeType::rounded);
    //[/UserPrePaint]

    g.fillAll (Colour (0xff33064f));

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff43165f));
    for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
        g.drawLine(x, 0, x, getHeight());
    for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
        g.drawLine(0, y, getWidth(), y);

    g.setColour(Colour(0x18ffffff));
    g.fillRect(0.0f, 0.0f, getAttackX(), 1.0f * getHeight());
    g.setColour(Colour(0x10ffffff));
    g.fillRect(getAttackX(), 0.0f, getDecayX() - getAttackX(), 1.0f * getHeight());
    g.setColour(Colour(0x08ffffff));
    g.fillRect(getDecayX(), 0.0f, getReleaseX() - getDecayX(), 1.0f * getHeight());

    g.setGradientFill(ColourGradient(Colour(0xffeca769), 0.0f, getHeight() * 0.1f,
                                     Colour(0x11cb587a), 0.0f, getHeight(), false));
    g.fillPath(envelope_line_);

    g.setColour(Colour(0xffcccccc));
    g.strokePath(envelope_line_, stroke);


    float hover_line_x = -20;
    if (attack_hover_)
        hover_line_x = getAttackX();
    else if (decay_hover_)
        hover_line_x = getDecayX();
    else if (release_hover_)
        hover_line_x = getReleaseX();

    if (mouse_down_) {
        g.setColour(Colour(0x11ffffff));
        g.fillRect(hover_line_x - 10.0f, 0.0f, 20.0f, 1.0f * getHeight());
    }

    g.setColour(Colour(0x66ffffff));
    g.fillRect(hover_line_x - 0.5f, 0.0f, 1.0f, 1.0f * getHeight());

    if (sustain_hover_) {
        if (mouse_down_) {
            g.setColour(Colour(0x11ffffff));
            g.fillEllipse(getDecayX() - 20.0, getSustainY() - 20.0, 40.0, 40.0);
        }

        g.setColour(Colour(0x66ffffff));
        g.drawEllipse(getDecayX() - 4.0, getSustainY() - 4.0, 8.0, 8.0, 1.0);
    }

    g.setColour(Colour(0xffcccccc));
    g.fillEllipse(getDecayX() - 2.0, getSustainY() - 2.0, 4.0, 4.0);
    //[/UserPaint]
}

void GraphicalEnvelope::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    resetEnvelopeLine();
    //[/UserResized]
}

void GraphicalEnvelope::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...
    float x = e.getPosition().x;
    float attack_delta = fabs(x - getAttackX());
    float decay_delta = fabs(x - getDecayX());
    float release_delta = fabs(x - getReleaseX());
    float sustain_delta = fabs(e.getPosition().y - getSustainY());

    bool a_hover = attack_delta < decay_delta && attack_delta < HOVER_DISTANCE;
    bool d_hover = !attack_hover_ && decay_delta < release_delta && decay_delta < HOVER_DISTANCE;
    bool r_hover = !decay_hover_ && release_delta < HOVER_DISTANCE;
    bool s_hover = !a_hover && !r_hover && x > getDecayX() - HOVER_DISTANCE &&
                   x < getDecayX() + HOVER_DISTANCE && sustain_delta < HOVER_DISTANCE;

    if (a_hover != attack_hover_ || d_hover != decay_hover_ ||
        s_hover != sustain_hover_ || r_hover != release_hover_) {
        attack_hover_ = a_hover;
        decay_hover_ = d_hover;
        sustain_hover_ = s_hover;
        release_hover_ = r_hover;
        repaint();
    }
    //[/UserCode_mouseMove]
}

void GraphicalEnvelope::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    attack_hover_ = false;
    decay_hover_ = false;
    sustain_hover_ = false;
    release_hover_ = false;
    repaint();
    //[/UserCode_mouseExit]
}

void GraphicalEnvelope::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    mouse_down_ = true;
    repaint();
    //[/UserCode_mouseDown]
}

void GraphicalEnvelope::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    if (attack_hover_)
        setAttackX(e.getPosition().x);
    else if (decay_hover_)
        setDecayX(e.getPosition().x);
    else if (release_hover_)
        setReleaseX(e.getPosition().x);

    if (sustain_hover_)
        setSustainY(e.getPosition().y);

    if (attack_hover_ || decay_hover_ || sustain_hover_ || release_hover_) {
        resetEnvelopeLine();
        repaint();
    }
    //[/UserCode_mouseDrag]
}

void GraphicalEnvelope::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    mouse_down_ = false;
    repaint();
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GraphicalEnvelope::sliderValueChanged(Slider* sliderThatWasMoved) {
    resetEnvelopeLine();
    repaint();
}

float GraphicalEnvelope::getAttackX() {
    if (!attack_slider_)
        return 0.0;

    double percent = attack_slider_->valueToProportionOfLength(attack_slider_->getValue());
    return getWidth() * percent * ATTACK_RANGE_PERCENT;
}

float GraphicalEnvelope::getDecayX() {
    if (!decay_slider_)
        return 0.0;

    double percent = decay_slider_->valueToProportionOfLength(decay_slider_->getValue());
    return getAttackX() + getWidth() * percent * DECAY_RANGE_PERCENT;
}

float GraphicalEnvelope::getSustainY() {
    if (!sustain_slider_)
        return 0.0;

    double percent = sustain_slider_->valueToProportionOfLength(sustain_slider_->getValue());
    return getHeight() * (1.0 - percent);
}

float GraphicalEnvelope::getReleaseX() {
    if (!release_slider_)
        return 0.0;

    double percent = release_slider_->valueToProportionOfLength(release_slider_->getValue());
    return getDecayX() + getWidth() * percent * (1.0 - DECAY_RANGE_PERCENT - ATTACK_RANGE_PERCENT);
}

void GraphicalEnvelope::setAttackX(double x) {
    if (!attack_slider_)
        return;

    double percent = x / (getWidth() * ATTACK_RANGE_PERCENT);
    attack_slider_->setValue(attack_slider_->proportionOfLengthToValue(percent));
}

void GraphicalEnvelope::setDecayX(double x) {
    if (!decay_slider_)
        return;

    double percent = (x - getAttackX()) / (getWidth() * DECAY_RANGE_PERCENT);
    decay_slider_->setValue(decay_slider_->proportionOfLengthToValue(percent));
}

void GraphicalEnvelope::setSustainY(double y) {
    if (!sustain_slider_)
        return;

    sustain_slider_->setValue(sustain_slider_->proportionOfLengthToValue(1.0 - y / getHeight()));
}

void GraphicalEnvelope::setReleaseX(double x) {
    if (!release_slider_)
        return;

    double percent = (x - getDecayX()) /
                     (getWidth() * (1.0 - DECAY_RANGE_PERCENT - ATTACK_RANGE_PERCENT));
    release_slider_->setValue(release_slider_->proportionOfLengthToValue(percent));
}

void GraphicalEnvelope::setAttackSlider(Slider* attack_slider) {
    if (attack_slider_)
        attack_slider_->removeListener(this);
    attack_slider_ = attack_slider;
    attack_slider_->addListener(this);
    resetEnvelopeLine();
    repaint();
}

void GraphicalEnvelope::setDecaySlider(Slider* decay_slider) {
    if (decay_slider_)
        decay_slider_->removeListener(this);
    decay_slider_ = decay_slider;
    decay_slider_->addListener(this);
    resetEnvelopeLine();
    repaint();
}

void GraphicalEnvelope::setSustainSlider(Slider* sustain_slider) {
    if (sustain_slider_)
        sustain_slider_->removeListener(this);
    sustain_slider_ = sustain_slider;
    sustain_slider_->addListener(this);
    resetEnvelopeLine();
    repaint();
}

void GraphicalEnvelope::setReleaseSlider(Slider* release_slider) {
    if (release_slider_)
        release_slider_->removeListener(this);
    release_slider_ = release_slider;
    release_slider_->addListener(this);
    resetEnvelopeLine();
    repaint();
}

void GraphicalEnvelope::resetEnvelopeLine() {
    envelope_line_.clear();
    envelope_line_.startNewSubPath(0, getHeight());
    envelope_line_.lineTo(getAttackX(), 0.0f);
    envelope_line_.quadraticTo(0.5f * (getAttackX() + getDecayX()), getSustainY(),
                               getDecayX(), getSustainY());

    envelope_line_.quadraticTo(0.5f * (getReleaseX() + getDecayX()), getHeight(),
                               getReleaseX(), getHeight());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GraphicalEnvelope" componentName=""
                 parentClasses="public Component, public SliderListener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="200" initialHeight="100">
  <METHODS>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff33064f"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
