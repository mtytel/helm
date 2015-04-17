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

#ifndef __JUCE_HEADER_B574069E15301B7__
#define __JUCE_HEADER_B574069E15301B7__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "modulation_manager.h"
//[/Headers]

#include "synthesis_interface.h"
#include "oscilloscope.h"
#include "global_tool_tip.h"
#include "patch_browser.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FullInterface  : public Component,
                       public SliderListener,
                       public ButtonListener
{
public:
    //==============================================================================
    FullInterface (mopo::control_map controls, mopo::output_map modulation_sources, mopo::output_map mono_modulations, mopo::output_map poly_modulations);
    ~FullInterface();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setAllValues(mopo::control_map& controls);
    void setValue(std::string name, mopo::mopo_float value,
                  NotificationType notification = sendNotificationAsync);
    void setOutputMemory(const mopo::Memory* output_memory);

    TwytchSlider* getSlider(std::string name);
    void createModulationSliders(mopo::output_map modulation_sources,
                                 mopo::output_map mono_modulations,
                                 mopo::output_map poly_modulations);
    void setModulationConnections(std::set<mopo::ModulationConnection*> connections);
    void changeModulator(std::string source);
    void setToolTipText(String parameter, String value);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::map<std::string, TwytchSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    ScopedPointer<ModulationManager> modulation_manager_;
    TooltipWindow tooltip_;
    ScopedPointer<TwytchSlider> arp_tempo_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<SynthesisInterface> synthesis_interface_;
    ScopedPointer<TwytchSlider> arp_frequency_;
    ScopedPointer<TwytchSlider> arp_gate_;
    ScopedPointer<TwytchSlider> arp_octaves_;
    ScopedPointer<TwytchSlider> arp_pattern_;
    ScopedPointer<Oscilloscope> oscilloscope_;
    ScopedPointer<ToggleButton> arp_on_;
    ScopedPointer<TwytchSlider> beats_per_minute_;
    ScopedPointer<GlobalToolTip> global_tool_tip_;
    ScopedPointer<TempoSelector> arp_sync_;
    ScopedPointer<PatchBrowser> global_tool_tip_2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullInterface)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B574069E15301B7__
