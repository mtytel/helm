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
                           public SliderListener
{
public:
    //==============================================================================
    GraphicalEnvelope ();
    ~GraphicalEnvelope();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void resetEnvelopeLine();
    void sliderValueChanged(Slider* sliderThatWasMoved) override;

    float getAttackX();
    float getDecayX();
    float getSustainY();
    float getReleaseX();

    void setAttackX(double x);
    void setDecayX(double x);
    void setSustainY(double y);
    void setReleaseX(double x);

    void setAttackSlider(Slider* attack_slider);
    void setDecaySlider(Slider* decay_slider);
    void setSustainSlider(Slider* sustain_slider);
    void setReleaseSlider(Slider* release_slider);

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
    bool attack_hover_;
    bool decay_hover_;
    bool sustain_hover_;
    bool release_hover_;
    bool mouse_down_;
    Path envelope_line_;

    Slider* attack_slider_;
    Slider* decay_slider_;
    Slider* sustain_slider_;
    Slider* release_slider_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalEnvelope)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_37075276847E8E25__
