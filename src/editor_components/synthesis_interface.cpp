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
#include <iomanip>
#include "value_change_manager.h"
//[/Headers]

#include "synthesis_interface.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SynthesisInterface::SynthesisInterface (mopo::control_map controls)
{
    addAndMakeVisible (step_sequencer_ = new GraphicalStepSequencer());
    addAndMakeVisible (amplitude_envelope_ = new GraphicalEnvelope());
    addAndMakeVisible (filter_envelope_ = new GraphicalEnvelope());
    addAndMakeVisible (osc_1_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (osc_2_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (polyphony_ = new Slider ("polyphony"));
    polyphony_->setRange (1, 32, 1);
    polyphony_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    polyphony_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    polyphony_->addListener (this);

    addAndMakeVisible (portamento_ = new Slider ("portamento"));
    portamento_->setRange (0, 0.2, 0);
    portamento_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    portamento_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    portamento_->addListener (this);

    addAndMakeVisible (pitch_bend_range_ = new Slider ("pitch bend range"));
    pitch_bend_range_->setRange (0, 48, 1);
    pitch_bend_range_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    pitch_bend_range_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    pitch_bend_range_->addListener (this);

    addAndMakeVisible (cross_modulation_ = new Slider ("cross modulation"));
    cross_modulation_->setRange (0, 10, 0);
    cross_modulation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    cross_modulation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    cross_modulation_->addListener (this);
    cross_modulation_->setSkewFactor (0.5);

    addAndMakeVisible (filter_response_ = new FilterResponse (128));
    addAndMakeVisible (legato_ = new Slider ("legato"));
    legato_->setRange (0, 1, 1);
    legato_->setSliderStyle (Slider::LinearHorizontal);
    legato_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    legato_->addListener (this);

    addAndMakeVisible (portamento_type_ = new Slider ("portamento type"));
    portamento_type_->setRange (0, 2, 1);
    portamento_type_->setSliderStyle (Slider::LinearHorizontal);
    portamento_type_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    portamento_type_->addListener (this);

    addAndMakeVisible (osc_mix_ = new Slider ("osc mix"));
    osc_mix_->setRange (0, 1, 0);
    osc_mix_->setSliderStyle (Slider::LinearBar);
    osc_mix_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_mix_->setColour (Slider::backgroundColourId, Colours::black);
    osc_mix_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    osc_mix_->addListener (this);

    addAndMakeVisible (osc_2_transpose_ = new Slider ("osc 2 transpose"));
    osc_2_transpose_->setRange (-48, 48, 1);
    osc_2_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_transpose_->addListener (this);

    addAndMakeVisible (osc_2_tune_ = new Slider ("osc 2 tune"));
    osc_2_tune_->setRange (-1, 1, 0);
    osc_2_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_tune_->addListener (this);

    addAndMakeVisible (volume_ = new Slider ("volume"));
    volume_->setRange (0, 1, 0);
    volume_->setSliderStyle (Slider::LinearHorizontal);
    volume_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volume_->addListener (this);
    volume_->setSkewFactor (0.3);

    addAndMakeVisible (delay_time_ = new Slider ("delay time"));
    delay_time_->setRange (0.01, 1, 0);
    delay_time_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_time_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_time_->addListener (this);
    delay_time_->setSkewFactor (2);

    addAndMakeVisible (delay_feedback_ = new Slider ("delay feedback"));
    delay_feedback_->setRange (-1, 1, 0);
    delay_feedback_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_feedback_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_feedback_->addListener (this);

    addAndMakeVisible (delay_dry_wet_ = new Slider ("delay dry wet"));
    delay_dry_wet_->setRange (0, 1, 0);
    delay_dry_wet_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_dry_wet_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_dry_wet_->addListener (this);

    addAndMakeVisible (velocity_track_ = new Slider ("velocity track"));
    velocity_track_->setRange (-1, 1, 0);
    velocity_track_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    velocity_track_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    velocity_track_->addListener (this);

    addAndMakeVisible (amp_attack_ = new Slider ("amp attack"));
    amp_attack_->setRange (0, 10, 0);
    amp_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_attack_->addListener (this);
    amp_attack_->setSkewFactor (0.3);

    addAndMakeVisible (amp_decay_ = new Slider ("amp decay"));
    amp_decay_->setRange (0, 10, 0);
    amp_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_decay_->addListener (this);
    amp_decay_->setSkewFactor (0.3);

    addAndMakeVisible (amp_release_ = new Slider ("amp release"));
    amp_release_->setRange (0, 10, 0);
    amp_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_release_->addListener (this);
    amp_release_->setSkewFactor (0.3);

    addAndMakeVisible (amp_sustain_ = new Slider ("amp sustain"));
    amp_sustain_->setRange (0, 1, 0);
    amp_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_sustain_->addListener (this);

    addAndMakeVisible (fil_attack_ = new Slider ("fil attack"));
    fil_attack_->setRange (0, 10, 0);
    fil_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_attack_->addListener (this);
    fil_attack_->setSkewFactor (0.3);

    addAndMakeVisible (fil_decay_ = new Slider ("fil decay"));
    fil_decay_->setRange (0, 10, 0);
    fil_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_decay_->addListener (this);
    fil_decay_->setSkewFactor (0.3);

    addAndMakeVisible (fil_release_ = new Slider ("fil release"));
    fil_release_->setRange (0, 10, 0);
    fil_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_release_->addListener (this);
    fil_release_->setSkewFactor (0.3);

    addAndMakeVisible (fil_sustain_ = new Slider ("fil sustain"));
    fil_sustain_->setRange (0, 1, 0);
    fil_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_sustain_->addListener (this);

    addAndMakeVisible (resonance_ = new Slider ("resonance"));
    resonance_->setRange (0, 1, 0);
    resonance_->setSliderStyle (Slider::LinearBar);
    resonance_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    resonance_->setColour (Slider::backgroundColourId, Colours::black);
    resonance_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    resonance_->addListener (this);

    addAndMakeVisible (filter_type_ = new Slider ("filter type"));
    filter_type_->setRange (0, 6, 1);
    filter_type_->setSliderStyle (Slider::LinearHorizontal);
    filter_type_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filter_type_->addListener (this);

    addAndMakeVisible (osc_1_waveform_ = new Slider ("osc 1 waveform"));
    osc_1_waveform_->setRange (0, 11, 1);
    osc_1_waveform_->setSliderStyle (Slider::LinearBar);
    osc_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc_1_waveform_->setColour (Slider::backgroundColourId, Colours::black);
    osc_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    osc_1_waveform_->addListener (this);

    addAndMakeVisible (osc_2_waveform_ = new Slider ("osc 2 waveform"));
    osc_2_waveform_->setRange (0, 11, 1);
    osc_2_waveform_->setSliderStyle (Slider::LinearBar);
    osc_2_waveform_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    osc_2_waveform_->setColour (Slider::backgroundColourId, Colours::black);
    osc_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
    osc_2_waveform_->addListener (this);

    addAndMakeVisible (cutoff_ = new Slider ("cutoff"));
    cutoff_->setRange (28, 127, 0);
    cutoff_->setSliderStyle (Slider::LinearBar);
    cutoff_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    cutoff_->setColour (Slider::backgroundColourId, Colours::black);
    cutoff_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    cutoff_->addListener (this);

    addAndMakeVisible (fil_env_depth_ = new Slider ("fil env depth"));
    fil_env_depth_->setRange (-128, 128, 0);
    fil_env_depth_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_env_depth_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_env_depth_->addListener (this);

    addAndMakeVisible (keytrack_ = new Slider ("keytrack"));
    keytrack_->setRange (-1, 1, 0);
    keytrack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    keytrack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    keytrack_->addListener (this);

    addAndMakeVisible (osc_feedback_transpose_ = new Slider ("osc feedback transpose"));
    osc_feedback_transpose_->setRange (-24, 24, 1);
    osc_feedback_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_transpose_->addListener (this);

    addAndMakeVisible (osc_feedback_amount_ = new Slider ("osc feedback amount"));
    osc_feedback_amount_->setRange (-1, 1, 0);
    osc_feedback_amount_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_amount_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_amount_->addListener (this);

    addAndMakeVisible (osc_feedback_tune_ = new Slider ("osc feedback tune"));
    osc_feedback_tune_->setRange (-1, 1, 0);
    osc_feedback_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_tune_->addListener (this);

    addAndMakeVisible (amplitude_env_mod_source_ = new ModulationSource ("amplitude env", Colour (0xff0000ff)));
    addAndMakeVisible (step_generator_mod_source_ = new ModulationSource ("step sequencer", Colour (0xff00ff00)));
    addAndMakeVisible (filter_env_mod_source_ = new ModulationSource ("filter env", Colour (0xffff0000)));
    addAndMakeVisible (cross_mod_destination_ = new ModulationDestination ("cross modulation"));
    addAndMakeVisible (pitch_mod_destination_ = new ModulationDestination ("pitch"));
    addAndMakeVisible (cutoff_mod_destination_ = new ModulationDestination ("cutoff"));
    addAndMakeVisible (lfo_1_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (lfo_2_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (lfo_1_waveform_ = new Slider ("lfo 1 waveform"));
    lfo_1_waveform_->setRange (0, 11, 1);
    lfo_1_waveform_->setSliderStyle (Slider::LinearBar);
    lfo_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    lfo_1_waveform_->setColour (Slider::backgroundColourId, Colours::black);
    lfo_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
    lfo_1_waveform_->addListener (this);

    addAndMakeVisible (lfo_2_waveform_ = new Slider ("lfo 2 waveform"));
    lfo_2_waveform_->setRange (0, 11, 1);
    lfo_2_waveform_->setSliderStyle (Slider::LinearBar);
    lfo_2_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    lfo_2_waveform_->setColour (Slider::backgroundColourId, Colours::black);
    lfo_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
    lfo_2_waveform_->addListener (this);

    addAndMakeVisible (lfo_1_mod_source_ = new ModulationSource ("lfo 1", Colour (0xffffff00)));
    addAndMakeVisible (lfo_2_mod_source_ = new ModulationSource ("lfo 2", Colour (0xffff00ff)));
    addAndMakeVisible (resonance_mod_destination_ = new ModulationDestination ("resonance"));
    addAndMakeVisible (num_steps_ = new Slider ("num steps"));
    num_steps_->setRange (1, 32, 1);
    num_steps_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    num_steps_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    num_steps_->addListener (this);

    addAndMakeVisible (step_frequency_ = new Slider ("step frequency"));
    step_frequency_->setRange (0, 20, 0);
    step_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    step_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    step_frequency_->addListener (this);
    step_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (lfo_1_frequency_ = new Slider ("lfo 1 frequency"));
    lfo_1_frequency_->setRange (0, 20, 0);
    lfo_1_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    lfo_1_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lfo_1_frequency_->addListener (this);
    lfo_1_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (lfo_2_frequency_ = new Slider ("lfo 2 frequency"));
    lfo_2_frequency_->setRange (0, 20, 0);
    lfo_2_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    lfo_2_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lfo_2_frequency_->addListener (this);
    lfo_2_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (osc_mix_mod_destination_ = new ModulationDestination ("osc mix"));
    addAndMakeVisible (filter_saturation_ = new Slider ("filter saturation"));
    filter_saturation_->setRange (1, 32, 0);
    filter_saturation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    filter_saturation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filter_saturation_->addListener (this);
    filter_saturation_->setSkewFactor (0.3);


    //[UserPreSize]
    resonance_->setSliderStyle(Slider::LinearBarVertical);
    osc_1_waveform_->setSliderStyle(Slider::LinearBarVertical);
    osc_2_waveform_->setSliderStyle(Slider::LinearBarVertical);
    lfo_1_waveform_->setSliderStyle(Slider::LinearBarVertical);
    lfo_2_waveform_->setSliderStyle(Slider::LinearBarVertical);
    osc_mix_->setSliderStyle(Slider::LinearBarVertical);

    amplitude_envelope_->setAttackSlider(amp_attack_);
    amplitude_envelope_->setDecaySlider(amp_decay_);
    amplitude_envelope_->setSustainSlider(amp_sustain_);
    amplitude_envelope_->setReleaseSlider(amp_release_);

    filter_envelope_->setAttackSlider(fil_attack_);
    filter_envelope_->setDecaySlider(fil_decay_);
    filter_envelope_->setSustainSlider(fil_sustain_);
    filter_envelope_->setReleaseSlider(fil_release_);

    osc_1_wave_display_->setWaveSlider(osc_1_waveform_);
    osc_2_wave_display_->setWaveSlider(osc_2_waveform_);
    lfo_1_wave_display_->setWaveSlider(lfo_1_waveform_);
    lfo_2_wave_display_->setWaveSlider(lfo_2_waveform_);

    filter_response_->setCutoffSlider(cutoff_);
    filter_response_->setResonanceSlider(resonance_);
    filter_response_->setFilterTypeSlider(filter_type_);

    for (int i = 0; i < num_steps_->getMaximum(); ++i) {
        String num(i);
        if (num.length() == 1)
            num = String("0") + num;

        Slider* step = new Slider(String("step seq ") + num);
        step->setRange(-1.0, 1.0);
        step->addListener(this);
        step_sequencer_sliders_.push_back(step);
        slider_lookup_[step->getName().toStdString()] = step;
    }
    step_sequencer_->setStepSliders(&step_sequencer_sliders_);
    step_sequencer_->setNumStepsSlider(num_steps_);

    for (int i = 0; i < getNumChildComponents(); ++i) {
        Slider* slider = dynamic_cast<Slider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;
    }

    setAllValues(controls);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SynthesisInterface::~SynthesisInterface()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    step_sequencer_ = nullptr;
    amplitude_envelope_ = nullptr;
    filter_envelope_ = nullptr;
    osc_1_wave_display_ = nullptr;
    osc_2_wave_display_ = nullptr;
    polyphony_ = nullptr;
    portamento_ = nullptr;
    pitch_bend_range_ = nullptr;
    cross_modulation_ = nullptr;
    filter_response_ = nullptr;
    legato_ = nullptr;
    portamento_type_ = nullptr;
    osc_mix_ = nullptr;
    osc_2_transpose_ = nullptr;
    osc_2_tune_ = nullptr;
    volume_ = nullptr;
    delay_time_ = nullptr;
    delay_feedback_ = nullptr;
    delay_dry_wet_ = nullptr;
    velocity_track_ = nullptr;
    amp_attack_ = nullptr;
    amp_decay_ = nullptr;
    amp_release_ = nullptr;
    amp_sustain_ = nullptr;
    fil_attack_ = nullptr;
    fil_decay_ = nullptr;
    fil_release_ = nullptr;
    fil_sustain_ = nullptr;
    resonance_ = nullptr;
    filter_type_ = nullptr;
    osc_1_waveform_ = nullptr;
    osc_2_waveform_ = nullptr;
    cutoff_ = nullptr;
    fil_env_depth_ = nullptr;
    keytrack_ = nullptr;
    osc_feedback_transpose_ = nullptr;
    osc_feedback_amount_ = nullptr;
    osc_feedback_tune_ = nullptr;
    amplitude_env_mod_source_ = nullptr;
    step_generator_mod_source_ = nullptr;
    filter_env_mod_source_ = nullptr;
    cross_mod_destination_ = nullptr;
    pitch_mod_destination_ = nullptr;
    cutoff_mod_destination_ = nullptr;
    lfo_1_wave_display_ = nullptr;
    lfo_2_wave_display_ = nullptr;
    lfo_1_waveform_ = nullptr;
    lfo_2_waveform_ = nullptr;
    lfo_1_mod_source_ = nullptr;
    lfo_2_mod_source_ = nullptr;
    resonance_mod_destination_ = nullptr;
    num_steps_ = nullptr;
    step_frequency_ = nullptr;
    lfo_1_frequency_ = nullptr;
    lfo_2_frequency_ = nullptr;
    osc_mix_mod_destination_ = nullptr;
    filter_saturation_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SynthesisInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff1f1f1f));

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colours::white);
    //[/UserPaint]
}

void SynthesisInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    step_sequencer_->setBounds (456, 296, 300, 100);
    amplitude_envelope_->setBounds (456, 152, 300, 100);
    filter_envelope_->setBounds (456, 8, 300, 100);
    osc_1_wave_display_->setBounds (32, 8, 240, 100);
    osc_2_wave_display_->setBounds (32, 112, 240, 100);
    polyphony_->setBounds (320, 496, 50, 50);
    portamento_->setBounds (320, 552, 50, 50);
    pitch_bend_range_->setBounds (472, 496, 50, 50);
    cross_modulation_->setBounds (328, 24, 50, 50);
    filter_response_->setBounds (32, 312, 300, 100);
    legato_->setBounds (384, 504, 64, 24);
    portamento_type_->setBounds (384, 560, 64, 24);
    osc_mix_->setBounds (272, 8, 20, 204);
    osc_2_transpose_->setBounds (328, 88, 50, 50);
    osc_2_tune_->setBounds (328, 152, 50, 50);
    volume_->setBounds (504, 560, 248, 24);
    delay_time_->setBounds (568, 488, 50, 50);
    delay_feedback_->setBounds (632, 488, 50, 50);
    delay_dry_wet_->setBounds (696, 488, 50, 50);
    velocity_track_->setBounds (720, 256, 32, 32);
    amp_attack_->setBounds (456, 256, 32, 32);
    amp_decay_->setBounds (496, 256, 32, 32);
    amp_release_->setBounds (576, 256, 32, 32);
    amp_sustain_->setBounds (536, 256, 32, 32);
    fil_attack_->setBounds (456, 112, 32, 32);
    fil_decay_->setBounds (496, 112, 32, 32);
    fil_release_->setBounds (576, 112, 32, 32);
    fil_sustain_->setBounds (536, 112, 32, 32);
    resonance_->setBounds (332, 312, 12, 100);
    filter_type_->setBounds (32, 296, 296, 16);
    osc_1_waveform_->setBounds (20, 8, 12, 100);
    osc_2_waveform_->setBounds (20, 112, 12, 100);
    cutoff_->setBounds (32, 412, 300, 12);
    fil_env_depth_->setBounds (680, 112, 32, 32);
    keytrack_->setBounds (720, 112, 32, 32);
    osc_feedback_transpose_->setBounds (64, 224, 50, 50);
    osc_feedback_amount_->setBounds (200, 224, 50, 50);
    osc_feedback_tune_->setBounds (128, 224, 50, 50);
    amplitude_env_mod_source_->setBounds (432, 184, 24, 24);
    step_generator_mod_source_->setBounds (432, 328, 24, 24);
    filter_env_mod_source_->setBounds (432, 40, 24, 24);
    cross_mod_destination_->setBounds (384, 40, 24, 24);
    pitch_mod_destination_->setBounds (384, 128, 24, 24);
    cutoff_mod_destination_->setBounds (168, 424, 24, 24);
    lfo_1_wave_display_->setBounds (56, 488, 140, 80);
    lfo_2_wave_display_->setBounds (56, 576, 140, 80);
    lfo_1_waveform_->setBounds (44, 488, 12, 80);
    lfo_2_waveform_->setBounds (44, 576, 12, 80);
    lfo_1_mod_source_->setBounds (208, 544, 24, 24);
    lfo_2_mod_source_->setBounds (208, 632, 24, 24);
    resonance_mod_destination_->setBounds (344, 328, 24, 24);
    num_steps_->setBounds (456, 400, 50, 50);
    step_frequency_->setBounds (512, 400, 50, 50);
    lfo_1_frequency_->setBounds (200, 488, 50, 50);
    lfo_2_frequency_->setBounds (200, 576, 50, 50);
    osc_mix_mod_destination_->setBounds (296, 96, 24, 24);
    filter_saturation_->setBounds (240, 424, 50, 50);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SynthesisInterface::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    std::string name = sliderThatWasMoved->getName().toStdString();
    ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
    parent->valueChanged(name, sliderThatWasMoved->getValue());
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == polyphony_)
    {
        //[UserSliderCode_polyphony_] -- add your slider handling code here..
        //[/UserSliderCode_polyphony_]
    }
    else if (sliderThatWasMoved == portamento_)
    {
        //[UserSliderCode_portamento_] -- add your slider handling code here..
        //[/UserSliderCode_portamento_]
    }
    else if (sliderThatWasMoved == pitch_bend_range_)
    {
        //[UserSliderCode_pitch_bend_range_] -- add your slider handling code here..
        //[/UserSliderCode_pitch_bend_range_]
    }
    else if (sliderThatWasMoved == cross_modulation_)
    {
        //[UserSliderCode_cross_modulation_] -- add your slider handling code here..
        //[/UserSliderCode_cross_modulation_]
    }
    else if (sliderThatWasMoved == legato_)
    {
        //[UserSliderCode_legato_] -- add your slider handling code here..
        //[/UserSliderCode_legato_]
    }
    else if (sliderThatWasMoved == portamento_type_)
    {
        //[UserSliderCode_portamento_type_] -- add your slider handling code here..
        //[/UserSliderCode_portamento_type_]
    }
    else if (sliderThatWasMoved == osc_mix_)
    {
        //[UserSliderCode_osc_mix_] -- add your slider handling code here..
        //[/UserSliderCode_osc_mix_]
    }
    else if (sliderThatWasMoved == osc_2_transpose_)
    {
        //[UserSliderCode_osc_2_transpose_] -- add your slider handling code here..
        //[/UserSliderCode_osc_2_transpose_]
    }
    else if (sliderThatWasMoved == osc_2_tune_)
    {
        //[UserSliderCode_osc_2_tune_] -- add your slider handling code here..
        //[/UserSliderCode_osc_2_tune_]
    }
    else if (sliderThatWasMoved == volume_)
    {
        //[UserSliderCode_volume_] -- add your slider handling code here..
        //[/UserSliderCode_volume_]
    }
    else if (sliderThatWasMoved == delay_time_)
    {
        //[UserSliderCode_delay_time_] -- add your slider handling code here..
        //[/UserSliderCode_delay_time_]
    }
    else if (sliderThatWasMoved == delay_feedback_)
    {
        //[UserSliderCode_delay_feedback_] -- add your slider handling code here..
        //[/UserSliderCode_delay_feedback_]
    }
    else if (sliderThatWasMoved == delay_dry_wet_)
    {
        //[UserSliderCode_delay_dry_wet_] -- add your slider handling code here..
        //[/UserSliderCode_delay_dry_wet_]
    }
    else if (sliderThatWasMoved == velocity_track_)
    {
        //[UserSliderCode_velocity_track_] -- add your slider handling code here..
        //[/UserSliderCode_velocity_track_]
    }
    else if (sliderThatWasMoved == amp_attack_)
    {
        //[UserSliderCode_amp_attack_] -- add your slider handling code here..
        //[/UserSliderCode_amp_attack_]
    }
    else if (sliderThatWasMoved == amp_decay_)
    {
        //[UserSliderCode_amp_decay_] -- add your slider handling code here..
        //[/UserSliderCode_amp_decay_]
    }
    else if (sliderThatWasMoved == amp_release_)
    {
        //[UserSliderCode_amp_release_] -- add your slider handling code here..
        //[/UserSliderCode_amp_release_]
    }
    else if (sliderThatWasMoved == amp_sustain_)
    {
        //[UserSliderCode_amp_sustain_] -- add your slider handling code here..
        //[/UserSliderCode_amp_sustain_]
    }
    else if (sliderThatWasMoved == fil_attack_)
    {
        //[UserSliderCode_fil_attack_] -- add your slider handling code here..
        //[/UserSliderCode_fil_attack_]
    }
    else if (sliderThatWasMoved == fil_decay_)
    {
        //[UserSliderCode_fil_decay_] -- add your slider handling code here..
        //[/UserSliderCode_fil_decay_]
    }
    else if (sliderThatWasMoved == fil_release_)
    {
        //[UserSliderCode_fil_release_] -- add your slider handling code here..
        //[/UserSliderCode_fil_release_]
    }
    else if (sliderThatWasMoved == fil_sustain_)
    {
        //[UserSliderCode_fil_sustain_] -- add your slider handling code here..
        //[/UserSliderCode_fil_sustain_]
    }
    else if (sliderThatWasMoved == resonance_)
    {
        //[UserSliderCode_resonance_] -- add your slider handling code here..
        //[/UserSliderCode_resonance_]
    }
    else if (sliderThatWasMoved == filter_type_)
    {
        //[UserSliderCode_filter_type_] -- add your slider handling code here..
        //[/UserSliderCode_filter_type_]
    }
    else if (sliderThatWasMoved == osc_1_waveform_)
    {
        //[UserSliderCode_osc_1_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_osc_1_waveform_]
    }
    else if (sliderThatWasMoved == osc_2_waveform_)
    {
        //[UserSliderCode_osc_2_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_osc_2_waveform_]
    }
    else if (sliderThatWasMoved == cutoff_)
    {
        //[UserSliderCode_cutoff_] -- add your slider handling code here..
        //[/UserSliderCode_cutoff_]
    }
    else if (sliderThatWasMoved == fil_env_depth_)
    {
        //[UserSliderCode_fil_env_depth_] -- add your slider handling code here..
        //[/UserSliderCode_fil_env_depth_]
    }
    else if (sliderThatWasMoved == keytrack_)
    {
        //[UserSliderCode_keytrack_] -- add your slider handling code here..
        //[/UserSliderCode_keytrack_]
    }
    else if (sliderThatWasMoved == osc_feedback_transpose_)
    {
        //[UserSliderCode_osc_feedback_transpose_] -- add your slider handling code here..
        //[/UserSliderCode_osc_feedback_transpose_]
    }
    else if (sliderThatWasMoved == osc_feedback_amount_)
    {
        //[UserSliderCode_osc_feedback_amount_] -- add your slider handling code here..
        //[/UserSliderCode_osc_feedback_amount_]
    }
    else if (sliderThatWasMoved == osc_feedback_tune_)
    {
        //[UserSliderCode_osc_feedback_tune_] -- add your slider handling code here..
        //[/UserSliderCode_osc_feedback_tune_]
    }
    else if (sliderThatWasMoved == lfo_1_waveform_)
    {
        //[UserSliderCode_lfo_1_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_lfo_1_waveform_]
    }
    else if (sliderThatWasMoved == lfo_2_waveform_)
    {
        //[UserSliderCode_lfo_2_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_lfo_2_waveform_]
    }
    else if (sliderThatWasMoved == num_steps_)
    {
        //[UserSliderCode_num_steps_] -- add your slider handling code here..
        //[/UserSliderCode_num_steps_]
    }
    else if (sliderThatWasMoved == step_frequency_)
    {
        //[UserSliderCode_step_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_step_frequency_]
    }
    else if (sliderThatWasMoved == lfo_1_frequency_)
    {
        //[UserSliderCode_lfo_1_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_lfo_1_frequency_]
    }
    else if (sliderThatWasMoved == lfo_2_frequency_)
    {
        //[UserSliderCode_lfo_2_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_lfo_2_frequency_]
    }
    else if (sliderThatWasMoved == filter_saturation_)
    {
        //[UserSliderCode_filter_saturation_] -- add your slider handling code here..
        //[/UserSliderCode_filter_saturation_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SynthesisInterface::setAllValues(mopo::control_map& controls) {
    std::map<std::string, Slider*>::iterator iter = slider_lookup_.begin();
    for (; iter != slider_lookup_.end(); ++iter)
        iter->second->setValue(controls[iter->first]->value());
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SynthesisInterface" componentName=""
                 parentClasses="public Component" constructorParams="mopo::control_map controls"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff1f1f1f"/>
  <JUCERCOMP name="" id="83a23936a8f464b5" memberName="step_sequencer_" virtualName="GraphicalStepSequencer"
             explicitFocusOrder="0" pos="456 296 300 100" sourceFile="graphical_step_sequencer.cpp"
             constructorParams=""/>
  <JUCERCOMP name="" id="b4880edb8b39ec9d" memberName="amplitude_envelope_"
             virtualName="GraphicalEnvelope" explicitFocusOrder="0" pos="456 152 300 100"
             sourceFile="graphical_envelope.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="87feb60c88df4fcc" memberName="filter_envelope_" virtualName="GraphicalEnvelope"
             explicitFocusOrder="0" pos="456 8 300 100" sourceFile="graphical_envelope.cpp"
             constructorParams=""/>
  <JUCERCOMP name="osc 1 wave display" id="55100715382ea344" memberName="osc_1_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="32 8 240 100"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <JUCERCOMP name="osc 2 wave display" id="c0c3e4a3ab2f045f" memberName="osc_2_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="32 112 240 100"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="polyphony" id="952bde38857bdba7" memberName="polyphony_"
          virtualName="" explicitFocusOrder="0" pos="320 496 50 50" min="1"
          max="32" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="portamento" id="9de85cc1c5f64eaa" memberName="portamento_"
          virtualName="" explicitFocusOrder="0" pos="320 552 50 50" min="0"
          max="0.2000000000000000111" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitch bend range" id="e53afc6d1a04c708" memberName="pitch_bend_range_"
          virtualName="" explicitFocusOrder="0" pos="472 496 50 50" min="0"
          max="48" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cross modulation" id="1d3e4b59d6e470fb" memberName="cross_modulation_"
          virtualName="" explicitFocusOrder="0" pos="328 24 50 50" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"/>
  <JUCERCOMP name="" id="e5ebb41c4c259ce1" memberName="filter_response_" virtualName="FilterResponse"
             explicitFocusOrder="0" pos="32 312 300 100" sourceFile="filter_response.cpp"
             constructorParams="128"/>
  <SLIDER name="legato" id="bae65942d016a0ea" memberName="legato_" virtualName=""
          explicitFocusOrder="0" pos="384 504 64 24" min="0" max="1" int="1"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="portamento type" id="909956998c46045e" memberName="portamento_type_"
          virtualName="" explicitFocusOrder="0" pos="384 560 64 24" min="0"
          max="2" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc mix" id="211f463b59b2454f" memberName="osc_mix_" virtualName=""
          explicitFocusOrder="0" pos="272 8 20 204" bkgcol="ff000000" textboxoutline="808080"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc 2 transpose" id="555c8ee21acbf804" memberName="osc_2_transpose_"
          virtualName="" explicitFocusOrder="0" pos="328 88 50 50" min="-48"
          max="48" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc 2 tune" id="a8bc3bcffe7146f" memberName="osc_2_tune_"
          virtualName="" explicitFocusOrder="0" pos="328 152 50 50" min="-1"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="volume" id="7cc7edfbfc537ee7" memberName="volume_" virtualName=""
          explicitFocusOrder="0" pos="504 560 248 24" min="0" max="1" int="0"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="delay time" id="1c4c7f1a27492c1d" memberName="delay_time_"
          virtualName="" explicitFocusOrder="0" pos="568 488 50 50" min="0.010000000000000000208"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="2"/>
  <SLIDER name="delay feedback" id="c89eb62eea2ab491" memberName="delay_feedback_"
          virtualName="" explicitFocusOrder="0" pos="632 488 50 50" min="-1"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay dry wet" id="dbc9d35179b5bac7" memberName="delay_dry_wet_"
          virtualName="" explicitFocusOrder="0" pos="696 488 50 50" min="0"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="velocity track" id="113bdc65c4c0f18f" memberName="velocity_track_"
          virtualName="" explicitFocusOrder="0" pos="720 256 32 32" min="-1"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp attack" id="f6cda312a5619a33" memberName="amp_attack_"
          virtualName="" explicitFocusOrder="0" pos="456 256 32 32" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="amp decay" id="6de7c8b3a5a4826d" memberName="amp_decay_"
          virtualName="" explicitFocusOrder="0" pos="496 256 32 32" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="amp release" id="86b41ada65ec49c0" memberName="amp_release_"
          virtualName="" explicitFocusOrder="0" pos="576 256 32 32" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="amp sustain" id="3853f65a726f763" memberName="amp_sustain_"
          virtualName="" explicitFocusOrder="0" pos="536 256 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil attack" id="bc169bc84bd26782" memberName="fil_attack_"
          virtualName="" explicitFocusOrder="0" pos="456 112 32 32" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="fil decay" id="f5b3d1ec7fc18e1" memberName="fil_decay_"
          virtualName="" explicitFocusOrder="0" pos="496 112 32 32" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="fil release" id="c878127a7ada93f0" memberName="fil_release_"
          virtualName="" explicitFocusOrder="0" pos="576 112 32 32" min="0"
          max="10" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="fil sustain" id="5b25f915f3694f34" memberName="fil_sustain_"
          virtualName="" explicitFocusOrder="0" pos="536 112 32 32" min="0"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="resonance" id="6c1c82a2d59d4b6e" memberName="resonance_"
          virtualName="" explicitFocusOrder="0" pos="332 312 12 100" bkgcol="ff000000"
          textboxoutline="808080" min="0" max="1" int="0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="filter type" id="30ae8dead7514514" memberName="filter_type_"
          virtualName="" explicitFocusOrder="0" pos="32 296 296 16" min="0"
          max="6" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc 1 waveform" id="ea97519d003b4224" memberName="osc_1_waveform_"
          virtualName="" explicitFocusOrder="0" pos="20 8 12 100" bkgcol="ff000000"
          textboxoutline="808080" min="0" max="11" int="1" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="0" textBoxHeight="0"
          skewFactor="1"/>
  <SLIDER name="osc 2 waveform" id="a38d2af584df969a" memberName="osc_2_waveform_"
          virtualName="" explicitFocusOrder="0" pos="20 112 12 100" bkgcol="ff000000"
          textboxoutline="0" min="0" max="11" int="1" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cutoff" id="4ccde767164ea675" memberName="cutoff_" virtualName=""
          explicitFocusOrder="0" pos="32 412 300 12" bkgcol="ff000000"
          textboxoutline="808080" min="28" max="127" int="0" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil env depth" id="ac3a5967de6a1a92" memberName="fil_env_depth_"
          virtualName="" explicitFocusOrder="0" pos="680 112 32 32" min="-128"
          max="128" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="keytrack" id="33bbd8b71aa721c1" memberName="keytrack_"
          virtualName="" explicitFocusOrder="0" pos="720 112 32 32" min="-1"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc feedback transpose" id="e747becfc7a4f3f7" memberName="osc_feedback_transpose_"
          virtualName="" explicitFocusOrder="0" pos="64 224 50 50" min="-24"
          max="24" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc feedback amount" id="a94cee38c880759c" memberName="osc_feedback_amount_"
          virtualName="" explicitFocusOrder="0" pos="200 224 50 50" min="-1"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc feedback tune" id="90dc1d31f03abf4e" memberName="osc_feedback_tune_"
          virtualName="" explicitFocusOrder="0" pos="128 224 50 50" min="-1"
          max="1" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="amplitude env" id="49107c44dc2f49e9" memberName="amplitude_env_mod_source_"
             virtualName="" explicitFocusOrder="0" pos="432 184 24 24" sourceFile="modulation_source.cpp"
             constructorParams="&quot;amplitude env&quot;, Colour (0xff0000ff)"/>
  <JUCERCOMP name="step sequencer" id="539abd48959288ef" memberName="step_generator_mod_source_"
             virtualName="" explicitFocusOrder="0" pos="432 328 24 24" sourceFile="modulation_source.cpp"
             constructorParams="&quot;step sequencer&quot;, Colour (0xff00ff00)"/>
  <JUCERCOMP name="filter env" id="36ddd25be02a4fe1" memberName="filter_env_mod_source_"
             virtualName="" explicitFocusOrder="0" pos="432 40 24 24" sourceFile="modulation_source.cpp"
             constructorParams="&quot;filter env&quot;, Colour (0xffff0000)"/>
  <JUCERCOMP name="cross modulation" id="82ee43b8fbcceb78" memberName="cross_mod_destination_"
             virtualName="" explicitFocusOrder="0" pos="384 40 24 24" sourceFile="modulation_destination.cpp"
             constructorParams="&quot;cross modulation&quot;"/>
  <JUCERCOMP name="pitch" id="146986590a708b08" memberName="pitch_mod_destination_"
             virtualName="" explicitFocusOrder="0" pos="384 128 24 24" sourceFile="modulation_destination.cpp"
             constructorParams="&quot;pitch&quot;"/>
  <JUCERCOMP name="cutoff" id="9fcdd6545a5e9cd2" memberName="cutoff_mod_destination_"
             virtualName="" explicitFocusOrder="0" pos="168 424 24 24" sourceFile="modulation_destination.cpp"
             constructorParams="&quot;cutoff&quot;"/>
  <JUCERCOMP name="lfo 1 wave display" id="24d32b65108fb2a5" memberName="lfo_1_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="56 488 140 80"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <JUCERCOMP name="lfo 2 wave display" id="fec9561bdacebdbe" memberName="lfo_2_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="56 576 140 80"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="lfo 1 waveform" id="4ed06bb2c6901afe" memberName="lfo_1_waveform_"
          virtualName="" explicitFocusOrder="0" pos="44 488 12 80" bkgcol="ff000000"
          textboxoutline="808080" min="0" max="11" int="1" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="0" textBoxHeight="0"
          skewFactor="1"/>
  <SLIDER name="lfo 2 waveform" id="72004168ec47b7e7" memberName="lfo_2_waveform_"
          virtualName="" explicitFocusOrder="0" pos="44 576 12 80" bkgcol="ff000000"
          textboxoutline="0" min="0" max="11" int="1" style="LinearBar"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="0" textBoxHeight="0"
          skewFactor="1"/>
  <JUCERCOMP name="lfo 1" id="9571e0e9ba0fa53a" memberName="lfo_1_mod_source_"
             virtualName="" explicitFocusOrder="0" pos="208 544 24 24" sourceFile="modulation_source.cpp"
             constructorParams="&quot;lfo 1&quot;, Colour (0xffffff00)"/>
  <JUCERCOMP name="lfo 2" id="57b28b867a25f366" memberName="lfo_2_mod_source_"
             virtualName="" explicitFocusOrder="0" pos="208 632 24 24" sourceFile="modulation_source.cpp"
             constructorParams="&quot;lfo 2&quot;, Colour (0xffff00ff)"/>
  <JUCERCOMP name="resonance" id="5bac7839db359a73" memberName="resonance_mod_destination_"
             virtualName="" explicitFocusOrder="0" pos="344 328 24 24" sourceFile="modulation_destination.cpp"
             constructorParams="&quot;resonance&quot;"/>
  <SLIDER name="num steps" id="8be29885961d7617" memberName="num_steps_"
          virtualName="" explicitFocusOrder="0" pos="456 400 50 50" min="1"
          max="32" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="step frequency" id="788574c1265fb47" memberName="step_frequency_"
          virtualName="" explicitFocusOrder="0" pos="512 400 50 50" min="0"
          max="20" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="lfo 1 frequency" id="ebc4a57528113c39" memberName="lfo_1_frequency_"
          virtualName="" explicitFocusOrder="0" pos="200 488 50 50" min="0"
          max="20" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="lfo 2 frequency" id="41dc8f7fbc0e8586" memberName="lfo_2_frequency_"
          virtualName="" explicitFocusOrder="0" pos="200 576 50 50" min="0"
          max="20" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"/>
  <JUCERCOMP name="osc mix" id="45625f8553a02da7" memberName="osc_mix_mod_destination_"
             virtualName="" explicitFocusOrder="0" pos="296 96 24 24" sourceFile="modulation_destination.cpp"
             constructorParams="&quot;osc mix&quot;"/>
  <SLIDER name="filter saturation" id="b5014a266e860882" memberName="filter_saturation_"
          virtualName="" explicitFocusOrder="0" pos="240 424 50 50" min="1"
          max="32" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
