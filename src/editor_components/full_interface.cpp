/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

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
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (synthesis_interface_ = new SynthesisInterface (controls));
    addAndMakeVisible (arp_frequency_ = new SynthSlider ("arp_frequency"));
    arp_frequency_->setRange (-1, 4, 0);
    arp_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    arp_frequency_->addListener (this);

    addAndMakeVisible (arp_gate_ = new SynthSlider ("arp_gate"));
    arp_gate_->setRange (0, 1, 0);
    arp_gate_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_gate_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_gate_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_gate_->setColour (Slider::textBoxTextColourId, Colours::white);
    arp_gate_->addListener (this);

    addAndMakeVisible (arp_octaves_ = new SynthSlider ("arp_octaves"));
    arp_octaves_->setRange (1, 4, 1);
    arp_octaves_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_octaves_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_octaves_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_octaves_->setColour (Slider::textBoxTextColourId, Colours::white);
    arp_octaves_->addListener (this);

    addAndMakeVisible (arp_pattern_ = new SynthSlider ("arp_pattern"));
    arp_pattern_->setRange (0, 4, 1);
    arp_pattern_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    arp_pattern_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    arp_pattern_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    arp_pattern_->setColour (Slider::textBoxTextColourId, Colours::white);
    arp_pattern_->addListener (this);

    addAndMakeVisible (oscilloscope_ = new Oscilloscope (512));
    addAndMakeVisible (arp_on_ = new ToggleButton ("arp_on"));
    arp_on_->setButtonText (String::empty);
    arp_on_->addListener (this);
    arp_on_->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (beats_per_minute_ = new SynthSlider ("beats_per_minute"));
    beats_per_minute_->setRange (20, 300, 0);
    beats_per_minute_->setSliderStyle (Slider::LinearBar);
    beats_per_minute_->setTextBoxStyle (Slider::TextBoxAbove, false, 150, 20);
    beats_per_minute_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    beats_per_minute_->setColour (Slider::textBoxTextColourId, Colours::white);
    beats_per_minute_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    beats_per_minute_->addListener (this);

    addAndMakeVisible (global_tool_tip_ = new GlobalToolTip());
    addAndMakeVisible (arp_sync_ = new TempoSelector ("arp_sync"));
    arp_sync_->setRange (0, 6, 1);
    arp_sync_->setSliderStyle (Slider::LinearBar);
    arp_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    arp_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    arp_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    arp_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xffbbbbbb));
    arp_sync_->addListener (this);

    addAndMakeVisible (patch_browser_ = new PatchBrowser());

    //[UserPreSize]
    arp_tempo_ = new SynthSlider("arp_tempo");
    addAndMakeVisible(arp_tempo_);
    arp_tempo_->setRange(0, sizeof(mopo::synced_freq_ratios) / sizeof(mopo::Value) - 1, 1);
    arp_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    arp_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    arp_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    arp_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
    arp_tempo_->addListener(this);

    arp_frequency_->setUnits("Hz");
    arp_frequency_->setScalingType(SynthSlider::kExponential);
    arp_tempo_->setStringLookup(mopo::strings::synced_frequencies);
    arp_pattern_->setStringLookup(mopo::strings::arp_patterns);

    arp_sync_->setStringLookup(mopo::strings::freq_sync_styles);
    arp_sync_->setTempoSlider(arp_tempo_);
    arp_sync_->setFreeSlider(arp_frequency_);

    for (int i = 0; i < getNumChildComponents(); ++i) {
        SynthSlider* slider = dynamic_cast<SynthSlider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;

        Button* button = dynamic_cast<Button*>(getChildComponent(i));
        if (button)
            button_lookup_[button->getName().toStdString()] = button;
    }

    setAllValues(controls);
    createModulationSliders(modulation_sources, mono_modulations, poly_modulations);
    //[/UserPreSize]

    setSize (800, 400);


    //[Constructor] You can add your own custom stuff here..
    setOpaque(true);
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
    arp_on_ = nullptr;
    beats_per_minute_ = nullptr;
    global_tool_tip_ = nullptr;
    arp_sync_ = nullptr;
    patch_browser_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FullInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    static const DropShadow shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
    static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
    static Font roboto_thin(Typeface::createSystemTypefaceFor(BinaryData::RobotoThin_ttf,
                                                              BinaryData::RobotoThin_ttfSize));

    g.setColour(Colour(0xff212121));
    g.fillRect(getLocalBounds());

    shadow.drawForRectangle(g, Rectangle<int>(472, 8, 266, 60));
    shadow.drawForRectangle(g, Rectangle<int>(16, 8, 220, 60));

    shadow.drawForRectangle(g, Rectangle<int>(376, 8, 88, 60));
    shadow.drawForRectangle(g, Rectangle<int>(368 - 124, 8, 124, 60));
    //[/UserPrePaint]

    g.setColour (Colour (0xff303030));
    g.fillRoundedRectangle (472.0f, 8.0f, 266.0f, 60.0f, 3.000f);

    g.setColour (Colour (0xff303030));
    g.fillRect (16, 8, 220, 60);

    g.setColour (Colour (0xffbbbbbb));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("GATE"),
                599 - (52 / 2), 54, 52, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffbbbbbb));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                527 - (52 / 2), 54, 52, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffbbbbbb));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("OCTAVES"),
                647 - (60 / 2), 54, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffbbbbbb));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PATTERN"),
                703 - (60 / 2), 54, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xffbbbbbb));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("BPM"),
                38 - (44 / 2), 56, 44, 10,
                Justification::centred, true);

    g.setGradientFill (ColourGradient (Colour (0x00000000),
                                       490.0f, 0.0f,
                                       Colours::black,
                                       494.0f, 0.0f,
                                       false));
    g.fillRect (472, 8, 20, 60);

    //[UserPaint] Add your own custom painting code here..

    component_shadow.drawForRectangle(g, patch_browser_->getBounds());

    for (auto slider : slider_lookup_)
        slider.second->drawShadow(g);

    g.addTransform(AffineTransform::rotation(-mopo::PI / 2.0f, 460, 20));
    g.setColour(Colour (0xff999999));
    g.setFont(Font(Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText(TRANS("ARP"),
               408, 35, 52, 12,
               Justification::centred, true);

    //[/UserPaint]
}

void FullInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    synthesis_interface_->setBounds (746 - 738, 744 - 672, 738, 672);
    arp_frequency_->setBounds (527 - (40 / 2), 12, 40, 40);
    arp_gate_->setBounds (599 - (40 / 2), 12, 40, 40);
    arp_octaves_->setBounds (647 - (40 / 2), 12, 40, 40);
    arp_pattern_->setBounds (703 - (40 / 2), 12, 40, 40);
    oscilloscope_->setBounds (376, 8, 88, 60);
    arp_on_->setBounds (474, 12, 16, 16);
    beats_per_minute_->setBounds (148 - (176 / 2), 48, 176, 20);
    global_tool_tip_->setBounds (368 - 124, 8, 124, 60);
    arp_sync_->setBounds (552, 28, 16, 16);
    patch_browser_->setBounds (16, 8, 220, 40);
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

    if (buttonThatWasClicked == arp_on_)
    {
        //[UserButtonCode_arp_on_] -- add your button handler code here..
        std::string name = buttonThatWasClicked->getName().toStdString();
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        if (parent)
            parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
        //[/UserButtonCode_arp_on_]
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
}

SynthSlider* FullInterface::getSlider(std::string name) {
    if (slider_lookup_.count(name))
        return slider_lookup_[name];
    return synthesis_interface_->getSlider(name);
}

void FullInterface::createModulationSliders(mopo::output_map modulation_sources,
                                            mopo::output_map mono_modulations,
                                            mopo::output_map poly_modulations) {
    std::map<std::string, SynthSlider*> modulatable_sliders;

    for (auto destination : mono_modulations) {
        SynthSlider* slider = getSlider(destination.first);
        if (slider)
            modulatable_sliders[destination.first] = slider;
    }
    modulation_manager_ = new ModulationManager(modulation_sources,
                                                modulatable_sliders,
                                                mono_modulations, poly_modulations);
    modulation_manager_->setOpaque(false);
    addAndMakeVisible(modulation_manager_);
}

void FullInterface::changeModulator(std::string source) {
    modulation_manager_->changeModulator(source);
}

void FullInterface::forgetModulator() {
    modulation_manager_->forgetModulator();
}

void FullInterface::modulationChanged(std::string source) {
    synthesis_interface_->modulationChanged(source);
}

std::string FullInterface::getCurrentModulator() {
    return modulation_manager_->getCurrentModulator();
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
                 variableInitialisers="" snapPixels="2" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="800" initialHeight="400">
  <BACKGROUND backgroundColour="303030">
    <ROUNDRECT pos="472 8 266 60" cornerSize="3" fill="solid: ff303030" hasStroke="0"/>
    <RECT pos="16 8 220 60" fill="solid: ff303030" hasStroke="0"/>
    <TEXT pos="599c 54 52 10" fill="solid: ffbbbbbb" hasStroke="0" text="GATE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="527c 54 52 10" fill="solid: ffbbbbbb" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="647c 54 60 10" fill="solid: ffbbbbbb" hasStroke="0" text="OCTAVES"
          fontname="Default sans-serif font" fontsize="11.400000000000000355" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="703c 54 60 10" fill="solid: ffbbbbbb" hasStroke="0" text="PATTERN"
          fontname="Default sans-serif font" fontsize="11.400000000000000355" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="38c 56 44 10" fill="solid: ffbbbbbb" hasStroke="0" text="BPM"
          fontname="Default sans-serif font" fontsize="11.400000000000000355" bold="0"
          italic="0" justification="36"/>
    <RECT pos="472 8 20 60" fill="linear: 490 0, 494 0, 0=0, 1=ff000000"
          hasStroke="0"/>
  </BACKGROUND>
  <JUCERCOMP name="" id="2ef5006082722165" memberName="synthesis_interface_"
             virtualName="" explicitFocusOrder="0" pos="746r 744r 738 672"
             sourceFile="synthesis_interface.cpp" constructorParams="controls"/>
  <SLIDER name="arp_frequency" id="90264eb571112e1b" memberName="arp_frequency_"
          virtualName="SynthSlider" explicitFocusOrder="0" pos="527c 12 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp_gate" id="e8f61b752c6d561e" memberName="arp_gate_"
          virtualName="SynthSlider" explicitFocusOrder="0" pos="599c 12 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp_octaves" id="858d1f30bb7ddacd" memberName="arp_octaves_"
          virtualName="SynthSlider" explicitFocusOrder="0" pos="647c 12 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="1" max="4"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="arp_pattern" id="92ea11d0205b2100" memberName="arp_pattern_"
          virtualName="SynthSlider" explicitFocusOrder="0" pos="703c 12 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="oscilloscope" id="341088b80b59e875" memberName="oscilloscope_"
             virtualName="" explicitFocusOrder="0" pos="376 8 88 60" sourceFile="oscilloscope.cpp"
             constructorParams="512"/>
  <TOGGLEBUTTON name="arp_on" id="5425f3b11382569d" memberName="arp_on_" virtualName=""
                explicitFocusOrder="0" pos="474 12 16 16" txtcol="ffffffff" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="beats_per_minute" id="ff281098ba229964" memberName="beats_per_minute_"
          virtualName="SynthSlider" explicitFocusOrder="0" pos="148c 48 176 20"
          bkgcol="ff333333" textboxtext="ffffffff" textboxoutline="808080"
          min="20" max="300" int="0" style="LinearBar" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="150" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="global_tool_tip" id="c501d8243c608e75" memberName="global_tool_tip_"
             virtualName="" explicitFocusOrder="0" pos="368r 8 124 60" sourceFile="global_tool_tip.cpp"
             constructorParams=""/>
  <SLIDER name="arp_sync" id="7f286b1ad8378afd" memberName="arp_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="552 28 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ffbbbbbb"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <JUCERCOMP name="patch_browser" id="dbae22170345d3ef" memberName="patch_browser_"
             virtualName="" explicitFocusOrder="0" pos="16 8 220 40" sourceFile="patch_browser.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
