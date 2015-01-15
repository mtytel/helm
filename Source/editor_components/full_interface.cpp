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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "full_interface.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
FullInterface::FullInterface ()
{
    addAndMakeVisible (synthesis_interface_ = new SynthesisInterface());
    addAndMakeVisible (save_button_ = new TextButton ("save"));
    save_button_->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    save_button_->addListener (this);

    addAndMakeVisible (load_button_ = new TextButton ("load"));
    load_button_->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    load_button_->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

FullInterface::~FullInterface()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    synthesis_interface_ = nullptr;
    save_button_ = nullptr;
    load_button_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void FullInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void FullInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    synthesis_interface_->setBounds (0, 40, proportionOfWidth (1.0000f), getHeight() - 40);
    save_button_->setBounds (192, 8, 150, 24);
    load_button_->setBounds (24, 8, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void FullInterface::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == save_button_)
    {
        //[UserButtonCode_save_button_] -- add your button handler code here..
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;
        FileBrowserComponent browser(flags, File::nonexistent, nullptr, nullptr);
        FileChooserDialogBox save_dialog("save patch", "save", browser, true, Colours::white);
        if (save_dialog.show()) {
            File save_file = browser.getSelectedFile(0);
            save_file.replaceWithText(JSON::toString(getState()));
        }
        //[/UserButtonCode_save_button_]
    }
    else if (buttonThatWasClicked == load_button_)
    {
        //[UserButtonCode_load_button_] -- add your button handler code here..
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::openMode;
        FileBrowserComponent browser(flags, File::nonexistent, nullptr, nullptr);
        FileChooserDialogBox load_dialog("load patch", "load", browser, true, Colours::white);
        if (load_dialog.show()) {
            File load_file = browser.getSelectedFile(0);

            var parsed_json_state;
            if (JSON::parse(load_file.loadFileAsString(), parsed_json_state).wasOk())
                writeState(parsed_json_state);
        }
        //[/UserButtonCode_load_button_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

var FullInterface::getState() {
    DynamicObject* state_object = new DynamicObject();
    state_object->setProperty("synthesis", synthesis_interface_->getState());
    return state_object;
}

void FullInterface::writeState(var state) {
    DynamicObject* object_state = state.getDynamicObject();
    synthesis_interface_->writeState(object_state->getProperty("synthesis"));
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="FullInterface" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff000000"/>
  <JUCERCOMP name="" id="2ef5006082722165" memberName="synthesis_interface_"
             virtualName="" explicitFocusOrder="0" pos="0 40 100% 40M" sourceFile="synthesis_interface.cpp"
             constructorParams=""/>
  <TEXTBUTTON name="save" id="80d4648667c9cf51" memberName="save_button_" virtualName=""
              explicitFocusOrder="0" pos="192 8 150 24" buttonText="save" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="load" id="41af69dad8b4335d" memberName="load_button_" virtualName=""
              explicitFocusOrder="0" pos="24 8 150 24" buttonText="load" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
