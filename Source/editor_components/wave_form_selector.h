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

#ifndef __JUCE_HEADER_E4E0EA192D66F72__
#define __JUCE_HEADER_E4E0EA192D66F72__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "wave.h"
#include "twytch_common.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WaveFormSelector  : public Component,
                          public SliderListener
{
public:
    //==============================================================================
    WaveFormSelector (int resolution);
    ~WaveFormSelector();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setWaveSlider(Slider* slider);
    void resetWavePath();
    void sliderValueChanged(Slider* sliderThatWasMoved) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void mouseDown (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Slider* wave_slider_;
    Path wave_path_;
    int resolution_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveFormSelector)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_E4E0EA192D66F72__
