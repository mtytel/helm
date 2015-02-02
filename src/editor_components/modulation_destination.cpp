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
#include "twytch_common.h"
//[/Headers]

#include "modulation_destination.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ModulationDestination::ModulationDestination (std::string name)
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    for (int i = 0; i < mopo::MAX_MODULATION_CONNECTIONS; ++i) {
        connections_.push_back(nullptr);
        connection_colors_.push_back(Colours::black);
        modulation_sliders_.push_back(nullptr);
    }
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
    g.setColour(connection_colors_[0]);
    g.fillRect(3, 3, proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);

    g.setColour(connection_colors_[1]);
    g.fillRect(proportionOfWidth(0.5f), 3,
               proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);

    g.setColour(connection_colors_[2]);
    g.fillRect(3, proportionOfHeight(0.5f),
               proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);

    g.setColour(connection_colors_[3]);
    g.fillRect(proportionOfWidth(0.5f), proportionOfHeight(0.5f),
               proportionOfWidth(0.5f) - 3, proportionOfHeight(0.5f) - 3);
    //[/UserPaint]
}

void ModulationDestination::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ModulationDestination::mouseMove (const MouseEvent& e)
{
    //[UserCode_mouseMove] -- Add your code here...

    //[/UserCode_mouseMove]
}

void ModulationDestination::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
    /*
    Component* parent = getParentComponent();
    parent->removeChildComponent(modulation_slider_);
    delete modulation_slider_;
    modulation_slider_ = nullptr;
     */
    //[/UserCode_mouseExit]
}

void ModulationDestination::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    if (e.mods.isRightButtonDown()) {
        int index = getHoverIndex(e.getPosition());
        if (connections_[index]) {
            ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
            parent->disconnectModulation(connections_[index]);
            connections_[index] = nullptr;
            connection_colors_[index] = Colours::black;

            if (modulation_sliders_[index])
                removeModulationSlider(modulation_sliders_[index]);
        }
        repaint();
    }
    else {
        Component* parent = getParentComponent();
        int index = getHoverIndex(e.getPosition());

        if (connections_[index] && modulation_sliders_[index] == nullptr) {
            HoverSlider* slider = new HoverSlider(this);
            modulation_sliders_[index] = slider;
            parent->addAndMakeVisible(slider);

            slider->setRange(-1, 1, 0);
            slider->setSliderStyle(Slider::LinearBarVertical);
            slider->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
            slider->setColour(Slider::backgroundColourId, Colours::black);
            slider->setColour(Slider::trackColourId, connection_colors_[index]);
            slider->addListener(this);

            Rectangle<int> bounds = getBounds();
            if (index % 2)
                bounds.setX(bounds.getX() + bounds.getWidth());
            else
                bounds.setX(bounds.getX() - bounds.getWidth());
            if (index >= 2)
                bounds.setY(bounds.getY() + bounds.getHeight() / 2.0);
            else
                bounds.setY(bounds.getY() - bounds.getHeight() / 2.0);
            slider->setBounds(bounds);
        }
    }
    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ModulationDestination::removeModulationSlider(HoverSlider* slider) {
    for (int i = 0; i < modulation_sliders_.size(); ++i) {
        if (modulation_sliders_[i] == slider) {
            getParentComponent()->removeChildComponent(slider);
            delete slider;
            modulation_sliders_[i] = nullptr;
        }
    }
}

bool ModulationDestination::addConnection(mopo::ModulationConnection* connection,
                                          juce::Colour source_color) {
    int index = getNextAvailableIndex();
    if (index < 0)
        return false;

    connections_[index] = connection;
    connection_colors_[index] = source_color;
    repaint();
    return true;
}

void ModulationDestination::clearConnections() {
    for (int i = 0; i < mopo::MAX_MODULATION_CONNECTIONS; ++i) {
        connections_[i] = nullptr;
        connection_colors_[i] = Colours::black;
    }
}

void ModulationDestination::connectToSource(std::string source_name, juce::Colour source_color) {
    std::string name = getName().toStdString();
    mopo::ModulationConnection* connection = new mopo::ModulationConnection(source_name, name);
    connection->amount.set(0.5);
    ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
    parent->connectModulation(connection);

    addConnection(connection, source_color);
}

void ModulationDestination::itemDropped(const SourceDetails &drag_source) {
    DynamicObject* source = drag_source.description.getDynamicObject();
    std::string source_name = source->getProperty("name").toString().toStdString();
    Colour source_color = Colour((int)source->getProperty("color"));
    connectToSource(source_name, source_color);
}

bool ModulationDestination::isInterestedInDragSource(const SourceDetails &drag_source) {
    return getNextAvailableIndex() >= 0;
}

int ModulationDestination::getNextAvailableIndex() {
    int index = 0;
    while (connections_[index] && index < mopo::MAX_MODULATION_CONNECTIONS)
        index++;
    if (index >= mopo::MAX_MODULATION_CONNECTIONS)
        return -1;
    return index;
}

int ModulationDestination::getHoverIndex(Point<int> position) {
    int index = 0;
    if (position.x > proportionOfWidth(0.5f))
        index += 1;
    if (position.y > proportionOfWidth(0.5f))
        index += 2;
    return index;
}

void ModulationDestination::sliderValueChanged(Slider *slider) {
    for (int i = 0; i < modulation_sliders_.size(); ++i) {
        if (modulation_sliders_[i] == slider && connections_[i])
            connections_[i]->amount.set(slider->getValue());
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModulationDestination" componentName=""
                 parentClasses="public Component, public DragAndDropTarget, public SliderListener"
                 constructorParams="std::string name" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
    <METHOD name="mouseMove (const MouseEvent&amp; e)"/>
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
