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
//[/Headers]

#include "synthesis_interface.h"
#include "oscilloscope.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FullInterface  : public Component,
                       public DragAndDropContainer,
                       public ButtonListener,
                       public SliderListener
{
public:
    //==============================================================================
    FullInterface (mopo::control_map controls);
    ~FullInterface();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setAllValues(mopo::control_map& controls);
    void setOutputMemory(const mopo::Memory* output_memory);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::map<std::string, juce::Slider*> slider_lookup_;
    std::map<std::string, juce::Button*> button_lookup_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<SynthesisInterface> synthesis_interface_;
    ScopedPointer<TextButton> save_button_;
    ScopedPointer<TextButton> load_button_;
    ScopedPointer<Slider> arp_frequency_;
    ScopedPointer<Slider> arp_gate_;
    ScopedPointer<Slider> arp_octaves_;
    ScopedPointer<Slider> arp_pattern_;
    ScopedPointer<ToggleButton> arp_on_;
    ScopedPointer<Oscilloscope> oscilloscope_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullInterface)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B574069E15301B7__
