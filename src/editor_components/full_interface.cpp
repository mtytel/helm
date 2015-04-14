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
#include "twytch_common.h"
#include "synth_gui_interface.h"
//[/Headers]

#include "full_interface.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FullInterface::FullInterface (mopo::control_map controls, mopo::output_map modulation_sources, mopo::output_map mono_modulations, mopo::output_map poly_modulations)
{
    addAndMakeVisible (synthesis_interface_ = new SynthesisInterface (controls));
    addAndMakeVisible (arp_frequency_ = new TwytchSlider ("arp_frequency"));
    arp_frequency_->setRange (-1, 4, 0);
    arp_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    arp_frequency_->addListener (this);

    addAndMakeVisible (arp_gate_ = new TwytchSlider ("arp_gate"));
    arp_gate_->setRange (0, 1, 0);
    arp_gate_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_gate_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_gate_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_gate_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    arp_gate_->addListener (this);

    addAndMakeVisible (arp_octaves_ = new TwytchSlider ("arp_octaves"));
    arp_octaves_->setRange (1, 4, 1);
    arp_octaves_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_octaves_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_octaves_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_octaves_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    arp_octaves_->addListener (this);

    addAndMakeVisible (arp_pattern_ = new TwytchSlider ("arp_pattern"));
    arp_pattern_->setRange (0, 4, 1);
    arp_pattern_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_pattern_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_pattern_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_pattern_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    arp_pattern_->addListener (this);

    addAndMakeVisible (oscilloscope_ = new Oscilloscope (512));
    addAndMakeVisible (load_button_ = new TextButton ("load"));
    load_button_->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    load_button_->addListener (this);

    addAndMakeVisible (arp_on_ = new ToggleButton ("arp_on"));
    arp_on_->setButtonText (String::empty);
    arp_on_->addListener (this);
    arp_on_->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (save_button_ = new TextButton ("save"));
    save_button_->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    save_button_->addListener (this);

    addAndMakeVisible (beats_per_minute_ = new TwytchSlider ("beats_per_minute"));
    beats_per_minute_->setRange (20, 300, 0);
    beats_per_minute_->setSliderStyle (Slider::LinearBar);
    beats_per_minute_->setTextBoxStyle (Slider::TextBoxAbove, false, 150, 20);
    beats_per_minute_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    beats_per_minute_->setColour (Slider::textBoxTextColourId, Colours::white);
    beats_per_minute_->addListener (this);

    addAndMakeVisible (global_tool_tip_ = new GlobalToolTip());
    addAndMakeVisible (arp_sync_ = new TempoSelector ("arp_sync"));
    arp_sync_->setRange (0, 6, 1);
    arp_sync_->setSliderStyle (Slider::LinearBar);
    arp_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    arp_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    arp_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    arp_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    arp_sync_->addListener (this);


    //[UserPreSize]
    arp_tempo_ = new TwytchSlider("arp_tempo");
    addAndMakeVisible(arp_tempo_);
    arp_tempo_->setRange(0, sizeof(mopo::synced_freq_ratios) / sizeof(mopo::Value) - 1, 1);
    arp_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    arp_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    arp_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    arp_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffdddddd));
    arp_tempo_->addListener(this);

    arp_frequency_->setUnits("Hz");
    arp_frequency_->setScalingType(TwytchSlider::kExponential);
    arp_tempo_->setStringLookup(mopo::strings::synced_frequencies);
    arp_pattern_->setStringLookup(mopo::strings::arp_patterns);

    arp_sync_->setTempoSlider(arp_tempo_);
    arp_sync_->setFreeSlider(arp_frequency_);

    for (int i = 0; i < getNumChildComponents(); ++i) {
        TwytchSlider* slider = dynamic_cast<TwytchSlider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;

        Button* button = dynamic_cast<Button*>(getChildComponent(i));
        if (button)
            button_lookup_[button->getName().toStdString()] = button;
    }
    setOpaque(true);
    setAllValues(controls);
    createModulationSliders(modulation_sources, mono_modulations, poly_modulations);
    //[/UserPreSize]

    setSize (800, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

FullInterface::~FullInterface()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    synthesis_interface_ = nullptr;
    arp_frequency_ = nullptr;
    arp_gate_ = nullptr;
    arp_octaves_ = nullptr;
    arp_pattern_ = nullptr;
    oscilloscope_ = nullptr;
    load_button_ = nullptr;
    arp_on_ = nullptr;
    save_button_ = nullptr;
    beats_per_minute_ = nullptr;
    global_tool_tip_ = nullptr;
    arp_sync_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FullInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff111111));

    g.setColour (Colour (0xff777777));
    g.setFont (Font ("Myriad Pro", 11.80f, Font::plain));
    g.drawText (TRANS("GATE"),
                620, 44, 52, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font ("Myriad Pro", 11.80f, Font::plain));
    g.drawText (TRANS("FREQ"),
                540, 44, 52, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font ("Myriad Pro", 11.80f, Font::plain));
    g.drawText (TRANS("OCTAVES"),
                676, 44, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font ("Myriad Pro", 11.80f, Font::plain));
    g.drawText (TRANS("PATTERN"),
                740, 44, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font ("Myriad Pro", 11.80f, Font::plain));
    g.drawText (TRANS("BPM:"),
                132, 20, 36, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff999999));
    g.setFont (Font ("Myriad Pro", 14.40f, Font::plain));
    g.drawText (TRANS("ARP"),
                492, 20, 52, 12,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FullInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    synthesis_interface_->setBounds (0, 64, 810, 700);
    arp_frequency_->setBounds (544, 8, 46, 46);
    arp_gate_->setBounds (624, 8, 46, 46);
    arp_octaves_->setBounds (688, 8, 46, 46);
    arp_pattern_->setBounds (752, 8, 46, 46);
    oscilloscope_->setBounds (416, 8, 64, 48);
    load_button_->setBounds (8, 8, 120, 24);
    arp_on_->setBounds (488, 16, 16, 16);
    save_button_->setBounds (8, 32, 120, 24);
    beats_per_minute_->setBounds (168, 8, 112, 48);
    global_tool_tip_->setBounds (288, 8, 120, 48);
    arp_sync_->setBounds (592, 24, 16, 16);
    //[UserResized] Add your own custom resize handling here..
    modulation_manager_->setBounds(getBounds());
    arp_tempo_->setBounds(arp_frequency_->getBounds());
    //[/UserResized]
}

