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
#include "full_interface.h"
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
    addAndMakeVisible (osc_1_wave_display_ = new WaveFormSelector (256));
    addAndMakeVisible (osc_2_wave_display_ = new WaveFormSelector (256));
    addAndMakeVisible (polyphony_ = new Slider ("polyphony"));
    polyphony_->setRange (1, 32, 1);
    polyphony_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    polyphony_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    polyphony_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    polyphony_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    polyphony_->addListener (this);

    addAndMakeVisible (portamento_ = new Slider ("portamento"));
    portamento_->setRange (0, 0.4, 0);
    portamento_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    portamento_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    portamento_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    portamento_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    portamento_->addListener (this);
    portamento_->setSkewFactor (0.5);

    addAndMakeVisible (pitch_bend_range_ = new Slider ("pitch bend range"));
    pitch_bend_range_->setRange (0, 48, 1);
    pitch_bend_range_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    pitch_bend_range_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    pitch_bend_range_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    pitch_bend_range_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    pitch_bend_range_->addListener (this);

    addAndMakeVisible (cross_modulation_ = new Slider ("cross modulation"));
    cross_modulation_->setRange (0, 1, 0);
    cross_modulation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    cross_modulation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    cross_modulation_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    cross_modulation_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    cross_modulation_->addListener (this);

    addAndMakeVisible (filter_response_ = new FilterResponse (300));
    addAndMakeVisible (portamento_type_ = new Slider ("portamento type"));
    portamento_type_->setRange (0, 2, 1);
    portamento_type_->setSliderStyle (Slider::LinearBar);
    portamento_type_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    portamento_type_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    portamento_type_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    portamento_type_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    portamento_type_->addListener (this);

    addAndMakeVisible (osc_mix_ = new Slider ("osc mix"));
    osc_mix_->setRange (0, 1, 0);
    osc_mix_->setSliderStyle (Slider::LinearBar);
    osc_mix_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_mix_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    osc_mix_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_mix_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    osc_mix_->addListener (this);

    addAndMakeVisible (osc_2_transpose_ = new Slider ("osc 2 transpose"));
    osc_2_transpose_->setRange (-48, 48, 1);
    osc_2_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_2_transpose_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_2_transpose_->addListener (this);

    addAndMakeVisible (osc_2_tune_ = new Slider ("osc 2 tune"));
    osc_2_tune_->setRange (-1, 1, 0);
    osc_2_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_2_tune_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_2_tune_->addListener (this);

    addAndMakeVisible (volume_ = new Slider ("volume"));
    volume_->setRange (0, 1, 0);
    volume_->setSliderStyle (Slider::LinearBar);
    volume_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volume_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    volume_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    volume_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    volume_->addListener (this);
    volume_->setSkewFactor (0.3);

    addAndMakeVisible (delay_time_ = new Slider ("delay time"));
    delay_time_->setRange (0.01, 1, 0);
    delay_time_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_time_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_time_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_time_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    delay_time_->addListener (this);
    delay_time_->setSkewFactor (0.4);

    addAndMakeVisible (delay_feedback_ = new Slider ("delay feedback"));
    delay_feedback_->setRange (-1, 1, 0);
    delay_feedback_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_feedback_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_feedback_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_feedback_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    delay_feedback_->addListener (this);

    addAndMakeVisible (delay_dry_wet_ = new Slider ("delay dry wet"));
    delay_dry_wet_->setRange (0, 1, 0);
    delay_dry_wet_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_dry_wet_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_dry_wet_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_dry_wet_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    delay_dry_wet_->addListener (this);

    addAndMakeVisible (velocity_track_ = new Slider ("velocity track"));
    velocity_track_->setRange (-1, 1, 0);
    velocity_track_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    velocity_track_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    velocity_track_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    velocity_track_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    velocity_track_->addListener (this);

    addAndMakeVisible (amp_attack_ = new Slider ("amp attack"));
    amp_attack_->setRange (0, 10, 0);
    amp_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_attack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_attack_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_attack_->addListener (this);
    amp_attack_->setSkewFactor (0.3);

    addAndMakeVisible (amp_decay_ = new Slider ("amp decay"));
    amp_decay_->setRange (0, 10, 0);
    amp_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_decay_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_decay_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_decay_->addListener (this);
    amp_decay_->setSkewFactor (0.3);

    addAndMakeVisible (amp_release_ = new Slider ("amp release"));
    amp_release_->setRange (0, 10, 0);
    amp_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_release_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_release_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_release_->addListener (this);
    amp_release_->setSkewFactor (0.3);

    addAndMakeVisible (amp_sustain_ = new Slider ("amp sustain"));
    amp_sustain_->setRange (0, 1, 0);
    amp_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_sustain_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_sustain_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_sustain_->addListener (this);

    addAndMakeVisible (fil_attack_ = new Slider ("fil attack"));
    fil_attack_->setRange (0, 10, 0);
    fil_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_attack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_attack_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_attack_->addListener (this);
    fil_attack_->setSkewFactor (0.3);

    addAndMakeVisible (fil_decay_ = new Slider ("fil decay"));
    fil_decay_->setRange (0, 10, 0);
    fil_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_decay_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_decay_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_decay_->addListener (this);
    fil_decay_->setSkewFactor (0.3);

    addAndMakeVisible (fil_release_ = new Slider ("fil release"));
    fil_release_->setRange (0, 10, 0);
    fil_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_release_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_release_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_release_->addListener (this);
    fil_release_->setSkewFactor (0.3);

    addAndMakeVisible (fil_sustain_ = new Slider ("fil sustain"));
    fil_sustain_->setRange (0, 1, 0);
    fil_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_sustain_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_sustain_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_sustain_->addListener (this);

    addAndMakeVisible (resonance_ = new Slider ("resonance"));
    resonance_->setRange (0, 1, 0);
    resonance_->setSliderStyle (Slider::LinearBar);
    resonance_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    resonance_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    resonance_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    resonance_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    resonance_->addListener (this);

    addAndMakeVisible (osc_1_waveform_ = new Slider ("osc 1 waveform"));
    osc_1_waveform_->setRange (0, 11, 1);
    osc_1_waveform_->setSliderStyle (Slider::LinearBar);
    osc_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc_1_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    osc_1_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    osc_1_waveform_->addListener (this);

    addAndMakeVisible (osc_2_waveform_ = new Slider ("osc 2 waveform"));
    osc_2_waveform_->setRange (0, 11, 1);
    osc_2_waveform_->setSliderStyle (Slider::LinearBar);
    osc_2_waveform_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    osc_2_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    osc_2_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    osc_2_waveform_->addListener (this);

    addAndMakeVisible (cutoff_ = new Slider ("cutoff"));
    cutoff_->setRange (28, 127, 0);
    cutoff_->setSliderStyle (Slider::LinearBar);
    cutoff_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    cutoff_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    cutoff_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    cutoff_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    cutoff_->addListener (this);

    addAndMakeVisible (fil_env_depth_ = new Slider ("fil env depth"));
    fil_env_depth_->setRange (-128, 128, 0);
    fil_env_depth_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_env_depth_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_env_depth_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_env_depth_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_env_depth_->addListener (this);

    addAndMakeVisible (keytrack_ = new Slider ("keytrack"));
    keytrack_->setRange (-1, 1, 0);
    keytrack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    keytrack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    keytrack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    keytrack_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    keytrack_->addListener (this);

    addAndMakeVisible (osc_feedback_transpose_ = new Slider ("osc feedback transpose"));
    osc_feedback_transpose_->setRange (-24, 24, 1);
    osc_feedback_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_transpose_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_feedback_transpose_->addListener (this);

    addAndMakeVisible (osc_feedback_amount_ = new Slider ("osc feedback amount"));
    osc_feedback_amount_->setRange (-1, 1, 0);
    osc_feedback_amount_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_amount_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_amount_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_amount_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_feedback_amount_->addListener (this);

    addAndMakeVisible (osc_feedback_tune_ = new Slider ("osc feedback tune"));
    osc_feedback_tune_->setRange (-1, 1, 0);
    osc_feedback_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_tune_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_feedback_tune_->addListener (this);

    addAndMakeVisible (lfo_1_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (lfo_1_waveform_ = new Slider ("lfo 1 waveform"));
    lfo_1_waveform_->setRange (0, 11, 1);
    lfo_1_waveform_->setSliderStyle (Slider::LinearBar);
    lfo_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    lfo_1_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    lfo_1_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    lfo_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    lfo_1_waveform_->addListener (this);

    addAndMakeVisible (num_steps_ = new Slider ("num steps"));
    num_steps_->setRange (1, 32, 1);
    num_steps_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    num_steps_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    num_steps_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    num_steps_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    num_steps_->addListener (this);

    addAndMakeVisible (step_frequency_ = new Slider ("step frequency"));
    step_frequency_->setRange (0, 20, 0);
    step_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    step_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    step_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    step_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    step_frequency_->addListener (this);
    step_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (lfo_1_frequency_ = new Slider ("lfo 1 frequency"));
    lfo_1_frequency_->setRange (0, 20, 0);
    lfo_1_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    lfo_1_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lfo_1_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    lfo_1_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    lfo_1_frequency_->addListener (this);
    lfo_1_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (lfo_2_frequency_ = new Slider ("lfo 2 frequency"));
    lfo_2_frequency_->setRange (0, 20, 0);
    lfo_2_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    lfo_2_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    lfo_2_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    lfo_2_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    lfo_2_frequency_->addListener (this);
    lfo_2_frequency_->setSkewFactor (0.5);

    addAndMakeVisible (filter_saturation_ = new Slider ("filter saturation"));
    filter_saturation_->setRange (0, 60, 0);
    filter_saturation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    filter_saturation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filter_saturation_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    filter_saturation_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    filter_saturation_->addListener (this);

    addAndMakeVisible (formant_gain_0_ = new Slider ("formant gain 0"));
    formant_gain_0_->setRange (0, 2, 0);
    formant_gain_0_->setSliderStyle (Slider::LinearBar);
    formant_gain_0_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_gain_0_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_gain_0_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_gain_0_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_gain_0_->addListener (this);

    addAndMakeVisible (formant_resonance_0_ = new Slider ("formant resonance 0"));
    formant_resonance_0_->setRange (0.5, 15, 0);
    formant_resonance_0_->setSliderStyle (Slider::LinearBar);
    formant_resonance_0_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_resonance_0_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_resonance_0_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_resonance_0_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_resonance_0_->addListener (this);

    addAndMakeVisible (formant_frequency_0_ = new Slider ("formant frequency 0"));
    formant_frequency_0_->setRange (10, 5000, 0);
    formant_frequency_0_->setSliderStyle (Slider::LinearBar);
    formant_frequency_0_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_frequency_0_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_frequency_0_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_frequency_0_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_frequency_0_->addListener (this);

    addAndMakeVisible (formant_gain_1_ = new Slider ("formant gain 1"));
    formant_gain_1_->setRange (0, 2, 0);
    formant_gain_1_->setSliderStyle (Slider::LinearBar);
    formant_gain_1_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_gain_1_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_gain_1_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_gain_1_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_gain_1_->addListener (this);

    addAndMakeVisible (formant_resonance_1_ = new Slider ("formant resonance 1"));
    formant_resonance_1_->setRange (0.5, 15, 0);
    formant_resonance_1_->setSliderStyle (Slider::LinearBar);
    formant_resonance_1_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_resonance_1_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_resonance_1_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_resonance_1_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_resonance_1_->addListener (this);

    addAndMakeVisible (formant_frequency_1_ = new Slider ("formant frequency 1"));
    formant_frequency_1_->setRange (10, 5000, 0);
    formant_frequency_1_->setSliderStyle (Slider::LinearBar);
    formant_frequency_1_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_frequency_1_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_frequency_1_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_frequency_1_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_frequency_1_->addListener (this);

    addAndMakeVisible (formant_gain_2_ = new Slider ("formant gain 2"));
    formant_gain_2_->setRange (0, 2, 0);
    formant_gain_2_->setSliderStyle (Slider::LinearBar);
    formant_gain_2_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_gain_2_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_gain_2_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_gain_2_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_gain_2_->addListener (this);

    addAndMakeVisible (formant_resonance_2_ = new Slider ("formant resonance 2"));
    formant_resonance_2_->setRange (0.5, 15, 0);
    formant_resonance_2_->setSliderStyle (Slider::LinearBar);
    formant_resonance_2_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_resonance_2_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_resonance_2_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_resonance_2_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_resonance_2_->addListener (this);

    addAndMakeVisible (formant_frequency_2_ = new Slider ("formant frequency 2"));
    formant_frequency_2_->setRange (10, 5000, 0);
    formant_frequency_2_->setSliderStyle (Slider::LinearBar);
    formant_frequency_2_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_frequency_2_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_frequency_2_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_frequency_2_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_frequency_2_->addListener (this);

    addAndMakeVisible (formant_gain_3_ = new Slider ("formant gain 3"));
    formant_gain_3_->setRange (0, 2, 0);
    formant_gain_3_->setSliderStyle (Slider::LinearBar);
    formant_gain_3_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_gain_3_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_gain_3_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_gain_3_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_gain_3_->addListener (this);

    addAndMakeVisible (formant_resonance_3_ = new Slider ("formant resonance 3"));
    formant_resonance_3_->setRange (0.5, 15, 0);
    formant_resonance_3_->setSliderStyle (Slider::LinearBar);
    formant_resonance_3_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_resonance_3_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_resonance_3_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_resonance_3_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_resonance_3_->addListener (this);

    addAndMakeVisible (formant_frequency_3_ = new Slider ("formant frequency 3"));
    formant_frequency_3_->setRange (10, 5000, 0);
    formant_frequency_3_->setSliderStyle (Slider::LinearBar);
    formant_frequency_3_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_frequency_3_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_frequency_3_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_frequency_3_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_frequency_3_->addListener (this);

    addAndMakeVisible (formant_passthrough_ = new Slider ("formant passthrough"));
    formant_passthrough_->setRange (0, 2, 0);
    formant_passthrough_->setSliderStyle (Slider::LinearBar);
    formant_passthrough_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_passthrough_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_passthrough_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_passthrough_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_passthrough_->addListener (this);

    addAndMakeVisible (formant_bypass_ = new ToggleButton ("formant bypass"));
    formant_bypass_->addListener (this);
    formant_bypass_->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (legato_ = new Slider ("legato"));
    legato_->setRange (0, 1, 1);
    legato_->setSliderStyle (Slider::LinearBar);
    legato_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    legato_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    legato_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    legato_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    legato_->addListener (this);

    addAndMakeVisible (formant_xy_pad_ = new XYPad());
    addAndMakeVisible (formant_x_ = new Slider ("formant x"));
    formant_x_->setRange (0, 1, 0);
    formant_x_->setSliderStyle (Slider::LinearBar);
    formant_x_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_x_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_x_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_x_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_x_->addListener (this);

    addAndMakeVisible (formant_y_ = new Slider ("formant y"));
    formant_y_->setRange (0, 1, 0);
    formant_y_->setSliderStyle (Slider::LinearBar);
    formant_y_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    formant_y_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_y_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_y_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_y_->addListener (this);

    addAndMakeVisible (filter_type_ = new Slider ("filter type"));
    filter_type_->setRange (0, 6, 1);
    filter_type_->setSliderStyle (Slider::LinearBar);
    filter_type_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filter_type_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    filter_type_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    filter_type_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    filter_type_->addListener (this);

    addAndMakeVisible (lfo_2_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (lfo_2_waveform_ = new Slider ("lfo 2 waveform"));
    lfo_2_waveform_->setRange (0, 11, 1);
    lfo_2_waveform_->setSliderStyle (Slider::LinearBar);
    lfo_2_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    lfo_2_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    lfo_2_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    lfo_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    lfo_2_waveform_->addListener (this);

    addAndMakeVisible (filter_envelope_mod_ = new TextButton ("filter env"));
    filter_envelope_mod_->setButtonText (TRANS("M"));
    filter_envelope_mod_->addListener (this);

    addAndMakeVisible (amplitude_envelope_mod_ = new TextButton ("amplitude env"));
    amplitude_envelope_mod_->setButtonText (TRANS("M"));
    amplitude_envelope_mod_->addListener (this);

    addAndMakeVisible (step_sequencer_mod_ = new TextButton ("step sequencer"));
    step_sequencer_mod_->setButtonText (TRANS("M"));
    step_sequencer_mod_->addListener (this);

    addAndMakeVisible (lfo_1_mod_ = new TextButton ("lfo 1"));
    lfo_1_mod_->setButtonText (TRANS("M"));
    lfo_1_mod_->addListener (this);

    addAndMakeVisible (lfo_2_mod_ = new TextButton ("lfo 2"));
    lfo_2_mod_->setButtonText (TRANS("M"));
    lfo_2_mod_->addListener (this);

    addAndMakeVisible (osc_1_mod_ = new TextButton ("osc 1"));
    osc_1_mod_->setButtonText (TRANS("M"));
    osc_1_mod_->addListener (this);

    addAndMakeVisible (osc_2_mod_ = new TextButton ("osc 2"));
    osc_2_mod_->setButtonText (TRANS("M"));
    osc_2_mod_->addListener (this);

    addAndMakeVisible (pitch_wheel_mod_ = new TextButton ("pitch wheel"));
    pitch_wheel_mod_->setButtonText (TRANS("M"));
    pitch_wheel_mod_->addListener (this);

    addAndMakeVisible (mod_wheel_mod_ = new TextButton ("mod wheel"));
    mod_wheel_mod_->setButtonText (TRANS("M"));
    mod_wheel_mod_->addListener (this);

    addAndMakeVisible (note_mod_ = new TextButton ("note"));
    note_mod_->setButtonText (TRANS("M"));
    note_mod_->addListener (this);

    addAndMakeVisible (velocity_mod_ = new TextButton ("velocity"));
    velocity_mod_->setButtonText (TRANS("M"));
    velocity_mod_->addListener (this);

    addAndMakeVisible (aftertouch_mod_ = new TextButton ("aftertouch"));
    aftertouch_mod_->setButtonText (TRANS("M"));
    aftertouch_mod_->addListener (this);


    //[UserPreSize]
    resonance_->setSliderStyle(Slider::LinearBarVertical);
    formant_y_->setSliderStyle(Slider::LinearBarVertical);

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

    formant_xy_pad_->setXSlider(formant_x_);
    formant_xy_pad_->setYSlider(formant_y_);

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

    osc_mix_->setLookAndFeel(&bipolar_look_and_feel_);

    for (int i = 0; i < getNumChildComponents(); ++i) {
        Slider* slider = dynamic_cast<Slider*>(getChildComponent(i));
        if (slider) {
            slider_lookup_[slider->getName().toStdString()] = slider;
            if (slider->getMaximum() == -slider->getMinimum())
                slider->setLookAndFeel(&bipolar_look_and_feel_);
        }

        Button* button = dynamic_cast<Button*>(getChildComponent(i));
        if (button)
            button_lookup_[button->getName().toStdString()] = button;
    }

    for (int i = 0; i < 4; ++i) {
        slider_lookup_[std::string("formant gain ") + std::to_string(i)]->setSliderStyle(Slider::LinearBarVertical);
        slider_lookup_[std::string("formant resonance ") + std::to_string(i)]->setSliderStyle(Slider::LinearBarVertical);
        slider_lookup_[std::string("formant frequency ") + std::to_string(i)]->setSliderStyle(Slider::LinearBarVertical);
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
    osc_1_waveform_ = nullptr;
    osc_2_waveform_ = nullptr;
    cutoff_ = nullptr;
    fil_env_depth_ = nullptr;
    keytrack_ = nullptr;
    osc_feedback_transpose_ = nullptr;
    osc_feedback_amount_ = nullptr;
    osc_feedback_tune_ = nullptr;
    lfo_1_wave_display_ = nullptr;
    lfo_1_waveform_ = nullptr;
    num_steps_ = nullptr;
    step_frequency_ = nullptr;
    lfo_1_frequency_ = nullptr;
    lfo_2_frequency_ = nullptr;
    filter_saturation_ = nullptr;
    formant_gain_0_ = nullptr;
    formant_resonance_0_ = nullptr;
    formant_frequency_0_ = nullptr;
    formant_gain_1_ = nullptr;
    formant_resonance_1_ = nullptr;
    formant_frequency_1_ = nullptr;
    formant_gain_2_ = nullptr;
    formant_resonance_2_ = nullptr;
    formant_frequency_2_ = nullptr;
    formant_gain_3_ = nullptr;
    formant_resonance_3_ = nullptr;
    formant_frequency_3_ = nullptr;
    formant_passthrough_ = nullptr;
    formant_bypass_ = nullptr;
    legato_ = nullptr;
    formant_xy_pad_ = nullptr;
    formant_x_ = nullptr;
    formant_y_ = nullptr;
    filter_type_ = nullptr;
    lfo_2_wave_display_ = nullptr;
    lfo_2_waveform_ = nullptr;
    filter_envelope_mod_ = nullptr;
    amplitude_envelope_mod_ = nullptr;
    step_sequencer_mod_ = nullptr;
    lfo_1_mod_ = nullptr;
    lfo_2_mod_ = nullptr;
    osc_1_mod_ = nullptr;
    osc_2_mod_ = nullptr;
    pitch_wheel_mod_ = nullptr;
    mod_wheel_mod_ = nullptr;
    note_mod_ = nullptr;
    velocity_mod_ = nullptr;
    aftertouch_mod_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SynthesisInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff3c2e50));

    g.setColour (Colour (0xff563e70));
    g.fillRect (612, 508, 180, 100);

    g.setColour (Colour (0xff563e70));
    g.fillRect (612, 612, 180, 84);

    g.setColour (Colour (0xff563e70));
    g.fillRect (428, 612, 180, 84);

    g.setColour (Colour (0xff563e70));
    g.fillRect (4, 4, 420, 236);

    g.setColour (Colour (0xff6a4584));
    g.fillRect (12, 24, 408, 152);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("OSC 2 TRANSPOSE"),
                236, 220, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("OSC 2 TUNE"),
                316, 220, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("X MOD"),
                172, 108, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff563e70));
    g.fillRect (4, 244, 420, 68);

    g.setColour (Colour (0xff563e70));
    g.fillRect (4, 316, 420, 188);

    g.setColour (Colour (0xff563e70));
    g.fillRect (428, 4, 364, 164);

    g.setColour (Colour (0xff563e70));
    g.fillRect (428, 172, 364, 164);

    g.setColour (Colour (0xff563e70));
    g.fillRect (428, 340, 364, 164);

    g.setColour (Colour (0xff563e70));
    g.fillRect (4, 508, 420, 100);

    g.setColour (Colour (0xff563e70));
    g.fillRect (428, 508, 180, 100);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FEEDBACK"),
                12, 244, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("OSCILLATORS"),
                12, 4, 116, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TRANSPOSE"),
                92, 292, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TUNE"),
                172, 292, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("AMOUNT"),
                260, 292, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("SATURATION"),
                340, 412, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FILTER"),
                12, 316, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("MONO LFO"),
                436, 508, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("POLY LFO"),
                620, 508, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                612, 564, 44, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                428, 564, 44, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FILTER ENVELOPE"),
                436, 4, 140, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("AMPLITUDE ENVELOPE"),
                436, 172, 140, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("STEP SEQUENCER"),
                436, 340, 140, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("DELAY"),
                436, 612, 44, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FEEDBACK"),
                492, 676, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TIME"),
                428, 676, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("DRY/WET"),
                556, 676, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("VOLUME"),
                620, 612, 60, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff563e70));
    g.fillRect (4, 612, 420, 84);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("ARTICULATION"),
                12, 612, 92, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("POLYPHONY"),
                20, 676, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PORTAMENTO"),
                108, 676, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("LEGATO"),
                212, 676, 92, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PORTAMENTO TYPE"),
                212, 644, 92, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PITCH BEND"),
                308, 676, 92, 12,
                Justification::centred, true);

    g.setColour (Colour (0xff563e70));
    g.fillRect (4, 700, 788, 36);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PITCH WHEEL"),
                36, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("MOD WHEEL"),
                188, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("NOTE"),
                348, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("VELOCITY"),
                508, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("AFTERTOUCH"),
                652, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("STEPS"),
                508, 476, 60, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQUENCY"),
                660, 476, 60, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("VEL TRACK"),
                708, 308, 60, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("KEY TRACK"),
                340, 476, 92, 20,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colours::white);
    //[/UserPaint]
}

void SynthesisInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    step_sequencer_->setBounds (464, 360, 300, 100);
    amplitude_envelope_->setBounds (464, 196, 300, 100);
    filter_envelope_->setBounds (464, 32, 300, 100);
    osc_1_wave_display_->setBounds (16, 40, 176, 112);
    osc_2_wave_display_->setBounds (240, 40, 176, 112);
    polyphony_->setBounds (40, 632, 50, 50);
    portamento_->setBounds (128, 632, 50, 50);
    pitch_bend_range_->setBounds (328, 624, 50, 50);
    cross_modulation_->setBounds (192, 64, 50, 50);
    filter_response_->setBounds (16, 360, 300, 100);
    portamento_type_->setBounds (224, 624, 64, 16);
    osc_mix_->setBounds (16, 152, 400, 24);
    osc_2_transpose_->setBounds (256, 176, 50, 50);
    osc_2_tune_->setBounds (336, 176, 50, 50);
    volume_->setBounds (672, 632, 96, 56);
    delay_time_->setBounds (432, 632, 50, 50);
    delay_feedback_->setBounds (496, 632, 50, 50);
    delay_dry_wet_->setBounds (560, 632, 50, 50);
    velocity_track_->setBounds (680, 301, 32, 32);
    amp_attack_->setBounds (464, 300, 32, 32);
    amp_decay_->setBounds (504, 300, 32, 32);
    amp_release_->setBounds (584, 300, 32, 32);
    amp_sustain_->setBounds (544, 300, 32, 32);
    fil_attack_->setBounds (464, 136, 32, 32);
    fil_decay_->setBounds (504, 136, 32, 32);
    fil_release_->setBounds (584, 136, 32, 32);
    fil_sustain_->setBounds (544, 136, 32, 32);
    resonance_->setBounds (316, 360, 12, 100);
    osc_1_waveform_->setBounds (16, 24, 176, 16);
    osc_2_waveform_->setBounds (240, 24, 176, 16);
    cutoff_->setBounds (16, 460, 300, 12);
    fil_env_depth_->setBounds (680, 136, 32, 32);
    keytrack_->setBounds (360, 432, 50, 50);
    osc_feedback_transpose_->setBounds (112, 248, 50, 50);
    osc_feedback_amount_->setBounds (280, 248, 50, 50);
    osc_feedback_tune_->setBounds (192, 248, 50, 50);
    lfo_1_wave_display_->setBounds (480, 544, 120, 64);
    lfo_1_waveform_->setBounds (480, 528, 120, 14);
    num_steps_->setBounds (480, 464, 40, 40);
    step_frequency_->setBounds (616, 464, 40, 40);
    lfo_1_frequency_->setBounds (432, 528, 40, 40);
    lfo_2_frequency_->setBounds (616, 528, 40, 40);
    filter_saturation_->setBounds (360, 360, 50, 50);
    formant_gain_0_->setBounds (456, 736, 12, 72);
    formant_resonance_0_->setBounds (472, 736, 12, 72);
    formant_frequency_0_->setBounds (488, 736, 12, 72);
    formant_gain_1_->setBounds (520, 736, 12, 72);
    formant_resonance_1_->setBounds (536, 736, 12, 72);
    formant_frequency_1_->setBounds (552, 736, 12, 72);
    formant_gain_2_->setBounds (584, 736, 12, 72);
    formant_resonance_2_->setBounds (600, 736, 12, 72);
    formant_frequency_2_->setBounds (616, 736, 12, 72);
    formant_gain_3_->setBounds (648, 736, 12, 72);
    formant_resonance_3_->setBounds (664, 736, 12, 72);
    formant_frequency_3_->setBounds (680, 736, 12, 72);
    formant_passthrough_->setBounds (336, 768, 112, 16);
    formant_bypass_->setBounds (8, 512, 120, 24);
    legato_->setBounds (224, 660, 64, 16);
    formant_xy_pad_->setBounds (144, 512, 256, 80);
    formant_x_->setBounds (144, 592, 256, 12);
    formant_y_->setBounds (400, 512, 12, 80);
    filter_type_->setBounds (16, 344, 300, 16);
    lfo_2_wave_display_->setBounds (664, 544, 120, 64);
    lfo_2_waveform_->setBounds (664, 528, 120, 14);
    filter_envelope_mod_->setBounds (432, 80, 24, 24);
    amplitude_envelope_mod_->setBounds (432, 248, 24, 24);
    step_sequencer_mod_->setBounds (432, 408, 24, 24);
    lfo_1_mod_->setBounds (440, 584, 24, 24);
    lfo_2_mod_->setBounds (624, 584, 24, 24);
    osc_1_mod_->setBounds (16, 184, 24, 24);
    osc_2_mod_->setBounds (392, 184, 24, 24);
    pitch_wheel_mod_->setBounds (24, 704, 24, 24);
    mod_wheel_mod_->setBounds (176, 704, 24, 24);
    note_mod_->setBounds (352, 704, 24, 24);
    velocity_mod_->setBounds (504, 704, 24, 24);
    aftertouch_mod_->setBounds (632, 704, 24, 24);
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
    else if (sliderThatWasMoved == formant_gain_0_)
    {
        //[UserSliderCode_formant_gain_0_] -- add your slider handling code here..
        //[/UserSliderCode_formant_gain_0_]
    }
    else if (sliderThatWasMoved == formant_resonance_0_)
    {
        //[UserSliderCode_formant_resonance_0_] -- add your slider handling code here..
        //[/UserSliderCode_formant_resonance_0_]
    }
    else if (sliderThatWasMoved == formant_frequency_0_)
    {
        //[UserSliderCode_formant_frequency_0_] -- add your slider handling code here..
        //[/UserSliderCode_formant_frequency_0_]
    }
    else if (sliderThatWasMoved == formant_gain_1_)
    {
        //[UserSliderCode_formant_gain_1_] -- add your slider handling code here..
        //[/UserSliderCode_formant_gain_1_]
    }
    else if (sliderThatWasMoved == formant_resonance_1_)
    {
        //[UserSliderCode_formant_resonance_1_] -- add your slider handling code here..
        //[/UserSliderCode_formant_resonance_1_]
    }
    else if (sliderThatWasMoved == formant_frequency_1_)
    {
        //[UserSliderCode_formant_frequency_1_] -- add your slider handling code here..
        //[/UserSliderCode_formant_frequency_1_]
    }
    else if (sliderThatWasMoved == formant_gain_2_)
    {
        //[UserSliderCode_formant_gain_2_] -- add your slider handling code here..
        //[/UserSliderCode_formant_gain_2_]
    }
    else if (sliderThatWasMoved == formant_resonance_2_)
    {
        //[UserSliderCode_formant_resonance_2_] -- add your slider handling code here..
        //[/UserSliderCode_formant_resonance_2_]
    }
    else if (sliderThatWasMoved == formant_frequency_2_)
    {
        //[UserSliderCode_formant_frequency_2_] -- add your slider handling code here..
        //[/UserSliderCode_formant_frequency_2_]
    }
    else if (sliderThatWasMoved == formant_gain_3_)
    {
        //[UserSliderCode_formant_gain_3_] -- add your slider handling code here..
        //[/UserSliderCode_formant_gain_3_]
    }
    else if (sliderThatWasMoved == formant_resonance_3_)
    {
        //[UserSliderCode_formant_resonance_3_] -- add your slider handling code here..
        //[/UserSliderCode_formant_resonance_3_]
    }
    else if (sliderThatWasMoved == formant_frequency_3_)
    {
        //[UserSliderCode_formant_frequency_3_] -- add your slider handling code here..
        //[/UserSliderCode_formant_frequency_3_]
    }
    else if (sliderThatWasMoved == formant_passthrough_)
    {
        //[UserSliderCode_formant_passthrough_] -- add your slider handling code here..
        //[/UserSliderCode_formant_passthrough_]
    }
    else if (sliderThatWasMoved == legato_)
    {
        //[UserSliderCode_legato_] -- add your slider handling code here..
        //[/UserSliderCode_legato_]
    }
    else if (sliderThatWasMoved == formant_x_)
    {
        //[UserSliderCode_formant_x_] -- add your slider handling code here..
        //[/UserSliderCode_formant_x_]
    }
    else if (sliderThatWasMoved == formant_y_)
    {
        //[UserSliderCode_formant_y_] -- add your slider handling code here..
        //[/UserSliderCode_formant_y_]
    }
    else if (sliderThatWasMoved == filter_type_)
    {
        //[UserSliderCode_filter_type_] -- add your slider handling code here..
        //[/UserSliderCode_filter_type_]
    }
    else if (sliderThatWasMoved == lfo_2_waveform_)
    {
        //[UserSliderCode_lfo_2_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_lfo_2_waveform_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SynthesisInterface::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    if (buttonThatWasClicked == formant_bypass_) {
        std::string name = buttonThatWasClicked->getName().toStdString();
        ValueChangeManager* parent = findParentComponentOfClass<ValueChangeManager>();
        if (parent)
            parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    }
    else {
        std::string name = buttonThatWasClicked->getName().toStdString();
        FullInterface* parent = findParentComponentOfClass<FullInterface>();
        if (parent)
            parent->changeModulator(name);
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == formant_bypass_)
    {
        //[UserButtonCode_formant_bypass_] -- add your button handler code here..
        //[/UserButtonCode_formant_bypass_]
    }
    else if (buttonThatWasClicked == filter_envelope_mod_)
    {
        //[UserButtonCode_filter_envelope_mod_] -- add your button handler code here..
        //[/UserButtonCode_filter_envelope_mod_]
    }
    else if (buttonThatWasClicked == amplitude_envelope_mod_)
    {
        //[UserButtonCode_amplitude_envelope_mod_] -- add your button handler code here..
        //[/UserButtonCode_amplitude_envelope_mod_]
    }
    else if (buttonThatWasClicked == step_sequencer_mod_)
    {
        //[UserButtonCode_step_sequencer_mod_] -- add your button handler code here..
        //[/UserButtonCode_step_sequencer_mod_]
    }
    else if (buttonThatWasClicked == lfo_1_mod_)
    {
        //[UserButtonCode_lfo_1_mod_] -- add your button handler code here..
        //[/UserButtonCode_lfo_1_mod_]
    }
    else if (buttonThatWasClicked == lfo_2_mod_)
    {
        //[UserButtonCode_lfo_2_mod_] -- add your button handler code here..
        //[/UserButtonCode_lfo_2_mod_]
    }
    else if (buttonThatWasClicked == osc_1_mod_)
    {
        //[UserButtonCode_osc_1_mod_] -- add your button handler code here..
        //[/UserButtonCode_osc_1_mod_]
    }
    else if (buttonThatWasClicked == osc_2_mod_)
    {
        //[UserButtonCode_osc_2_mod_] -- add your button handler code here..
        //[/UserButtonCode_osc_2_mod_]
    }
    else if (buttonThatWasClicked == pitch_wheel_mod_)
    {
        //[UserButtonCode_pitch_wheel_mod_] -- add your button handler code here..
        //[/UserButtonCode_pitch_wheel_mod_]
    }
    else if (buttonThatWasClicked == mod_wheel_mod_)
    {
        //[UserButtonCode_mod_wheel_mod_] -- add your button handler code here..
        //[/UserButtonCode_mod_wheel_mod_]
    }
    else if (buttonThatWasClicked == note_mod_)
    {
        //[UserButtonCode_note_mod_] -- add your button handler code here..
        //[/UserButtonCode_note_mod_]
    }
    else if (buttonThatWasClicked == velocity_mod_)
    {
        //[UserButtonCode_velocity_mod_] -- add your button handler code here..
        //[/UserButtonCode_velocity_mod_]
    }
    else if (buttonThatWasClicked == aftertouch_mod_)
    {
        //[UserButtonCode_aftertouch_mod_] -- add your button handler code here..
        //[/UserButtonCode_aftertouch_mod_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SynthesisInterface::setAllValues(mopo::control_map& controls) {
    for (auto slider : slider_lookup_) {
        if (controls.count(slider.first))
            slider.second->setValue(controls[slider.first]->value());
    }

    for (auto button : button_lookup_) {
        if (controls.count(button.first)) {
            button.second->setToggleState((bool)controls[button.first]->value(),
                                          NotificationType::sendNotification);
        }
    }
}

