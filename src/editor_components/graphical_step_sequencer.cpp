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
#include "synth_gui_interface.h"
//[/Headers]

#include "graphical_step_sequencer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define FRAMES_PER_SECOND 30
//[/MiscUserDefs]

//==============================================================================
GraphicalStepSequencer::GraphicalStepSequencer ()
{

    //[UserPreSize]
    num_steps_slider_ = nullptr;
    step_generator_output_ = nullptr;
    last_step_ = -1;
    highlighted_step_ = -1;
    num_steps_ = 1;
    setOpaque(true);
    setFramesPerSecond(FRAMES_PER_SECOND);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GraphicalStepSequencer::~GraphicalStepSequencer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GraphicalStepSequencer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    if (sequence_.size() == 0 || num_steps_slider_ == nullptr)
        return;
    //[/UserPrePaint]

    g.fillAll (Colour (0xff246370));

    //[UserPaint] Add your own custom painting code here..
    float x_inc = getWidth() / (1.0f * num_steps_);
    g.setColour(Colour(0xff347380));
    for (int i = 1; i * x_inc < getWidth(); ++i)
        g.drawLine(i * x_inc, 0, i * x_inc, getHeight());

    float x = 0.0f;
    for (int i = 0; i < num_steps_; ++i) {
        float val = sequence_[i]->getValue();
        float bar_position = proportionOfHeight((1.0f - val) / 2.0f);
        if (val >= 0) {
            g.setGradientFill(ColourGradient(Colour(0xffffffff), 0.0f, 0.0,
                                             Colour(0x55347380), 0.0f, getHeight() / 2.0f, false));
            g.fillRect(x, bar_position, x_inc, proportionOfHeight(0.5f) - bar_position);
        }
        else {
            float half_height = proportionOfHeight(0.5f);
            g.setGradientFill(ColourGradient(Colour(0xffffffff), 0.0f, getHeight(),
                                             Colour(0xff347380), 0.0f, half_height, false));
            g.fillRect(x, half_height, x_inc, bar_position - half_height);
        }

        g.setColour(Colour(0xffcccccc));
        g.fillRect(x, bar_position - 1.0f, x_inc, 2.0f);

        if (highlighted_step_ == i) {
            g.setColour(Colour(0x22ffffff));
            g.fillRect(x, 0.0f, x_inc, 1.0f * getHeight());
        }

        if (last_step_ == i) {
            g.setColour(Colour(0x22ffffff));
            g.fillRect(x, 0.0f, x_inc, 1.0f * getHeight());
        }

        x += x_inc;
    }
    //[/UserPaint]
}

void GraphicalStepSequencer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    ensureMinSize();
    //[/UserResized]
}

void GraphicalStepSequencer::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...
    updateHover(getHoveredStep(e.getPosition()));
    //[/UserCode_mouseMove]
}

void GraphicalStepSequencer::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    updateHover(-1);
    //[/UserCode_mouseExit]
}

void GraphicalStepSequencer::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    last_edit_position_ = e.getPosition();
    updateHover(getHoveredStep(e.getPosition()));
    changeStep(e);
    //[/UserCode_mouseDown]
}

void GraphicalStepSequencer::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    updateHover(getHoveredStep(e.getPosition()));
    changeStep(e);
    last_edit_position_ = e.getPosition();
    //[/UserCode_mouseDrag]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GraphicalStepSequencer::update() {
    if (step_generator_output_) {
        int new_step = step_generator_output_->buffer[0];
        if (new_step != last_step_) {
            last_step_ = new_step;
            repaint();
        }
    }
}

void GraphicalStepSequencer::setStepSliders(std::vector<Slider*> sliders) {
    sequence_ = sliders;
    for (int i = 0; i < sliders.size(); ++i)
        sequence_[i]->addListener(this);
    repaint();
}

void GraphicalStepSequencer::sliderValueChanged(Slider* moved_slider) {
    ensureMinSize();
    repaint();
}

void GraphicalStepSequencer::setNumStepsSlider(Slider* num_steps_slider) {
    if (num_steps_slider_)
        num_steps_slider_->removeListener(this);
    num_steps_slider_ = num_steps_slider;
    num_steps_slider_->addListener(this);

    ensureMinSize();
    repaint();
}

void GraphicalStepSequencer::ensureMinSize() {
    if (num_steps_slider_ == nullptr)
        return;

    num_steps_ = num_steps_slider_->getValue();
}

// Sets the height of the steps based on mouse positions.
// If the mouse skipped over some steps between last mouseDrag event, we'll interpolate
// and set all the steps inbetween to appropriate values.
void GraphicalStepSequencer::changeStep(const MouseEvent& e) {
    Point<int> mouse_position = e.getPosition();
    int from_step = getHoveredStep(last_edit_position_);
    int selected_step = getHoveredStep(mouse_position);

    float x = mouse_position.x;
    float y = mouse_position.y;
    float x_delta = last_edit_position_.x - x;
    float y_delta = last_edit_position_.y - y;
    float slope = y_delta == 0 ? 0 : y_delta / x_delta;
    float next_x = getWidth() * (1.0f * selected_step) / num_steps_;
    int direction = -1;

    if (selected_step < from_step) {
        direction = 1;
        next_x += getWidth() * 1.0f / num_steps_;
    }
    float inc_x = next_x - x;

    for (int step = selected_step; step != from_step + direction; step += direction) {
        if (step >= 0 && step < num_steps_) {
            float new_value = -2.0f * y / getHeight() + 1.0f;
            sequence_[step]->setValue(std::max(std::min(new_value, 1.0f), -1.0f));
        }
        y += inc_x * slope;
        inc_x = direction * getWidth() * 1.0f / num_steps_;
    }
    repaint();
}

int GraphicalStepSequencer::getHoveredStep(Point<int> position) {
    return floorf(num_steps_ * (1.0f * position.x) / getWidth());
}

void GraphicalStepSequencer::updateHover(int step_index) {
    if (step_index == highlighted_step_)
        return;
    highlighted_step_ = step_index;
    repaint();
}


void GraphicalStepSequencer::showRealtimeFeedback() {
    if (step_generator_output_ == nullptr) {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        if (parent)
            step_generator_output_ = parent->getModSource(getName().toStdString());
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GraphicalStepSequencer" componentName=""
                 parentClasses="public AnimatedAppComponent, public SliderListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <METHODS>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff246370"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