void FullInterface::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    std::string name = sliderThatWasMoved->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->valueChanged(name, sliderThatWasMoved->getValue());
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
    else if (sliderThatWasMoved == beats_per_minute_)
    {
        //[UserSliderCode_beats_per_minute_] -- add your slider handling code here..
        //[/UserSliderCode_beats_per_minute_]
    }
    else if (sliderThatWasMoved == arp_sync_)
    {
        //[UserSliderCode_arp_sync_] -- add your slider handling code here..
        //[/UserSliderCode_arp_sync_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void FullInterface::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == load_button_)
    {
        //[UserButtonCode_load_button_] -- add your button handler code here..
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::openMode;
        FileBrowserComponent browser(flags, File::nonexistent, nullptr, nullptr);
        FileChooserDialogBox load_dialog("load patch", "load", browser, true, Colours::white);
        if (load_dialog.show()) {
            File load_file = browser.getSelectedFile(0);

            var parsed_json_state;
            if (JSON::parse(load_file.loadFileAsString(), parsed_json_state).wasOk()) {
                SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
                parent->loadFromVar(parsed_json_state);
            }
        }
        //[/UserButtonCode_load_button_]
    }
    else if (buttonThatWasClicked == arp_on_)
    {
        //[UserButtonCode_arp_on_] -- add your button handler code here..
        std::string name = buttonThatWasClicked->getName().toStdString();
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        if (parent)
            parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
        //[/UserButtonCode_arp_on_]
    }
    else if (buttonThatWasClicked == save_button_)
    {
        //[UserButtonCode_save_button_] -- add your button handler code here..
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;
        FileBrowserComponent browser(flags, File::nonexistent, nullptr, nullptr);
        FileChooserDialogBox save_dialog("save patch", "save", browser, true, Colours::white);
        if (save_dialog.show()) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            File save_file = browser.getSelectedFile(0);
            save_file.replaceWithText(JSON::toString(parent->saveToVar()));
        }
        //[/UserButtonCode_save_button_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void FullInterface::setAllValues(mopo::control_map& controls) {
    for (auto slider : slider_lookup_)
        slider.second->setValue(controls[slider.first]->value());

    for (auto button : button_lookup_) {
        if (controls.count(button.first)) {
            button.second->setToggleState((bool)controls[button.first]->value(),
                                          NotificationType::sendNotification);
        }
    }
    synthesis_interface_->setAllValues(controls);
}

void FullInterface::setValue(std::string name, mopo::mopo_float value,
                             NotificationType notification) {
    if (slider_lookup_.count(name))
        slider_lookup_[name]->setValue(value, notification);
    else
        synthesis_interface_->setValue(name, value, notification);
}

