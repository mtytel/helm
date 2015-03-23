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
#include "synth_gui_interface.h"
#include "twytch_common.h"
//[/Headers]

#include "modulation_manager.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#define FRAMES_PER_SECOND 60

//[/MiscUserDefs]

//==============================================================================
ModulationManager::ModulationManager (mopo::output_map modulation_sources, std::map<std::string, TwytchSlider*> sliders, mopo::output_map mono_modulations, mopo::output_map poly_modulations)
{

    //[UserPreSize]
    modulation_sources_ = modulation_sources;
    setInterceptsMouseClicks(false, true);
    startTimerHz(FRAMES_PER_SECOND);

    current_modulator_ = "";

    polyphonic_destinations_ = new Component();
    addAndMakeVisible(polyphonic_destinations_);
    polyphonic_destinations_->setInterceptsMouseClicks(false, true);

    monophonic_destinations_ = new Component();
    addAndMakeVisible(monophonic_destinations_);
    monophonic_destinations_->setInterceptsMouseClicks(false, true);

    slider_model_lookup_ = sliders;
    for (auto slider : slider_model_lookup_) {
        std::string name = slider.first;
        const mopo::Processor::Output* mono_total = mono_modulations[name];
        const mopo::Processor::Output* poly_total = poly_modulations[name];

        // Create modulation slider.
        ModulationSlider* mod_slider = new ModulationSlider(slider.second);
        mod_slider->setLookAndFeel(&look_and_feel_);
        mod_slider->addListener(this);
        if (poly_total)
            polyphonic_destinations_->addAndMakeVisible(mod_slider);
        else
            monophonic_destinations_->addAndMakeVisible(mod_slider);

        slider_lookup_[name] = mod_slider;
        owned_sliders_.push_back(mod_slider);

        // Create modulation meter.
        if (mono_total) {
            std::string name = slider.second->getName().toStdString();
            ModulationMeter* meter = new ModulationMeter(mono_total, poly_total, slider.second);
            addAndMakeVisible(meter);
            meter_lookup_[name] = meter;
            meter->setName(name);
            meter->setBounds(slider.second->getBounds());
            meters_.push_back(meter);
        }
    }
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
    polyphonic_destinations_->setBounds(getBounds());
    monophonic_destinations_->setBounds(getBounds());
    for (auto slider : slider_lookup_) {
        TwytchSlider* model = slider_model_lookup_[slider.first];
        Point<int> global_top_left = model->localPointToGlobal(Point<int>(0, 0));
        slider.second->setBounds(global_top_left.x, global_top_left.y,
                                 model->getWidth(), model->getHeight());
    }

    for (ModulationMeter* meter : meters_) {
        meter->setBounds(slider_model_lookup_[meter->getName().toStdString()]->getBounds());
        meter->setVisible(slider_model_lookup_[meter->getName().toStdString()]->isVisible());
    }
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ModulationManager::timerCallback() {
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent == nullptr)
        return;

    int num_voices = parent->getNumActiveVoices();

    for (ModulationMeter* meter : meters_) {
        meter->setVisible(slider_model_lookup_[meter->getName().toStdString()]->isVisible());
        meter->update(num_voices);
    }
}

void ModulationManager::sliderValueChanged(Slider *slider) {
    std::string destination_name = slider->getName().toStdString();

    if (slider->getValue() == 0.0f)
        clearModulation(current_modulator_, destination_name);
    else
        setModulationAmount(current_modulator_, destination_name, slider->getValue());
}

void ModulationManager::setModulationAmount(std::string source, std::string destination, mopo::mopo_float amount) {
    if (connections_[source].count(destination) == 0) {
        mopo::ModulationConnection* connection = new mopo::ModulationConnection(current_modulator_, destination);
        connections_[source][destination] = connection;

        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        parent->connectModulation(connection);
    }

    connections_[source][destination]->amount.set(amount);
}

void ModulationManager::clearModulation(std::string source, std::string destination) {
    if (connections_[source].count(destination)) {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        parent->disconnectModulation(connections_[source][destination]);
        connections_[source].erase(destination);
    }
}

void ModulationManager::changeModulator(std::string new_modulator) {
    if (new_modulator == current_modulator_ || new_modulator == "") {
        polyphonic_destinations_->setVisible(false);
        monophonic_destinations_->setVisible(false);
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

        polyphonic_destinations_->setVisible(true);
        bool source_is_poly = modulation_sources_[current_modulator_]->owner->isPolyphonic();
        monophonic_destinations_->setVisible(!source_is_poly);
    }
}

void ModulationManager::showMeter(std::string name, bool show) {
    meter_lookup_[name]->setVisible(show);
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
                 parentClasses="public Component, public SliderListener, public Timer"
                 constructorParams="mopo::output_map modulation_sources, std::map&lt;std::string, TwytchSlider*&gt; sliders, mopo::output_map mono_modulations, mopo::output_map poly_modulations"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
