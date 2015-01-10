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

#include "draggable_component.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
DraggableComponent::DraggableComponent ()
{

    //[UserPreSize]
    is_hovered_ = false;
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

DraggableComponent::~DraggableComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DraggableComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0xffc8c8c8));
    g.drawRoundedRectangle (static_cast<float> (proportionOfWidth (0.5000f) - (proportionOfWidth (0.2500f) / 2)), static_cast<float> (proportionOfHeight (0.5000f) - (proportionOfHeight (0.2500f) / 2)), static_cast<float> (proportionOfWidth (0.2500f)), static_cast<float> (proportionOfHeight (0.2500f)), 2.000f, 4.000f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DraggableComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DraggableComponent::moved()
{
    //[UserCode_moved] -- Add your code here...
    Point<int> center = getBoundsInParent().getCentre();
    Point<int> constrained_center = center;
    constrained_center.x = std::min(restrictions_.getX() + restrictions_.getWidth(), center.x);
    constrained_center.x = std::max(restrictions_.getX(), constrained_center.x);
    constrained_center.y = std::min(restrictions_.getY() + restrictions_.getHeight(), center.y);
    constrained_center.y = std::max(restrictions_.getY(), constrained_center.y);

    if (constrained_center != center)
        setCentrePosition(constrained_center.x, constrained_center.y);

    //[/UserCode_moved]
}

void DraggableComponent::mouseEnter (const MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
    is_hovered_ = true;
    // We will repaint the parent just in case the hovering changes something in the parent.
    Component* parent = getParentComponent();
    if (parent)
        parent->repaint();
    //[/UserCode_mouseEnter]
}

void DraggableComponent::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    is_hovered_ = false;
    // We will repaint the parent just in case the hovering changes something in the parent.
    Component* parent = getParentComponent();
    if (parent)
        parent->repaint();
    //[/UserCode_mouseExit]
}

void DraggableComponent::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    dragger_.startDraggingComponent(this, e);
    //[/UserCode_mouseDown]
}

void DraggableComponent::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    dragger_.dragComponent(this, e, nullptr);
    //[/UserCode_mouseDrag]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DraggableComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="moved()"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseEnter (const MouseEvent&amp; e)"/>
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
