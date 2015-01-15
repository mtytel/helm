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


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class FullInterface  : public Component,
                       public ButtonListener
{
public:
    //==============================================================================
    FullInterface ();
    ~FullInterface();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void addControls(mopo::control_map controls) { synthesis_interface_->addControls(controls); }
    var getState();
    void writeState(var state);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<SynthesisInterface> synthesis_interface_;
    ScopedPointer<TextButton> save_button_;
    ScopedPointer<TextButton> load_button_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FullInterface)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B574069E15301B7__
