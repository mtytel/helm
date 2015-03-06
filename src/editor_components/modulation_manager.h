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

#ifndef __JUCE_HEADER_4114FD08478B2179__
#define __JUCE_HEADER_4114FD08478B2179__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "twytch_common.h"
#include "modulation_look_and_feel.h"
#include "modulation_meter.h"
#include <set>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ModulationManager  : public Component,
                           public SliderListener,
                           public Timer
{
public:
    //==============================================================================
    ModulationManager (mopo::output_map modulation_sources, std::map<std::string, Slider*> sliders, mopo::output_map mono_modulations, mopo::output_map poly_modulations);
    ~ModulationManager();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void sliderValueChanged(Slider *slider);
    void setModulationAmount(std::string source, std::string destination, mopo::mopo_float amount);
    void clearModulation(std::string source, std::string destination);
    void changeModulator(std::string new_modulator);

    void timerCallback() override;

    void clearModulationConnections();
    void setModulationConnections(std::set<mopo::ModulationConnection*> connections);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ModulationLookAndFeel look_and_feel_;
    ScopedPointer<Component> polyphonic_destinations_;
    ScopedPointer<Component> monophonic_destinations_;

    std::map<std::string, std::map<std::string, mopo::ModulationConnection*>> connections_;
    std::string current_modulator_;

    std::map<std::string, Slider*> slider_lookup_;
    std::map<std::string, Slider*> slider_model_lookup_;
    std::vector<ScopedPointer<Slider>> owned_sliders_;

    std::vector<ScopedPointer<ModulationMeter>> meters_;
    mopo::output_map modulation_sources_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationManager)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_4114FD08478B2179__