Slider* SynthesisInterface::getSlider(std::string name) {
    if (slider_lookup_.count(name))
        return slider_lookup_[name];
    return nullptr;
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
  <BACKGROUND backgroundColour="ff3c2e50">
    <RECT pos="612 508 180 100" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="612 612 180 84" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="428 612 180 84" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="4 4 420 236" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="12 24 408 152" fill="solid: ff6a4584" hasStroke="0"/>
    <TEXT pos="236 220 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSC 2 TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="316 220 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSC 2 TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="172 108 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="X MOD"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="4 244 420 68" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="4 316 420 188" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="428 4 364 164" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="428 172 364 164" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="428 340 364 164" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="4 508 420 100" fill="solid: ff563e70" hasStroke="0"/>
    <RECT pos="428 508 180 100" fill="solid: ff563e70" hasStroke="0"/>
    <TEXT pos="12 244 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="FEEDBACK"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="12 4 116 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSCILLATORS"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="92 292 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="172 292 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="260 292 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="AMOUNT"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="340 412 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="SATURATION"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="12 316 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="FILTER"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="436 508 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="MONO LFO"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="620 508 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="POLY LFO"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="612 564 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="428 564 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="436 4 140 20" fill="solid: ffd4b0e0" hasStroke="0" text="FILTER ENVELOPE"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="436 172 140 20" fill="solid: ffd4b0e0" hasStroke="0" text="AMPLITUDE ENVELOPE"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="436 340 140 20" fill="solid: ffd4b0e0" hasStroke="0" text="STEP SEQUENCER"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="436 612 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="DELAY"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="492 676 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="FEEDBACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="428 676 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="TIME"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="556 676 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="DRY/WET"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="620 612 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="VOLUME"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <RECT pos="4 612 420 84" fill="solid: ff563e70" hasStroke="0"/>
    <TEXT pos="12 612 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="ARTICULATION"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="20 676 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="POLYPHONY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="108 676 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="PORTAMENTO"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="212 676 92 12" fill="solid: ffd4b0e0" hasStroke="0" text="LEGATO"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="212 644 92 12" fill="solid: ffd4b0e0" hasStroke="0" text="PORTAMENTO TYPE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="308 676 92 12" fill="solid: ffd4b0e0" hasStroke="0" text="PITCH BEND"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="4 700 788 36" fill="solid: ff563e70" hasStroke="0"/>
    <TEXT pos="36 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="PITCH WHEEL"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="188 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="MOD WHEEL"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="348 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="NOTE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="508 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="VELOCITY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="652 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="AFTERTOUCH"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="508 476 60 12" fill="solid: ffd4b0e0" hasStroke="0" text="STEPS"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="660 476 60 12" fill="solid: ffd4b0e0" hasStroke="0" text="FREQUENCY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="708 308 60 12" fill="solid: ffd4b0e0" hasStroke="0" text="VEL TRACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="340 476 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="KEY TRACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <JUCERCOMP name="" id="83a23936a8f464b5" memberName="step_sequencer_" virtualName="GraphicalStepSequencer"
             explicitFocusOrder="0" pos="464 360 300 100" sourceFile="graphical_step_sequencer.cpp"
             constructorParams=""/>
  <JUCERCOMP name="" id="b4880edb8b39ec9d" memberName="amplitude_envelope_"
             virtualName="GraphicalEnvelope" explicitFocusOrder="0" pos="464 196 300 100"
             sourceFile="graphical_envelope.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="87feb60c88df4fcc" memberName="filter_envelope_" virtualName="GraphicalEnvelope"
             explicitFocusOrder="0" pos="464 32 300 100" sourceFile="graphical_envelope.cpp"
             constructorParams=""/>
  <JUCERCOMP name="osc 1 wave display" id="55100715382ea344" memberName="osc_1_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="16 40 176 112"
             sourceFile="wave_form_selector.cpp" constructorParams="256"/>
  <JUCERCOMP name="osc 2 wave display" id="c0c3e4a3ab2f045f" memberName="osc_2_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="240 40 176 112"
             sourceFile="wave_form_selector.cpp" constructorParams="256"/>
  <SLIDER name="polyphony" id="952bde38857bdba7" memberName="polyphony_"
          virtualName="" explicitFocusOrder="0" pos="40 632 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="1" max="32" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="portamento" id="9de85cc1c5f64eaa" memberName="portamento_"
          virtualName="" explicitFocusOrder="0" pos="128 632 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="0.4000000000000000222" int="0"
          style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="pitch bend range" id="e53afc6d1a04c708" memberName="pitch_bend_range_"
          virtualName="" explicitFocusOrder="0" pos="328 624 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="48" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cross modulation" id="1d3e4b59d6e470fb" memberName="cross_modulation_"
          virtualName="" explicitFocusOrder="0" pos="192 64 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="" id="e5ebb41c4c259ce1" memberName="filter_response_" virtualName="FilterResponse"
             explicitFocusOrder="0" pos="16 360 300 100" sourceFile="filter_response.cpp"
             constructorParams="300"/>
  <SLIDER name="portamento type" id="909956998c46045e" memberName="portamento_type_"
          virtualName="" explicitFocusOrder="0" pos="224 624 64 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="2"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc mix" id="211f463b59b2454f" memberName="osc_mix_" virtualName=""
          explicitFocusOrder="0" pos="16 152 400 24" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc 2 transpose" id="555c8ee21acbf804" memberName="osc_2_transpose_"
          virtualName="" explicitFocusOrder="0" pos="256 176 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-48" max="48" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc 2 tune" id="a8bc3bcffe7146f" memberName="osc_2_tune_"
          virtualName="" explicitFocusOrder="0" pos="336 176 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-1" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="volume" id="7cc7edfbfc537ee7" memberName="volume_" virtualName=""
          explicitFocusOrder="0" pos="672 632 96 56" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="delay time" id="1c4c7f1a27492c1d" memberName="delay_time_"
          virtualName="" explicitFocusOrder="0" pos="432 632 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0.010000000000000000208" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.4000000000000000222"/>
  <SLIDER name="delay feedback" id="c89eb62eea2ab491" memberName="delay_feedback_"
          virtualName="" explicitFocusOrder="0" pos="496 632 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-1" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay dry wet" id="dbc9d35179b5bac7" memberName="delay_dry_wet_"
          virtualName="" explicitFocusOrder="0" pos="560 632 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="velocity track" id="113bdc65c4c0f18f" memberName="velocity_track_"
          virtualName="" explicitFocusOrder="0" pos="680 301 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-1" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp attack" id="f6cda312a5619a33" memberName="amp_attack_"
          virtualName="" explicitFocusOrder="0" pos="464 300 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="amp decay" id="6de7c8b3a5a4826d" memberName="amp_decay_"
          virtualName="" explicitFocusOrder="0" pos="504 300 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="amp release" id="86b41ada65ec49c0" memberName="amp_release_"
          virtualName="" explicitFocusOrder="0" pos="584 300 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="amp sustain" id="3853f65a726f763" memberName="amp_sustain_"
          virtualName="" explicitFocusOrder="0" pos="544 300 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil attack" id="bc169bc84bd26782" memberName="fil_attack_"
          virtualName="" explicitFocusOrder="0" pos="464 136 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="fil decay" id="f5b3d1ec7fc18e1" memberName="fil_decay_"
          virtualName="" explicitFocusOrder="0" pos="504 136 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="fil release" id="c878127a7ada93f0" memberName="fil_release_"
          virtualName="" explicitFocusOrder="0" pos="584 136 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="10" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.2999999999999999889"/>
  <SLIDER name="fil sustain" id="5b25f915f3694f34" memberName="fil_sustain_"
          virtualName="" explicitFocusOrder="0" pos="544 136 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="resonance" id="6c1c82a2d59d4b6e" memberName="resonance_"
          virtualName="" explicitFocusOrder="0" pos="316 360 12 100" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc 1 waveform" id="ea97519d003b4224" memberName="osc_1_waveform_"
          virtualName="" explicitFocusOrder="0" pos="16 24 176 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="11"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="osc 2 waveform" id="a38d2af584df969a" memberName="osc_2_waveform_"
          virtualName="" explicitFocusOrder="0" pos="240 24 176 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="11"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cutoff" id="4ccde767164ea675" memberName="cutoff_" virtualName=""
          explicitFocusOrder="0" pos="16 460 300 12" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="28" max="127"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil env depth" id="ac3a5967de6a1a92" memberName="fil_env_depth_"
          virtualName="" explicitFocusOrder="0" pos="680 136 32 32" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-128" max="128" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="keytrack" id="33bbd8b71aa721c1" memberName="keytrack_"
          virtualName="" explicitFocusOrder="0" pos="360 432 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-1" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc feedback transpose" id="e747becfc7a4f3f7" memberName="osc_feedback_transpose_"
          virtualName="" explicitFocusOrder="0" pos="112 248 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-24" max="24" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc feedback amount" id="a94cee38c880759c" memberName="osc_feedback_amount_"
          virtualName="" explicitFocusOrder="0" pos="280 248 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-1" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc feedback tune" id="90dc1d31f03abf4e" memberName="osc_feedback_tune_"
          virtualName="" explicitFocusOrder="0" pos="192 248 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="-1" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="lfo 1 wave display" id="24d32b65108fb2a5" memberName="lfo_1_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="480 544 120 64"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="lfo 1 waveform" id="4ed06bb2c6901afe" memberName="lfo_1_waveform_"
          virtualName="" explicitFocusOrder="0" pos="480 528 120 14" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="11"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="num steps" id="8be29885961d7617" memberName="num_steps_"
          virtualName="" explicitFocusOrder="0" pos="480 464 40 40" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="1" max="32" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="step frequency" id="788574c1265fb47" memberName="step_frequency_"
          virtualName="" explicitFocusOrder="0" pos="616 464 40 40" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="20" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="lfo 1 frequency" id="ebc4a57528113c39" memberName="lfo_1_frequency_"
          virtualName="" explicitFocusOrder="0" pos="432 528 40 40" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="20" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="lfo 2 frequency" id="41dc8f7fbc0e8586" memberName="lfo_2_frequency_"
          virtualName="" explicitFocusOrder="0" pos="616 528 40 40" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="20" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="0.5"/>
  <SLIDER name="filter saturation" id="b5014a266e860882" memberName="filter_saturation_"
          virtualName="" explicitFocusOrder="0" pos="360 360 50 50" rotarysliderfill="7fffffff"
          textboxtext="ffdddddd" min="0" max="60" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant gain 0" id="7445acea26274d0c" memberName="formant_gain_0_"
          virtualName="" explicitFocusOrder="0" pos="456 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="2"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant resonance 0" id="a1166eff2b3b6567" memberName="formant_resonance_0_"
          virtualName="" explicitFocusOrder="0" pos="472 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0.5" max="15"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant frequency 0" id="e18eaf9056a405d" memberName="formant_frequency_0_"
          virtualName="" explicitFocusOrder="0" pos="488 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="10" max="5000"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant gain 1" id="9601c630fe693c63" memberName="formant_gain_1_"
          virtualName="" explicitFocusOrder="0" pos="520 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="2"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant resonance 1" id="4dc251d80763956c" memberName="formant_resonance_1_"
          virtualName="" explicitFocusOrder="0" pos="536 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0.5" max="15"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant frequency 1" id="8589ba088fbb5875" memberName="formant_frequency_1_"
          virtualName="" explicitFocusOrder="0" pos="552 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="10" max="5000"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant gain 2" id="28248ad5785f2cc6" memberName="formant_gain_2_"
          virtualName="" explicitFocusOrder="0" pos="584 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="2"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant resonance 2" id="e8b740fe8d402bf0" memberName="formant_resonance_2_"
          virtualName="" explicitFocusOrder="0" pos="600 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0.5" max="15"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant frequency 2" id="d3fd31f7f3439932" memberName="formant_frequency_2_"
          virtualName="" explicitFocusOrder="0" pos="616 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="10" max="5000"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant gain 3" id="ebcc9ad147b57478" memberName="formant_gain_3_"
          virtualName="" explicitFocusOrder="0" pos="648 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="2"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant resonance 3" id="31544fb199eb0cc7" memberName="formant_resonance_3_"
          virtualName="" explicitFocusOrder="0" pos="664 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0.5" max="15"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant frequency 3" id="2fd6f7bbc87ff01d" memberName="formant_frequency_3_"
          virtualName="" explicitFocusOrder="0" pos="680 736 12 72" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="10" max="5000"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant passthrough" id="4bec3c81bedb48a2" memberName="formant_passthrough_"
          virtualName="" explicitFocusOrder="0" pos="336 768 112 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="2"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="formant bypass" id="a27029ddc5597777" memberName="formant_bypass_"
                virtualName="" explicitFocusOrder="0" pos="8 512 120 24" txtcol="ffffffff"
                buttonText="formant bypass" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <SLIDER name="legato" id="5974d3f0077190f" memberName="legato_" virtualName=""
          explicitFocusOrder="0" pos="224 660 64 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="" id="202ea6e8e33b6ac7" memberName="formant_xy_pad_" virtualName="XYPad"
             explicitFocusOrder="0" pos="144 512 256 80" sourceFile="xy_pad.cpp"
             constructorParams=""/>
  <SLIDER name="formant x" id="d182d63c43cb241f" memberName="formant_x_"
          virtualName="" explicitFocusOrder="0" pos="144 592 256 12" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant y" id="f9e64695877940a6" memberName="formant_y_"
          virtualName="" explicitFocusOrder="0" pos="400 512 12 80" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="filter type" id="8d1283d4f2ace0ec" memberName="filter_type_"
          virtualName="" explicitFocusOrder="0" pos="16 344 300 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="6"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <JUCERCOMP name="lfo 1 wave display" id="d0c6b9dad7409074" memberName="lfo_2_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="664 544 120 64"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="lfo 2 waveform" id="315f17d5a0e21167" memberName="lfo_2_waveform_"
          virtualName="" explicitFocusOrder="0" pos="664 528 120 14" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="11"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <TEXTBUTTON name="filter env" id="b1c0e1b81ba12955" memberName="filter_envelope_mod_"
              virtualName="" explicitFocusOrder="0" pos="432 80 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="amplitude env" id="f8f7a3ca2ba5265d" memberName="amplitude_envelope_mod_"
              virtualName="" explicitFocusOrder="0" pos="432 248 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="step sequencer" id="4fd19f52e690cd89" memberName="step_sequencer_mod_"
              virtualName="" explicitFocusOrder="0" pos="432 408 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="lfo 1" id="1ea938f771b995ba" memberName="lfo_1_mod_" virtualName=""
              explicitFocusOrder="0" pos="440 584 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="lfo 2" id="db902bdbd3d218a6" memberName="lfo_2_mod_" virtualName=""
              explicitFocusOrder="0" pos="624 584 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="osc 1" id="4dcc36c544b0bbf7" memberName="osc_1_mod_" virtualName=""
              explicitFocusOrder="0" pos="16 184 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="osc 2" id="19039c3681943288" memberName="osc_2_mod_" virtualName=""
              explicitFocusOrder="0" pos="392 184 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pitch wheel" id="527add472856006" memberName="pitch_wheel_mod_"
              virtualName="" explicitFocusOrder="0" pos="24 704 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="mod wheel" id="38b300e016b7fbb" memberName="mod_wheel_mod_"
              virtualName="" explicitFocusOrder="0" pos="176 704 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="note" id="f119ef83db8634e0" memberName="note_mod_" virtualName=""
              explicitFocusOrder="0" pos="352 704 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="velocity" id="fc46bb54c2093224" memberName="velocity_mod_"
              virtualName="" explicitFocusOrder="0" pos="504 704 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="aftertouch" id="5e4c99bb63fbb5c6" memberName="aftertouch_mod_"
              virtualName="" explicitFocusOrder="0" pos="632 704 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