void FullInterface::setOutputMemory(const mopo::Memory *output_memory) {
    oscilloscope_->setOutputMemory(output_memory);
    // recording_->setOutputMemory(output_memory);
}

TwytchSlider* FullInterface::getSlider(std::string name) {
    if (slider_lookup_.count(name))
        return slider_lookup_[name];
    return synthesis_interface_->getSlider(name);
}

void FullInterface::createModulationSliders(mopo::output_map modulation_sources,
                                            mopo::output_map mono_modulations,
                                            mopo::output_map poly_modulations) {
    std::map<std::string, TwytchSlider*> modulatable_sliders;

    for (auto destination : mono_modulations) {
        TwytchSlider* slider = getSlider(destination.first);
        modulatable_sliders[destination.first] = slider;
    }
    modulation_manager_ = new ModulationManager(modulation_sources,
                                                modulatable_sliders,
                                                mono_modulations, poly_modulations);
    modulation_manager_->setOpaque(false);
    addAndMakeVisible(modulation_manager_);
}

void FullInterface::setModulationConnections(std::set<mopo::ModulationConnection*> connections) {
    modulation_manager_->setModulationConnections(connections);
}

void FullInterface::changeModulator(std::string source) {
    modulation_manager_->changeModulator(source);
}

void FullInterface::setToolTipText(String parameter, String value) {
    if (global_tool_tip_)
        global_tool_tip_->setText(parameter, value);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FullInterface" componentName=""
                 parentClasses="public Component" constructorParams="mopo::control_map controls, mopo::output_map modulation_sources, mopo::output_map mono_modulations, mopo::output_map poly_modulations"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="800" initialHeight="400">
  <BACKGROUND backgroundColour="ff111111">
    <TEXT pos="620 44 52 20" fill="solid: ff777777" hasStroke="0" text="GATE"
          fontname="Myriad Pro" fontsize="11.800000000000000711" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="540 44 52 20" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Myriad Pro" fontsize="11.800000000000000711" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="676 44 60 20" fill="solid: ff777777" hasStroke="0" text="OCTAVES"
          fontname="Myriad Pro" fontsize="11.800000000000000711" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="740 44 60 20" fill="solid: ff777777" hasStroke="0" text="PATTERN"
          fontname="Myriad Pro" fontsize="11.800000000000000711" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="132 20 36 20" fill="solid: ff777777" hasStroke="0" text="BPM:"
          fontname="Myriad Pro" fontsize="11.800000000000000711" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="492 20 52 12" fill="solid: ff999999" hasStroke="0" text="ARP"
          fontname="Myriad Pro" fontsize="14.400000000000000355" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <JUCERCOMP name="" id="2ef5006082722165" memberName="synthesis_interface_"
             virtualName="" explicitFocusOrder="0" pos="0 64 810 700" sourceFile="synthesis_interface.cpp"
             constructorParams="controls"/>
  <SLIDER name="arp_frequency" id="90264eb571112e1b" memberName="arp_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="544 8 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp_gate" id="e8f61b752c6d561e" memberName="arp_gate_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="624 8 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp_octaves" id="858d1f30bb7ddacd" memberName="arp_octaves_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="688 8 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="1" max="4"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp_pattern" id="92ea11d0205b2100" memberName="arp_pattern_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="752 8 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="oscilloscope" id="341088b80b59e875" memberName="oscilloscope_"
             virtualName="" explicitFocusOrder="0" pos="416 8 64 48" sourceFile="oscilloscope.cpp"
             constructorParams="512"/>
  <TEXTBUTTON name="load" id="41af69dad8b4335d" memberName="load_button_" virtualName=""
              explicitFocusOrder="0" pos="8 8 120 24" buttonText="load" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="arp_on" id="5425f3b11382569d" memberName="arp_on_" virtualName=""
                explicitFocusOrder="0" pos="488 16 16 16" txtcol="ffffffff" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="save" id="80d4648667c9cf51" memberName="save_button_" virtualName=""
              explicitFocusOrder="0" pos="8 32 120 24" buttonText="save" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="beats_per_minute" id="ff281098ba229964" memberName="beats_per_minute_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="168 8 112 48"
          bkgcol="ff333333" textboxtext="ffffffff" min="20" max="300" int="0"
          style="LinearBar" textBoxPos="TextBoxAbove" textBoxEditable="1"
          textBoxWidth="150" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="global_tool_tip" id="c501d8243c608e75" memberName="global_tool_tip_"
             virtualName="" explicitFocusOrder="0" pos="288 8 120 48" sourceFile="global_tool_tip.cpp"
             constructorParams=""/>
  <SLIDER name="arp_sync" id="7f286b1ad8378afd" memberName="arp_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="592 24 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
