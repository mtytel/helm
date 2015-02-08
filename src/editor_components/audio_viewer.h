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

#ifndef __JUCE_HEADER_523C1D32EA61B457__
#define __JUCE_HEADER_523C1D32EA61B457__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "memory.h"
#include <list>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AudioViewer  : public AnimatedAppComponent
{
public:
    //==============================================================================
    AudioViewer (int num_samples);
    ~AudioViewer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void update() override;
    void resetWavePath();
    void setOutputMemory(const mopo::Memory* memory) {
      output_memory_ = memory;
      last_offset_ = output_memory_->getOffset();
    }
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    const mopo::Memory* output_memory_;
    std::list<float> peak_values_;
    int last_offset_;
    int samples_to_show_;
    Path wave_top_;
    Path wave_bottom_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioViewer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_523C1D32EA61B457__
