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
    const float ATTACK_RANGE_PERCENT = 0.3f;
    const float DECAY_RANGE_PERCENT = 0.3f;
    const float RELEASE_RANGE_PERCENT = 0.3f;

    void setSkewedValue(mopo::Control* control, double t, double skew) {
        double skewed_t = pow(t, skew);
        control->set(control->min() + skewed_t * (control->max() - control->min()));
    }

    double getPercentFromControl(mopo::Control* control, double skew) {
        double t = (control->value()->value() - control->min()) / (control->max() - control->min());
        return pow(t, 1.0 / skew);
    }

} // namespace

//[/MiscUserDefs]

//==============================================================================
GraphicalEnvelope::GraphicalEnvelope ()
{

    //[UserPreSize]
    attack_percent_ = 0.5f;
    decay_percent_ = 0.5f;
    sustain_percent_ = 0.5f;
    release_percent_ = 0.5f;

    time_skew_factor_ = 1.0;
    
    attack_control_ = nullptr;
    decay_control_ = nullptr;
    sustain_control_ = nullptr;
    release_control_ = nullptr;
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
    //[/UserPrePaint]

    g.fillAll (Colour (0xff303030));

    //[UserPaint] Add your own custom painting code here..
    g.setGradientFill(ColourGradient(Colour(0xff888888), 0.0f, 0.0f,
                                     Colour(0x00000000), 0.0f, getHeight(), false));
    g.fillPath(envelope_line_);

    g.setColour(Colour(0xffcccccc));
    g.strokePath(envelope_line_, PathStrokeType(2.000f, PathStrokeType::beveled, PathStrokeType::rounded));

    g.setColour(Colour(0xffc8c8c8));
    g.drawRoundedRectangle(-2.0f, getHeight() - 2.0f, 4.0f, 4.0f, 2.0f, 4.0f);
    //[/UserPaint]
}

void GraphicalEnvelope::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    /*
    attack_->setMovementRestrictions(0, 0, proportionOfWidth(ATTACK_RANGE_PERCENT), 0);

    decay_->setMovementRestrictions(proportionOfWidth(ATTACK_RANGE_PERCENT), 0,
                                    proportionOfWidth(DECAY_RANGE_PERCENT), 0);

    sustain_->setMovementRestrictions(proportionOfWidth(1.0f - RELEASE_RANGE_PERCENT), 0,
                                      0, getHeight());

    release_->setMovementRestrictions(proportionOfWidth(1.0f - RELEASE_RANGE_PERCENT),
                                      getHeight(),
                                      proportionOfWidth(RELEASE_RANGE_PERCENT), 0);
     */
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    setPositionsFromValues();
    resetEnvelopeLine();
    //[/UserResized]
}

void GraphicalEnvelope::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...
    //[/UserCode_mouseMove]
}

void GraphicalEnvelope::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    //[/UserCode_mouseExit]
}

void GraphicalEnvelope::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    //[/UserCode_mouseDown]
}

void GraphicalEnvelope::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    //[/UserCode_mouseDrag]
}

void GraphicalEnvelope::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GraphicalEnvelope::childBoundsChanged(Component *child) {
    resetEnvelopeLine();
    /*
    if (child == attack_) {
        int delta = attack_->getX() - last_attack_x_;
        Rectangle<int> decay_restrictions = decay_->getMovementRestrictions();
        decay_restrictions.setX(attack_->getBounds().getCentre().x);
        decay_->setMovementRestrictions(decay_restrictions);

        decay_->setTopLeftPosition(decay_->getX() + delta, decay_->getY());
        last_attack_x_ = attack_->getX();
        if (attack_control_) {
            double x_pos = attack_->getBounds().getCentre().x;
            double t = x_pos / proportionOfWidth(ATTACK_RANGE_PERCENT);
            setSkewedValue(attack_control_, t, time_skew_factor_);
        }
    }
    else if (child == decay_) {
        if (decay_control_) {
            double x_pos = decay_->getX() - attack_->getX();
            double t = x_pos / proportionOfWidth(DECAY_RANGE_PERCENT);
            setSkewedValue(decay_control_, t, time_skew_factor_);
        }
    }
    else if (child == sustain_) {
        Rectangle<int> decay_restrictions = decay_->getMovementRestrictions();
        decay_restrictions.setHeight(0);
        decay_restrictions.setY(sustain_->getBounds().getCentre().y);
        decay_->setMovementRestrictions(decay_restrictions);

        decay_->setTopLeftPosition(decay_->getX(), sustain_->getY());

        if (sustain_control_) {
            double y_pos = getHeight() - sustain_->getBounds().getCentre().y;
            sustain_control_->set(y_pos / getHeight());
        }
    }
    else if (child == release_) {
        if (release_control_) {
            double x_pos = release_->getX() - sustain_->getX();
            double t = x_pos / proportionOfWidth(RELEASE_RANGE_PERCENT);
            setSkewedValue(release_control_, t, time_skew_factor_);
        }
    }
    repaint();
     */
}

void GraphicalEnvelope::setPositionsFromValues() {
    // This order is important because attack and sustain modify decay position.
    /*
    if (decay_control_) {
        double decay_percent = getPercentFromControl(decay_control_, time_skew_factor_);
        double decay_x = attack_->getBounds().getCentre().x +
        proportionOfWidth(DECAY_RANGE_PERCENT * decay_percent);
        decay_->setCentrePosition(decay_x, decay_->getBounds().getCentre().y);
    }
    if (attack_control_) {
        double attack_percent = getPercentFromControl(attack_control_, time_skew_factor_);
        double attack_x = proportionOfWidth(ATTACK_RANGE_PERCENT * attack_percent);
        attack_->setCentrePosition(attack_x, attack_->getBounds().getCentre().y);
    }
    if (sustain_control_) {
        double sustain_percent = getPercentFromControl(sustain_control_, 1.0);
        double sustain_x = sustain_->getBounds().getCentre().x;
        sustain_->setCentrePosition(sustain_x, proportionOfHeight(1.0 - sustain_percent));
    }
    if (release_control_) {
        double release_percent = getPercentFromControl(release_control_, time_skew_factor_);
        double release_x = sustain_->getBounds().getCentre().x +
                           proportionOfWidth(RELEASE_RANGE_PERCENT * release_percent);
        release_->setCentrePosition(release_x, release_->getBounds().getCentre().y);
    }
     */
}

void GraphicalEnvelope::resetEnvelopeLine() {
    float attack_x = proportionOfWidth(attack_percent_ * ATTACK_RANGE_PERCENT);
    float decay_x = attack_x + proportionOfWidth(decay_percent_ * DECAY_RANGE_PERCENT);
    float sustain_x = proportionOfWidth(1.0f - RELEASE_RANGE_PERCENT);
    float sustain_height = proportionOfHeight(sustain_percent_);
    float release_x = sustain_x + proportionOfWidth(release_percent_ * RELEASE_RANGE_PERCENT);

    envelope_line_.clear();
    envelope_line_.startNewSubPath(0, getHeight());
    envelope_line_.lineTo(attack_x, 0.0f);
    envelope_line_.lineTo(decay_x, sustain_height);
    envelope_line_.lineTo(sustain_x, sustain_height);
    envelope_line_.quadraticTo((release_x + sustain_x) / 2.0f, getHeight(), release_x, getHeight());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GraphicalEnvelope" componentName=""
                 parentClasses="public Component, public DragAndDropContainer"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="200"
                 initialHeight="100">
  <METHODS>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff303030"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
