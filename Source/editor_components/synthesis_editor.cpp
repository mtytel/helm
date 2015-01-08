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

#include "synthesis_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

namespace {
    const double TIME_SKEW_FACTOR = 2.0;
}

//[/MiscUserDefs]

//==============================================================================
SynthesisEditor::SynthesisEditor ()
{
    addAndMakeVisible (step_sequencer_ = new GraphicalStepSequencer (16));
    addAndMakeVisible (amplitude_envelope_ = new GraphicalEnvelope());
    addAndMakeVisible (filter_envelope_ = new GraphicalEnvelope());

    //[UserPreSize]
    addAndMakeVisible(knob_ = new Slider("knob"));
    knob_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    knob_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    amplitude_envelope_->setTimeSkewFactor(TIME_SKEW_FACTOR);
    filter_envelope_->setTimeSkewFactor(TIME_SKEW_FACTOR);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SynthesisEditor::~SynthesisEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    step_sequencer_ = nullptr;
    amplitude_envelope_ = nullptr;
    filter_envelope_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SynthesisEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff1f1f1f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SynthesisEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    step_sequencer_->setBounds (56, 400, 300, 100);
    amplitude_envelope_->setBounds (56, 88, 300, 100);
    filter_envelope_->setBounds (56, 224, 300, 100);
    //[UserResized] Add your own custom resize handling here..
    knob_->setBounds(400, 100, 50, 50);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SynthesisEditor::addControls(mopo::control_map controls) {
    amplitude_envelope_->setAttackControl(controls["amp attack"]);
    amplitude_envelope_->setDecayControl(controls["amp decay"]);
    amplitude_envelope_->setSustainControl(controls["amp sustain"]);
    amplitude_envelope_->setReleaseControl(controls["amp release"]);

    filter_envelope_->setAttackControl(controls["fil attack"]);
    filter_envelope_->setDecayControl(controls["fil decay"]);
    filter_envelope_->setSustainControl(controls["fil sustain"]);
    filter_envelope_->setReleaseControl(controls["fil release"]);

    amplitude_envelope_->setPositionsFromValues();
    filter_envelope_->setPositionsFromValues();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SynthesisEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff1f1f1f"/>
  <JUCERCOMP name="" id="83a23936a8f464b5" memberName="step_sequencer_" virtualName="GraphicalStepSequencer"
             explicitFocusOrder="0" pos="56 400 300 100" sourceFile="graphical_step_sequencer.cpp"
             constructorParams="16"/>
  <JUCERCOMP name="" id="b4880edb8b39ec9d" memberName="amplitude_envelope_"
             virtualName="GraphicalEnvelope" explicitFocusOrder="0" pos="56 88 300 100"
             sourceFile="graphical_envelope.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="87feb60c88df4fcc" memberName="filter_envelope_" virtualName="GraphicalEnvelope"
             explicitFocusOrder="0" pos="56 224 300 100" sourceFile="graphical_envelope.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
