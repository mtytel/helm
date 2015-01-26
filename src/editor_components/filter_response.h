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

#ifndef __JUCE_HEADER_A5EB01B215C341A5__
#define __JUCE_HEADER_A5EB01B215C341A5__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "twytch_common.h"
#include "filter.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FilterResponse  : public Component,
                        SliderListener
{
public:
    //==============================================================================
    FilterResponse (int resolution);
    ~FilterResponse();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void resetResponsePath();
    void computeFilterCoefficients();
    void setFilterSettingsFromPosition(Point<int> position);
    void sliderValueChanged(Slider* moved_slider) override;

    void setResonanceSlider(Slider* slider);
    void setCutoffSlider(Slider* slider);
    void setFilterTypeSlider(Slider* slider);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Path filter_response_path_;
    int resolution_;

    mopo::Filter filter_;

    Slider* filter_type_slider_;
    Slider* cutoff_slider_;
    Slider* resonance_slider_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterResponse)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_A5EB01B215C341A5__
