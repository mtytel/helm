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
#include "modulation_slider.h"
#include "value_change_manager.h"
#include "twytch_common.h"
//[/Headers]

#include "modulation_manager.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ModulationManager::ModulationManager ()
{

    //[UserPreSize]
    current_modulator_ = "";
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ModulationManager::~ModulationManager()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ModulationManager::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ModulationManager::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    for (auto slider : slider_lookup_) {
        Slider* model = slider_model_lookup_[slider.first];
        Point<int> global_top_left = model->localPointToGlobal(Point<int>(0, 0));
        slider.second->setBounds(global_top_left.x, global_top_left.y,
                                 model->getWidth(), model->getHeight());
    }
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ModulationManager::sliderValueChanged(Slider *slider) {
    std::string destination_name = slider->getName().toStdString();

    if (slider->getValue() == 0.0f)
        clearModulation(current_modulator_, destination_name);
    else
        setModulationAmount(current_modulator_, destination_name, slider->getValue());
}

void ModulationManager::setModulationAmount(std::string source, std::string destination, mopo::mopo_float amount) {
    if (connections_[source].count(destination) == 0) {
        mopo::ModulationConnection* connection =
        new mopo::ModulationConnection(current_modulator_, destination);
        connections_[source][destination] = connection;

        ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
        parent->connectModulation(connection);
    }

    connections_[source][destination]->amount.set(amount);
}

void ModulationManager::clearModulation(std::string source, std::string destination) {
    if (connections_[source].count(destination)) {
        ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
        parent->disconnectModulation(connections_[source][destination]);
        connections_[source].erase(destination);
    }
}

void ModulationManager::changeModulator(std::string new_modulator) {
    if (new_modulator == current_modulator_ || new_modulator == "") {
        setVisible(false);
        current_modulator_ = "";
    }
    else {
        current_modulator_ = new_modulator;
        for (auto slider : slider_lookup_) {
            std::string destination_name = slider.second->getName().toStdString();
            float value = 0.0f;
            if (connections_[current_modulator_].count(destination_name))
                value = connections_[current_modulator_][destination_name]->amount.value();
            slider.second->setValue(value);
        }

        setVisible(true);
    }
}

void ModulationManager::createModulationSlider(Slider* destination) {
    std::string name = destination->getName().toStdString();
    slider_model_lookup_[name] = destination;

    ModulationSlider* mod_slider = new ModulationSlider(destination);
    mod_slider->setLookAndFeel(&look_and_feel_);
    mod_slider->addListener(this);
    addAndMakeVisible(mod_slider);

    slider_lookup_[name] = mod_slider;
    owned_sliders_.push_back(mod_slider);
}

void ModulationManager::clearModulationConnections() {
    changeModulator("");
    for (auto source : connections_)
        source.second.clear();
}

void ModulationManager::setModulationConnections(std::set<mopo::ModulationConnection*> connections) {
    clearModulationConnections();
    for (mopo::ModulationConnection* connection : connections)
        connections_[connection->source][connection->destination] = connection;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ModulationManager" componentName=""
                 parentClasses="public Component, public SliderListener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
