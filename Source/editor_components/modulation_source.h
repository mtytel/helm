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

#ifndef __JUCE_HEADER_E998F9B49B6EECD5__
#define __JUCE_HEADER_E998F9B49B6EECD5__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ModulationSource  : public Component,
                          public DragAndDropContainer
{
public:
    //==============================================================================
    ModulationSource (std::string name);
    ~ModulationSource();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool isHovered() { return is_hovered_; }
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void moved();
    void mouseEnter (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ComponentDragger dragger_;
    bool is_hovered_;
    Rectangle<int> initial_bounds_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationSource)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_E998F9B49B6EECD5__
