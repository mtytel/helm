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

#ifndef __JUCE_HEADER_37075276847E8E25__
#define __JUCE_HEADER_37075276847E8E25__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "twytch_common.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GraphicalEnvelope  : public Component,
                           public DragAndDropContainer
{
public:
    //==============================================================================
    GraphicalEnvelope ();
    ~GraphicalEnvelope();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void childBoundsChanged (Component *child) override;
    void resetEnvelopeLine();

    void setTimeSkewFactor(double skew) {
        time_skew_factor_ = skew;
    }

    void setAttackControl(mopo::Control* attack_control) {
        attack_control_ = attack_control;
    }

    void setDecayControl(mopo::Control* decay_control) {
        decay_control_ = decay_control;
    }

    void setSustainControl(mopo::Control* sustain_control) {
        sustain_control_ = sustain_control;
    }

    void setReleaseControl(mopo::Control* release_control) {
        release_control_ = release_control;
    }

    void setPositionsFromValues();

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void mouseMove (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    float attack_percent_;
    float decay_percent_;
    float sustain_percent_;
    float release_percent_;
    Path envelope_line_;

    double time_skew_factor_;
    mopo::Control* attack_control_;
    mopo::Control* decay_control_;
    mopo::Control* sustain_control_;
    mopo::Control* release_control_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalEnvelope)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_37075276847E8E25__
