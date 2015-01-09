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
    addAndMakeVisible (wave_form_1_ = new WaveFormSelector (128));
    addAndMakeVisible (wave_form_2_ = new WaveFormSelector (128));
    addAndMakeVisible (knob1_ = new Slider ("knob 1"));
    knob1_->setRange (0, 10, 0);
    knob1_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    knob1_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    knob1_->addListener (this);

    addAndMakeVisible (knob2_ = new Slider ("knob 2"));
    knob2_->setRange (0, 10, 0);
    knob2_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    knob2_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    knob2_->addListener (this);

    addAndMakeVisible (knob3_ = new Slider ("knob 3"));
    knob3_->setRange (0, 10, 0);
    knob3_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    knob3_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    knob3_->addListener (this);

    addAndMakeVisible (knob4_ = new Slider ("knob 4"));
    knob4_->setRange (0, 10, 0);
    knob4_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    knob4_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    knob4_->addListener (this);


    //[UserPreSize]
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
    wave_form_1_ = nullptr;
    wave_form_2_ = nullptr;
    knob1_ = nullptr;
    knob2_ = nullptr;
    knob3_ = nullptr;
    knob4_ = nullptr;


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
    wave_form_1_->setBounds (384, 88, 300, 100);
    wave_form_2_->setBounds (384, 224, 300, 100);
    knob1_->setBounds (400, 400, 50, 50);
    knob2_->setBounds (472, 400, 50, 50);
    knob3_->setBounds (544, 400, 50, 50);
    knob4_->setBounds (616, 400, 50, 50);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SynthesisEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == knob1_)
    {
        //[UserSliderCode_knob1_] -- add your slider handling code here..
        //[/UserSliderCode_knob1_]
    }
    else if (sliderThatWasMoved == knob2_)
    {
        //[UserSliderCode_knob2_] -- add your slider handling code here..
        //[/UserSliderCode_knob2_]
    }
    else if (sliderThatWasMoved == knob3_)
    {
        //[UserSliderCode_knob3_] -- add your slider handling code here..
        //[/UserSliderCode_knob3_]
    }
    else if (sliderThatWasMoved == knob4_)
    {
        //[UserSliderCode_knob4_] -- add your slider handling code here..
        //[/UserSliderCode_knob4_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
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

    wave_form_1_->setWaveControl(controls["osc 1 waveform"]);
    wave_form_2_->setWaveControl(controls["osc 2 waveform"]);
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
  <JUCERCOMP name="" id="55100715382ea344" memberName="wave_form_1_" virtualName="WaveFormSelector"
             explicitFocusOrder="0" pos="384 88 300 100" sourceFile="wave_form_selector.cpp"
             constructorParams="128"/>
  <JUCERCOMP name="" id="c0c3e4a3ab2f045f" memberName="wave_form_2_" virtualName="WaveFormSelector"
             explicitFocusOrder="0" pos="384 224 300 100" sourceFile="wave_form_selector.cpp"
             constructorParams="128"/>
  <SLIDER name="knob 1" id="952bde38857bdba7" memberName="knob1_" virtualName=""
          explicitFocusOrder="0" pos="400 400 50 50" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="knob 2" id="9de85cc1c5f64eaa" memberName="knob2_" virtualName=""
          explicitFocusOrder="0" pos="472 400 50 50" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="knob 3" id="e53afc6d1a04c708" memberName="knob3_" virtualName=""
          explicitFocusOrder="0" pos="544 400 50 50" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="knob 4" id="1d3e4b59d6e470fb" memberName="knob4_" virtualName=""
          explicitFocusOrder="0" pos="616 400 50 50" min="0" max="10" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
