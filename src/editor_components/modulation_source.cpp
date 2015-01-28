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

#include "modulation_source.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ModulationSource::ModulationSource (std::string name, Colour color)
{

    //[UserPreSize]
    is_hovered_ = false;
    color_ = color;
    DynamicObject* description_object = new DynamicObject();
    description_object->setProperty("name", name.c_str());
    description_object->setProperty("color", (int)color.getARGB());
    description_ = var(description_object);
    //[/UserPreSize]

    setSize (20, 20);


    //[Constructor] You can add your own custom stuff here..
    setName(name);
    //[/Constructor]
}

ModulationSource::~ModulationSource()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ModulationSource::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffc8c8c8));
    g.drawRoundedRectangle (static_cast<float> (proportionOfWidth (0.5000f) - (proportionOfWidth (0.2500f) / 2)), static_cast<float> (proportionOfHeight (0.5000f) - (proportionOfHeight (0.2500f) / 2)), static_cast<float> (proportionOfWidth (0.2500f)), static_cast<float> (proportionOfHeight (0.2500f)), 2.000f, 4.000f);

    //[UserPaint] Add your own custom painting code here..
    g.setColour (color_);
    g.drawRoundedRectangle (static_cast<float> (proportionOfWidth (0.5000f) - (proportionOfWidth (0.2500f) / 2)), static_cast<float> (proportionOfHeight (0.5000f) - (proportionOfHeight (0.2500f) / 2)), static_cast<float> (proportionOfWidth (0.2500f)), static_cast<float> (proportionOfHeight (0.2500f)), 2.000f, 4.000f);
    //[/UserPaint]
}

void ModulationSource::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    initial_bounds_ = getBounds();
    //[/UserResized]
}

void ModulationSource::moved()
{
    //[UserCode_moved] -- Add your code here...
    //[/UserCode_moved]
}

void ModulationSource::mouseEnter (const MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
    is_hovered_ = true;
    //[/UserCode_mouseEnter]
}

void ModulationSource::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    is_hovered_ = false;
    //[/UserCode_mouseExit]
}

void ModulationSource::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    // dragger_.startDraggingComponent(this, e);
    DragAndDropContainer* drag_container = DragAndDropContainer::findParentDragContainerFor(this);
    if (!drag_container->isDragAndDropActive())
        drag_container->startDragging(description_, this);
    //[/UserCode_mouseDown]
}

void ModulationSource::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    // dragger_.dragComponent(this, e, nullptr);
    //[/UserCode_mouseDrag]
}

void ModulationSource::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    // if (getBounds() != initial_bounds_)
    //     setBounds(initial_bounds_);
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModulationSource" componentName=""
                 parentClasses="public Component, public DragAndDropContainer"
                 constructorParams="std::string name, Colour color" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="20" initialHeight="20">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="moved()"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseEnter (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="464646">
    <ROUNDRECT pos="50%c 50%c 25% 25%" cornerSize="2" fill="solid: 8b93ff" hasStroke="1"
               stroke="4, mitered, butt" strokeColour="solid: ffc8c8c8"/>
  </BACKGROUND>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
