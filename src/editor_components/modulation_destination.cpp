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
#include "value_change_manager.h"
//[/Headers]

#include "modulation_destination.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#define MAX_CONNECTIONS 4
//[/MiscUserDefs]

//==============================================================================
ModulationDestination::ModulationDestination (std::string name)
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    setName(name);
    //[/Constructor]
}

ModulationDestination::~ModulationDestination()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ModulationDestination::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffd4d4d4));
    g.drawRect (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f), 3);

    //[UserPaint] Add your own custom painting code here..
    if (modulation_scales_.size() > 3) {
        g.setColour (Colour (0xffdd4444));
        g.fillRect(proportionOfWidth(0.5f), proportionOfHeight(0.5f),
                   proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);
    }
    if (modulation_scales_.size() > 2) {
        g.setColour (Colour (0xff44dd77));
        g.fillRect(3, proportionOfHeight(0.5f),
                   proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);
    }
    if (modulation_scales_.size() > 1) {
        g.setColour (Colour (0xff4477dd));
        g.fillRect(proportionOfWidth(0.5f), 3,
                   proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);
    }
    if (modulation_scales_.size() > 0) {
        g.setColour (Colour (0xffaa22dd));
        g.fillRect(3, 3, proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);
    }
    //[/UserPaint]
}

void ModulationDestination::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ModulationDestination::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    if (!e.mods.isRightButtonDown()) {
        int index = 0;
        if (e.getPosition().x > proportionOfWidth(0.5f))
            index += 1;
        if (e.getPosition().y > proportionOfWidth(0.5f))
            index += 2;
        if (index < modulation_scales_.size()) {
            ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
            parent->disconnectModulation(getName().toStdString(), modulation_scales_[index]);
            modulation_scales_.erase(modulation_scales_.begin() + index);
        }
        repaint();
    }
    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ModulationDestination::itemDropped(const SourceDetails &drag_source) {
    if (modulation_scales_.size() >= MAX_CONNECTIONS)
        return;

    std::string source_name = drag_source.description.toString().toStdString();
    mopo::Value* scale = new mopo::Value(1.0);
    
    ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
    parent->connectModulation(source_name, getName().toStdString(), scale);
    modulation_scales_.push_back(scale);
    repaint();
}

bool ModulationDestination::isInterestedInDragSource(const SourceDetails &drag_source) {
    return true;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModulationDestination" componentName=""
                 parentClasses="public Component, public DragAndDropTarget" constructorParams="std::string name"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff">
    <RECT pos="0 0 100% 100%" fill="solid: 2aa530" hasStroke="1" stroke="3.1, mitered, butt"
          strokeColour="solid: ffd4d4d4"/>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
