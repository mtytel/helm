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
#include "twytch_engine.h"
//[/Headers]

#include "full_interface.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FullInterface::FullInterface (mopo::control_map controls)
{
    addAndMakeVisible (synthesis_interface_ = new SynthesisInterface (controls));
    addAndMakeVisible (save_button_ = new TextButton ("save"));
    save_button_->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    save_button_->addListener (this);

    addAndMakeVisible (load_button_ = new TextButton ("load"));
    load_button_->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    load_button_->addListener (this);

    addAndMakeVisible (arp_frequency_ = new Slider ("arp frequency"));
    arp_frequency_->setRange (1, 20, 0);
    arp_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_frequency_->addListener (this);
    arp_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (arp_gate_ = new Slider ("arp gate"));
    arp_gate_->setRange (0, 1, 0);
    arp_gate_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_gate_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_gate_->addListener (this);

    addAndMakeVisible (arp_octaves_ = new Slider ("arp octaves"));
    arp_octaves_->setRange (1, 4, 1);
    arp_octaves_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_octaves_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_octaves_->addListener (this);

    addAndMakeVisible (arp_pattern_ = new Slider ("arp pattern"));
    arp_pattern_->setRange (0, 4, 1);
    arp_pattern_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_pattern_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_pattern_->addListener (this);

    addAndMakeVisible (arp_on_ = new ToggleButton ("arp on"));
    arp_on_->setButtonText (TRANS("arp"));
    arp_on_->addListener (this);
    arp_on_->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (oscilloscope_ = new Oscilloscope (512));

    //[UserPreSize]
    for (int i = 0; i < getNumChildComponents(); ++i) {
        Slider* slider = dynamic_cast<Slider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;
    }
    addControls(controls);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

FullInterface::~FullInterface()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    synthesis_interface_ = nullptr;
    save_button_ = nullptr;
    load_button_ = nullptr;
    arp_frequency_ = nullptr;
    arp_gate_ = nullptr;
    arp_octaves_ = nullptr;
    arp_pattern_ = nullptr;
    arp_on_ = nullptr;
    oscilloscope_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FullInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FullInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    synthesis_interface_->setBounds (0, 40, proportionOfWidth (1.0000f), getHeight() - 113);
    save_button_->setBounds (192, 8, 150, 24);
    load_button_->setBounds (24, 8, 150, 24);
    arp_frequency_->setBounds (176, 728, 50, 50);
    arp_gate_->setBounds (112, 728, 50, 50);
    arp_octaves_->setBounds (240, 728, 50, 50);
    arp_pattern_->setBounds (304, 728, 50, 50);
    arp_on_->setBounds (48, 744, 48, 24);
    oscilloscope_->setBounds (600, 664, 272, getHeight() - 681);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FullInterface::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    std::string name = buttonThatWasClicked->getName().toStdString();
    if (controls_.count(name)) {
        int val = buttonThatWasClicked->getToggleState() ? 1 : 0;
        controls_[name]->set(val);
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == save_button_)
    {
        //[UserButtonCode_save_button_] -- add your button handler code here..
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;
        FileBrowserComponent browser(flags, File::nonexistent, nullptr, nullptr);
        FileChooserDialogBox save_dialog("save patch", "save", browser, true, Colours::white);
        if (save_dialog.show()) {
            File save_file = browser.getSelectedFile(0);
            save_file.replaceWithText(JSON::toString(getState()));
        }
        //[/UserButtonCode_save_button_]
    }
    else if (buttonThatWasClicked == load_button_)
    {
        //[UserButtonCode_load_button_] -- add your button handler code here..
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::openMode;
        FileBrowserComponent browser(flags, File::nonexistent, nullptr, nullptr);
        FileChooserDialogBox load_dialog("load patch", "load", browser, true, Colours::white);
        if (load_dialog.show()) {
            File load_file = browser.getSelectedFile(0);

            var parsed_json_state;
            if (JSON::parse(load_file.loadFileAsString(), parsed_json_state).wasOk())
                writeState(parsed_json_state);
        }
        //[/UserButtonCode_load_button_]
    }
    else if (buttonThatWasClicked == arp_on_)
    {
        //[UserButtonCode_arp_on_] -- add your button handler code here..

        //[/UserButtonCode_arp_on_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void FullInterface::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    std::string name = sliderThatWasMoved->getName().toStdString();
    if (controls_.count(name))
        controls_[name]->set(sliderThatWasMoved->getValue());
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == arp_frequency_)
    {
        //[UserSliderCode_arp_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_arp_frequency_]
    }
    else if (sliderThatWasMoved == arp_gate_)
    {
        //[UserSliderCode_arp_gate_] -- add your slider handling code here..
        //[/UserSliderCode_arp_gate_]
    }
    else if (sliderThatWasMoved == arp_octaves_)
    {
        //[UserSliderCode_arp_octaves_] -- add your slider handling code here..
        //[/UserSliderCode_arp_octaves_]
    }
    else if (sliderThatWasMoved == arp_pattern_)
    {
        //[UserSliderCode_arp_pattern_] -- add your slider handling code here..
        //[/UserSliderCode_arp_pattern_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void FullInterface::addControls(mopo::control_map controls) {
    controls_ = controls;

    std::map<std::string, Slider*>::iterator iter = slider_lookup_.begin();
    for (; iter != slider_lookup_.end(); ++iter)
        iter->second->setValue(controls_[iter->first]->value(),
                               NotificationType::dontSendNotification);
}

void FullInterface::setOutputMemory(const mopo::Memory *output_memory) {
    oscilloscope_->setOutputMemory(output_memory);
}

void FullInterface::setSynth(mopo::TwytchEngine* synth) {
    synthesis_interface_->setSynth(synth);
}

var FullInterface::getState() {
    DynamicObject* state_object = new DynamicObject();
    state_object->setProperty("synthesis", synthesis_interface_->getState());

    std::map<std::string, Slider*>::iterator iter = slider_lookup_.begin();
    for (; iter != slider_lookup_.end(); ++iter)
        state_object->setProperty(String(iter->first), iter->second->getValue());

    return state_object;
}

void FullInterface::writeState(var state) {
    DynamicObject* object_state = state.getDynamicObject();
    synthesis_interface_->writeState(object_state->getProperty("synthesis"));

    NamedValueSet properties = object_state->getProperties();
    int size = properties.size();
    for (int i = 0; i < size; ++i) {
        Identifier id = properties.getName(i);
        if (id.isValid()) {
            String name = id.toString();
            if (slider_lookup_.count(name.toStdString())) {
                mopo::mopo_float value = properties.getValueAt(i);
                slider_lookup_[name.toStdString()]->setValue(value);
            }
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

<JUCER_COMPONENT documentType="Component" className="FullInterface" componentName=""
                 parentClasses="public Component, public DragAndDropContainer"
                 constructorParams="mopo::control_map controls" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff000000"/>
  <JUCERCOMP name="" id="2ef5006082722165" memberName="synthesis_interface_"
             virtualName="" explicitFocusOrder="0" pos="0 40 100% 113M" sourceFile="synthesis_interface.cpp"
             constructorParams="controls"/>
  <TEXTBUTTON name="save" id="80d4648667c9cf51" memberName="save_button_" virtualName=""
              explicitFocusOrder="0" pos="192 8 150 24" buttonText="save" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="load" id="41af69dad8b4335d" memberName="load_button_" virtualName=""
              explicitFocusOrder="0" pos="24 8 150 24" buttonText="load" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="arp frequency" id="90264eb571112e1b" memberName="arp_frequency_"
          virtualName="" explicitFocusOrder="0" pos="176 728 50 50" min="1"
          max="20" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="arp gate" id="e8f61b752c6d561e" memberName="arp_gate_"
          virtualName="" explicitFocusOrder="0" pos="112 728 50 50" min="0"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp octaves" id="858d1f30bb7ddacd" memberName="arp_octaves_"
          virtualName="" explicitFocusOrder="0" pos="240 728 50 50" min="1"
          max="4" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp pattern" id="92ea11d0205b2100" memberName="arp_pattern_"
          virtualName="" explicitFocusOrder="0" pos="304 728 50 50" min="0"
          max="4" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="arp on" id="5425f3b11382569d" memberName="arp_on_" virtualName=""
                explicitFocusOrder="0" pos="48 744 48 24" txtcol="ffffffff" buttonText="arp"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <JUCERCOMP name="oscilloscope" id="341088b80b59e875" memberName="oscilloscope_"
             virtualName="" explicitFocusOrder="0" pos="600 664 272 681M"
             sourceFile="oscilloscope.cpp" constructorParams="512"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
