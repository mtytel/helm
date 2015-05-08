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

//[Headers] You can add your own extra header files here...
#include "synth_gui_interface.h"
#include "browser_look_and_feel.h"
//[/Headers]

#include "patch_browser.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#define PATCH_EXTENSION "twytch"
#define TEXT_PADDING 4.0f
#define LINUX_SYSTEM_PATCH_DIRECTORY "/usr/share/twytch/patches"
#define LINUX_USER_PATCH_DIRECTORY "/usr/local/share/twytch/patches"
//[/MiscUserDefs]

//==============================================================================
PatchBrowser::PatchBrowser ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    setLookAndFeel(BrowserLookAndFeel::instance());
    //[/Constructor_pre]

    addAndMakeVisible (prev_folder_ = new TextButton ("prev_folder"));
    prev_folder_->setButtonText (TRANS("<"));
    prev_folder_->addListener (this);
    prev_folder_->setColour (TextButton::buttonColourId, Colour (0xff303030));
    prev_folder_->setColour (TextButton::buttonOnColourId, Colours::black);
    prev_folder_->setColour (TextButton::textColourOffId, Colours::white);

    addAndMakeVisible (prev_patch_ = new TextButton ("prev_patch"));
    prev_patch_->setButtonText (TRANS("<"));
    prev_patch_->addListener (this);
    prev_patch_->setColour (TextButton::buttonColourId, Colour (0xff464646));
    prev_patch_->setColour (TextButton::buttonOnColourId, Colours::black);
    prev_patch_->setColour (TextButton::textColourOffId, Colours::white);

    addAndMakeVisible (next_folder_ = new TextButton ("next_folder"));
    next_folder_->setButtonText (TRANS(">"));
    next_folder_->addListener (this);
    next_folder_->setColour (TextButton::buttonColourId, Colour (0xff303030));
    next_folder_->setColour (TextButton::buttonOnColourId, Colours::black);
    next_folder_->setColour (TextButton::textColourOffId, Colours::white);

    addAndMakeVisible (next_patch_ = new TextButton ("next_patch"));
    next_patch_->setButtonText (TRANS(">"));
    next_patch_->addListener (this);
    next_patch_->setColour (TextButton::buttonColourId, Colour (0xff464646));
    next_patch_->setColour (TextButton::buttonOnColourId, Colour (0xff212121));
    next_patch_->setColour (TextButton::textColourOffId, Colours::white);

    addAndMakeVisible (save_ = new TextButton ("save"));
    save_->setButtonText (TRANS("SAVE"));
    save_->addListener (this);
    save_->setColour (TextButton::buttonColourId, Colour (0xff464646));
    save_->setColour (TextButton::buttonOnColourId, Colours::black);
    save_->setColour (TextButton::textColourOffId, Colours::white);


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
    static const DropShadow shadow(Colour(0xff000000), 4, Point<int>(0, 0));
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colour(0xff303030));
    g.fillRect(0, 0, getWidth(), proportionOfHeight(0.5));

    g.setColour(Colour(0xff464646));
    g.fillRect(0, proportionOfHeight(0.5), getWidth(), proportionOfHeight(0.5));

    Rectangle<int> left(proportionOfWidth(0.2), 0,
                        proportionOfWidth(0.1), proportionOfHeight(1.0));
    Rectangle<int> right(proportionOfWidth(0.9), 0,
                         proportionOfWidth(0.1), proportionOfHeight(1.0));
    shadow.drawForRectangle(g, left);
    shadow.drawForRectangle(g, right);

    g.setFont(Font(Font::getDefaultMonospacedFontName(), 12.0f, Font::plain));
    g.setColour(Colour(0xffbbbbbb));
    g.drawText(folder_text_, proportionOfWidth(0.3) + TEXT_PADDING, 0.0,
               proportionOfWidth(0.6) - TEXT_PADDING, proportionOfHeight(0.5),
               Justification::centredLeft);
    g.setColour(Colour(0xffffffff));
    g.drawText(patch_text_, proportionOfWidth(0.3) + TEXT_PADDING, proportionOfHeight(0.5),
               proportionOfWidth(0.6) - TEXT_PADDING, proportionOfHeight(0.5),
               Justification::centredLeft);
    //[/UserPaint]
}

