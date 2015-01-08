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
    addAndMakeVisible (attack_ = new DraggableComponent());
    attack_->setExplicitFocusOrder (1);
    addAndMakeVisible (decay_ = new DraggableComponent());
    decay_->setExplicitFocusOrder (1);
    addAndMakeVisible (sustain_ = new DraggableComponent());
    sustain_->setExplicitFocusOrder (1);
    addAndMakeVisible (release_ = new DraggableComponent());
    release_->setExplicitFocusOrder (1);

    //[UserPreSize]
    time_skew_factor_ = 1.0;
    hovered_over_ = nullptr;
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

    attack_ = nullptr;
    decay_ = nullptr;
    sustain_ = nullptr;
    release_ = nullptr;


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
    g.setGradientFill (ColourGradient (Colour (0xff3875ea),
                                       static_cast<float> (proportionOfWidth (0.5000f)), static_cast<float> (proportionOfHeight (0.0000f)),
                                       Colour (0xff1c2b35),
                                       static_cast<float> (proportionOfWidth (0.5000f)), static_cast<float> (proportionOfHeight (1.0000f)),
                                       false));
    g.fillPath(envelope_line_);
    g.setColour(Colour(0xff568596));
    g.setColour (Colour (0xff66bdff));
    g.strokePath(envelope_line_, PathStrokeType(5.000f, PathStrokeType::beveled, PathStrokeType::rounded));

    g.setColour(Colour(0x22ffffff));
    if (hovered_over_ == attack_ || attack_->isHovered())
        g.fillRect(0, 0, attack_->getBounds().getCentre().x, getHeight());
    else if(hovered_over_ == decay_ || decay_->isHovered()) {
        g.fillRect(attack_->getBounds().getCentre().x, 0,
                   decay_->getX() - attack_->getX(), getHeight());
    }
    else if(hovered_over_ == sustain_ || sustain_->isHovered()) {
        g.fillRect(decay_->getBounds().getCentre().x, 0,
                   sustain_->getX() - decay_->getX(), getHeight());
    }
    else if(hovered_over_ == release_ || release_->isHovered()) {
        g.fillRect(sustain_->getBounds().getCentre().x, 0,
                   release_->getX() - sustain_->getX(), getHeight());
    }

    g.setColour (Colour (0xff2a55a5));
    g.drawRoundedRectangle (static_cast<float> (-4), static_cast<float> (getHeight() - 4), 8.0f, 8.0f, 2.000f, 4.000f);
    //[/UserPaint]
}

void GraphicalEnvelope::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    attack_->setMovementRestrictions(0, 0, proportionOfWidth(ATTACK_RANGE_PERCENT), 0);

    decay_->setMovementRestrictions(proportionOfWidth(ATTACK_RANGE_PERCENT), 0,
                                    proportionOfWidth(DECAY_RANGE_PERCENT), 0);

    sustain_->setMovementRestrictions(proportionOfWidth(1.0f - RELEASE_RANGE_PERCENT), 0,
                                      0, getHeight());

    release_->setMovementRestrictions(proportionOfWidth(1.0f - RELEASE_RANGE_PERCENT),
                                      getHeight(),
                                      proportionOfWidth(RELEASE_RANGE_PERCENT), 0);
    //[/UserPreResize]

    attack_->setBounds (proportionOfWidth (0.1004f), 0, 32, 32);
    decay_->setBounds (proportionOfWidth (0.1998f), proportionOfHeight (0.2996f), 32, 32);
    sustain_->setBounds (proportionOfWidth (0.5000f), proportionOfHeight (0.2996f), 32, 32);
    release_->setBounds (proportionOfWidth (0.8996f), 304, 32, 32);
    //[UserResized] Add your own custom resize handling here..
    setPositionsFromValues();
    resetEnvelopeLine();
    last_attack_x_ = attack_->getX();
    //[/UserResized]
}

void GraphicalEnvelope::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...
    DraggableComponent* hovered = hovered_over_;
    hovered_over_ = getHoveredSection(e);
    if (hovered_over_ != hovered)
        repaint();
    //[/UserCode_mouseMove]
}

void GraphicalEnvelope::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    DraggableComponent* hovered = hovered_over_;
    hovered_over_ = nullptr;
    if (hovered)
        repaint();
    //[/UserCode_mouseExit]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GraphicalEnvelope::childBoundsChanged(Component *child) {
    resetEnvelopeLine();
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
}

void GraphicalEnvelope::setPositionsFromValues() {
    // This order is important because attack and sustain modify decay position.
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
}

void GraphicalEnvelope::resetEnvelopeLine() {
    envelope_line_.clear();
    envelope_line_.startNewSubPath(0, getHeight());
    envelope_line_.lineTo(attack_->getBounds().toFloat().getCentre());
    envelope_line_.lineTo(decay_->getBounds().toFloat().getCentre());
    envelope_line_.lineTo(sustain_->getBounds().toFloat().getCentre());
    Point<float> rel_control = release_->getBounds().toFloat().getCentre();
    rel_control.setX((sustain_->getBounds().getCentre().getX() + rel_control.getX()) / 2.0f);
    envelope_line_.quadraticTo(rel_control, release_->getBounds().toFloat().getCentre());
}

DraggableComponent* GraphicalEnvelope::getHoveredSection(const MouseEvent& e) {
    if (e.getPosition().x < attack_->getBounds().getCentre().x)
        return attack_.get();
    if (e.getPosition().x < decay_->getBounds().getCentre().x)
        return decay_.get();
    if (e.getPosition().x < sustain_->getBounds().getCentre().x)
        return sustain_.get();
    if (e.getPosition().x < release_->getBounds().getCentre().x)
        return release_.get();
    return nullptr;
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
  </METHODS>
  <BACKGROUND backgroundColour="ff303030"/>
  <JUCERCOMP name="" id="24607738ba2dcb76" memberName="attack_" virtualName="DraggableComponent"
             explicitFocusOrder="1" pos="10.044% 0 32 32" sourceFile="draggable_component.cpp"
             constructorParams=""/>
  <JUCERCOMP name="" id="a55a7d6beda6b886" memberName="decay_" virtualName="DraggableComponent"
             explicitFocusOrder="1" pos="19.978% 29.963% 32 32" sourceFile="draggable_component.cpp"
             constructorParams=""/>
  <JUCERCOMP name="" id="33c87b37efec2a9f" memberName="sustain_" virtualName="DraggableComponent"
             explicitFocusOrder="1" pos="50% 29.963% 32 32" sourceFile="draggable_component.cpp"
             constructorParams=""/>
  <JUCERCOMP name="" id="81a378f6086fec18" memberName="release_" virtualName="DraggableComponent"
             explicitFocusOrder="1" pos="89.956% 304 32 32" sourceFile="draggable_component.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
