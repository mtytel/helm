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
#include "synth_gui_interface.h"
//[/Headers]

#include "patch_browser.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PatchBrowser::PatchBrowser ()
{
    addAndMakeVisible (prev_folder_ = new TextButton ("prev_folder"));
    prev_folder_->setButtonText (TRANS("<"));
    prev_folder_->addListener (this);

    addAndMakeVisible (prev_patch_ = new TextButton ("prev_patch"));
    prev_patch_->setButtonText (TRANS("<"));
    prev_patch_->addListener (this);

    addAndMakeVisible (next_folder_ = new TextButton ("next_folder"));
    next_folder_->setButtonText (TRANS(">"));
    next_folder_->addListener (this);

    addAndMakeVisible (next_patch_ = new TextButton ("next_patch"));
    next_patch_->setButtonText (TRANS(">"));
    next_patch_->addListener (this);

    addAndMakeVisible (save_ = new TextButton ("save"));
    save_->setButtonText (TRANS("SAVE"));
    save_->addListener (this);


    //[UserPreSize]
    folder_index_ = 0;
    patch_index_ = 0;
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PatchBrowser::~PatchBrowser()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    prev_folder_ = nullptr;
    prev_patch_ = nullptr;
    next_folder_ = nullptr;
    next_patch_ = nullptr;
    save_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PatchBrowser::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff444444));
    g.fillRect(0, 0, getWidth(), proportionOfHeight(0.5));

    g.setColour(Colour(0xff424242));
    g.fillRect(0, proportionOfHeight(0.5), getWidth(), proportionOfHeight(0.5));

    g.setColour(Colour(0xffffffff));
    g.drawText(folder_text_, proportionOfWidth(0.2), 0.0,
               proportionOfWidth(0.8), proportionOfHeight(0.5), Justification::centred);
    g.drawText(patch_text_, proportionOfWidth(0.2), proportionOfHeight(0.5),
               proportionOfWidth(0.8), proportionOfHeight(0.5), Justification::centred);
    //[/UserPaint]
}

void PatchBrowser::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    prev_folder_->setBounds (proportionOfWidth (0.2000f), 0, proportionOfWidth (0.1496f), proportionOfHeight (0.5006f));
    prev_patch_->setBounds (proportionOfWidth (0.2000f), proportionOfHeight (0.5006f), proportionOfWidth (0.1496f), proportionOfHeight (0.5006f));
    next_folder_->setBounds (getWidth() - proportionOfWidth (0.1500f), 0, proportionOfWidth (0.1500f), proportionOfHeight (0.5006f));
    next_patch_->setBounds (getWidth() - proportionOfWidth (0.1500f), proportionOfHeight (0.5006f), proportionOfWidth (0.1500f), proportionOfHeight (0.5006f));
    save_->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (0.1998f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PatchBrowser::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if (buttonThatWasClicked == save_) {
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;
        FileBrowserComponent browser(flags, getPatchDirectory(), nullptr, nullptr);
        FileChooserDialogBox save_dialog("save patch", "save", browser, true, Colours::white);
        if (save_dialog.show()) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            File save_file = browser.getSelectedFile(0);
            save_file.replaceWithText(JSON::toString(parent->saveToVar()));
        }
    }
    else {
        if (buttonThatWasClicked == prev_folder_) {
            folder_index_--;
            patch_index_ = 0;
        }
        else if (buttonThatWasClicked == next_folder_) {
            folder_index_++;
            patch_index_ = 0;
        }
        else if (buttonThatWasClicked == prev_patch_)
            patch_index_--;
        else if (buttonThatWasClicked == next_patch_)
            patch_index_++;

        File folder = getCurrentFolder();
        File patch = getCurrentPatch();
        folder_text_ = folder.getFileNameWithoutExtension();
        patch_text_ = patch.getFileNameWithoutExtension();
        loadFromFile(patch);
        repaint();
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == prev_folder_)
    {
        //[UserButtonCode_prev_folder_] -- add your button handler code here..
        //[/UserButtonCode_prev_folder_]
    }
    else if (buttonThatWasClicked == prev_patch_)
    {
        //[UserButtonCode_prev_patch_] -- add your button handler code here..
        //[/UserButtonCode_prev_patch_]
    }
    else if (buttonThatWasClicked == next_folder_)
    {
        //[UserButtonCode_next_folder_] -- add your button handler code here..
        //[/UserButtonCode_next_folder_]
    }
    else if (buttonThatWasClicked == next_patch_)
    {
        //[UserButtonCode_next_patch_] -- add your button handler code here..
        //[/UserButtonCode_next_patch_]
    }
    else if (buttonThatWasClicked == save_)
    {
        //[UserButtonCode_save_] -- add your button handler code here..
        //[/UserButtonCode_save_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

File PatchBrowser::getPatchDirectory() {
    File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
    File patch_dir = data_dir.getChildFile(String("Audio/Presets/") + ProjectInfo::projectName);
    if (!patch_dir.exists())
        patch_dir.createDirectory();
    return patch_dir;
}

File PatchBrowser::getCurrentPatch() {
    Array<File> patches;
    File patch_folder = getCurrentFolder();

    patch_folder.findChildFiles(patches, File::findFiles, false, "*.twytch");
    if (patch_index_ >= patches.size())
        patch_index_ = 0;
    else if (patch_index_ < 0)
        patch_index_ = patches.size() - 1;

    return patches[patch_index_];
}

File PatchBrowser::getCurrentFolder() {
    Array<File> folders;
    File patch_dir = getPatchDirectory();

    patch_dir.findChildFiles(folders, File::findDirectories, false);
    if (folder_index_ >= folders.size())
        folder_index_ = 0;
    else if (folder_index_ < 0)
        folder_index_ = folders.size() - 1;

    return folders[folder_index_];
}

void PatchBrowser::loadFromFile(File &patch) {
    var parsed_json_state;
    if (JSON::parse(patch.loadFileAsString(), parsed_json_state).wasOk()) {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        parent->loadFromVar(parsed_json_state);
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PatchBrowser" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
  <TEXTBUTTON name="prev_folder" id="7c73e1569d0f159e" memberName="prev_folder_"
              virtualName="" explicitFocusOrder="0" pos="19.979% 0 14.958% 50.061%"
              buttonText="&lt;" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="prev_patch" id="4bd9ba5a5597cba4" memberName="prev_patch_"
              virtualName="" explicitFocusOrder="0" pos="19.979% 50.061% 14.958% 50.061%"
              buttonText="&lt;" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="next_folder" id="fc5a3ac5b9f154fe" memberName="next_folder_"
              virtualName="" explicitFocusOrder="0" pos="0Rr 0 14.958% 50.061%"
              buttonText="&gt;" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="next_patch" id="4a7cf0e77ebc8208" memberName="next_patch_"
              virtualName="" explicitFocusOrder="0" pos="0Rr 50.061% 14.958% 50.061%"
              buttonText="&gt;" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="save" id="eee3d6aa88811178" memberName="save_" virtualName=""
              explicitFocusOrder="0" pos="0% 0% 19.979% 100%" buttonText="SAVE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
