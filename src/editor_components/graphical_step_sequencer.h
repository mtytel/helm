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

#ifndef __JUCE_HEADER_9CB049E4401FF7BE__
#define __JUCE_HEADER_9CB049E4401FF7BE__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include <vector>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GraphicalStepSequencer  : public Component,
                                public SliderListener
{
public:
    //==============================================================================
    GraphicalStepSequencer ();
    ~GraphicalStepSequencer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setNumStepsSlider(Slider* num_steps_slider);
    void setStepSliders(std::vector<ScopedPointer<Slider> >* sliders);
    void changeStep(const MouseEvent& e);
    int getHoveredStep(Point<int> position);
    void updateHover(int step_index);
    void sliderValueChanged(Slider* moved_slider) override;
    void ensureMinSize();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void mouseMove (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int num_steps_;
    Slider* num_steps_slider_;
    int highlighted_step_;
    std::vector<ScopedPointer<Slider> >* sequence_;
    Point<int> last_edit_position_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalStepSequencer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9CB049E4401FF7BE__
