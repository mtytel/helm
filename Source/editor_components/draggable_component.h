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

#ifndef __JUCE_HEADER_FD9A527AD144B567__
#define __JUCE_HEADER_FD9A527AD144B567__

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
class DraggableComponent  : public Component
{
public:
    //==============================================================================
    DraggableComponent ();
    ~DraggableComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setMovementRestrictions(Rectangle<int> restrictions) { restrictions_ = restrictions; }
    void setMovementRestrictions(int x, int y, int w, int h) {
        setMovementRestrictions(Rectangle<int>(x, y, w, h));
    }
    Rectangle<int> getMovementRestrictions() { return restrictions_; }
    bool isHovered() { return is_hovered_; }
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void moved();
    void mouseEnter (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ComponentDragger dragger_;
    Rectangle<int> restrictions_;
    bool is_hovered_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_FD9A527AD144B567__