void PatchBrowser::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    prev_folder_->setBounds (proportionOfWidth (0.1998f), 0, proportionOfWidth (0.1004f), proportionOfHeight (0.5006f));
    prev_patch_->setBounds (proportionOfWidth (0.1998f), proportionOfHeight (0.5006f), proportionOfWidth (0.1004f), proportionOfHeight (0.5006f));
    next_folder_->setBounds (getWidth() - proportionOfWidth (0.1004f), 0, proportionOfWidth (0.1004f), proportionOfHeight (0.5006f));
    next_patch_->setBounds (getWidth() - proportionOfWidth (0.1004f), proportionOfHeight (0.5006f), proportionOfWidth (0.1004f), proportionOfHeight (0.5006f));
    save_->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (0.1950f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PatchBrowser::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if (buttonThatWasClicked == save_) {
        int flags = FileBrowserComponent::canSelectFiles | FileBrowserComponent::saveMode;
        FileBrowserComponent browser(flags, getUserPatchDirectory(), nullptr, nullptr);
        FileChooserDialogBox save_dialog("save patch", "save", browser, true, Colours::white);
        if (save_dialog.show()) {
            SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
            File save_file = browser.getSelectedFile(0);
            if (save_file.getFileExtension() != PATCH_EXTENSION)
                save_file = save_file.withFileExtension(PATCH_EXTENSION);
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

File PatchBrowser::getSystemPatchDirectory() {
    File patch_dir = File("");
#ifdef LINUX
    patch_dir = File(LINUX_SYSTEM_PATCH_DIRECTORY);
#elif defined(__APPLE__)
    File data_dir = File::getSpecialLocation(File::commonApplicationDataDirectory);
    patch_dir = data_dir.getChildFile(String("Audio/Presets/") + ProjectInfo::projectName);
#elif defined(_WIN32)
    patch_dir = File("C:");
#endif

    if (!patch_dir.exists())
        patch_dir.createDirectory();
    return patch_dir;
}

File PatchBrowser::getUserPatchDirectory() {
    File patch_dir = File("");
#ifdef LINUX
    patch_dir = File(LINUX_USER_PATCH_DIRECTORY);
#elif defined(__APPLE__)
    File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
    File patch_dir = data_dir.getChildFile(String("Audio/Presets/") + ProjectInfo::projectName);
#elif defined(_WIN32)
    patch_dir = File("C:");
#endif

    if (!patch_dir.exists())
        patch_dir.createDirectory();
    return patch_dir;
}

File PatchBrowser::getCurrentPatch() {
    Array<File> patches;
    File patch_folder = getCurrentFolder();

    patch_folder.findChildFiles(patches, File::findFiles, false, String("*.") + PATCH_EXTENSION);
    if (patch_index_ >= patches.size())
        patch_index_ = 0;
    else if (patch_index_ < 0)
        patch_index_ = patches.size() - 1;

    return patches[patch_index_];
}

File PatchBrowser::getCurrentFolder() {
    Array<File> folders;
    File patch_dir = getSystemPatchDirectory();

    patch_dir.findChildFiles(folders, File::findDirectories, false);
    folders.add(getUserPatchDirectory());
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
              virtualName="" explicitFocusOrder="0" pos="19.979% 0 10.042% 50.061%"
              bgColOff="ff303030" bgColOn="ff000000" textColOn="ffffffff" buttonText="&lt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="prev_patch" id="4bd9ba5a5597cba4" memberName="prev_patch_"
              virtualName="" explicitFocusOrder="0" pos="19.979% 50.061% 10.042% 50.061%"
              bgColOff="ff464646" bgColOn="ff000000" textColOn="ffffffff" buttonText="&lt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="next_folder" id="fc5a3ac5b9f154fe" memberName="next_folder_"
              virtualName="" explicitFocusOrder="0" pos="0Rr 0 10.042% 50.061%"
              bgColOff="ff303030" bgColOn="ff000000" textColOn="ffffffff" buttonText="&gt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="next_patch" id="4a7cf0e77ebc8208" memberName="next_patch_"
              virtualName="" explicitFocusOrder="0" pos="0Rr 50.061% 10.042% 50.061%"
              bgColOff="ff464646" bgColOn="ff212121" textColOn="ffffffff" buttonText="&gt;"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="save" id="eee3d6aa88811178" memberName="save_" virtualName=""
              explicitFocusOrder="0" pos="0% 0% 19.456% 100%" bgColOff="ff464646"
              bgColOn="ff000000" textColOn="ffffffff" buttonText="SAVE" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
