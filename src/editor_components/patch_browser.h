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

#ifndef __JUCE_HEADER_36E92220428C85BB__
#define __JUCE_HEADER_36E92220428C85BB__

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
class PatchBrowser  : public Component,
                      public ButtonListener
{
public:
    //==============================================================================
    PatchBrowser ();
    ~PatchBrowser();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    File getPatchDirectory();

    void refreshPatches();

    File getCurrentPatch();
    File getCurrentFolder();

    void loadFromFile(File& patch);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int folder_index_;
    int patch_index_;

    String folder_text_;
    String patch_text_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> prev_folder_;
    ScopedPointer<TextButton> prev_patch_;
    ScopedPointer<TextButton> next_folder_;
    ScopedPointer<TextButton> next_patch_;
    ScopedPointer<TextButton> save_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchBrowser)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_36E92220428C85BB__
