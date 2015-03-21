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
#include "synth_gui_interface.h"
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
    addAndMakeVisible (polyphony_ = new TwytchSlider ("polyphony"));
    polyphony_->setRange (1, 32, 1);
    polyphony_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    polyphony_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    polyphony_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    polyphony_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    polyphony_->addListener (this);

    addAndMakeVisible (portamento_ = new TwytchSlider ("portamento"));
    portamento_->setRange (-9, -1, 0);
    portamento_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    portamento_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    portamento_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    portamento_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    portamento_->addListener (this);

    addAndMakeVisible (pitch_bend_range_ = new TwytchSlider ("pitch_bend_range"));
    pitch_bend_range_->setRange (0, 48, 1);
    pitch_bend_range_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    pitch_bend_range_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    pitch_bend_range_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    pitch_bend_range_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    pitch_bend_range_->addListener (this);

    addAndMakeVisible (cross_modulation_ = new TwytchSlider ("cross_modulation"));
    cross_modulation_->setRange (0, 1, 0);
    cross_modulation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    cross_modulation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    cross_modulation_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    cross_modulation_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    cross_modulation_->addListener (this);

    addAndMakeVisible (filter_response_ = new FilterResponse (300));
    addAndMakeVisible (portamento_type_ = new TwytchSlider ("portamento_type"));
    portamento_type_->setRange (0, 2, 1);
    portamento_type_->setSliderStyle (Slider::LinearBar);
    portamento_type_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    portamento_type_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    portamento_type_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    portamento_type_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    portamento_type_->addListener (this);

    addAndMakeVisible (osc_mix_ = new TwytchSlider ("osc_mix"));
    osc_mix_->setRange (0, 1, 0);
    osc_mix_->setSliderStyle (Slider::LinearBar);
    osc_mix_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_mix_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    osc_mix_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_mix_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    osc_mix_->addListener (this);

    addAndMakeVisible (osc_2_transpose_ = new TwytchSlider ("osc_2_transpose"));
    osc_2_transpose_->setRange (-48, 48, 1);
    osc_2_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_2_transpose_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_2_transpose_->addListener (this);

    addAndMakeVisible (osc_2_tune_ = new TwytchSlider ("osc_2_tune"));
    osc_2_tune_->setRange (-1, 1, 0);
    osc_2_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_2_tune_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_2_tune_->addListener (this);

    addAndMakeVisible (volume_ = new TwytchSlider ("volume"));
    volume_->setRange (0, 1, 0);
    volume_->setSliderStyle (Slider::LinearBar);
    volume_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volume_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    volume_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    volume_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    volume_->addListener (this);

    addAndMakeVisible (delay_feedback_ = new TwytchSlider ("delay_feedback"));
    delay_feedback_->setRange (-1, 1, 0);
    delay_feedback_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_feedback_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_feedback_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_feedback_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    delay_feedback_->addListener (this);

    addAndMakeVisible (delay_dry_wet_ = new TwytchSlider ("delay_dry_wet"));
    delay_dry_wet_->setRange (0, 1, 0);
    delay_dry_wet_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_dry_wet_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_dry_wet_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_dry_wet_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    delay_dry_wet_->addListener (this);

    addAndMakeVisible (velocity_track_ = new TwytchSlider ("velocity_track"));
    velocity_track_->setRange (-1, 1, 0);
    velocity_track_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    velocity_track_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    velocity_track_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    velocity_track_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    velocity_track_->addListener (this);

    addAndMakeVisible (amp_attack_ = new TwytchSlider ("amp_attack"));
    amp_attack_->setRange (0, 4, 0);
    amp_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_attack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_attack_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_attack_->addListener (this);

    addAndMakeVisible (amp_decay_ = new TwytchSlider ("amp_decay"));
    amp_decay_->setRange (0, 4, 0);
    amp_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_decay_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_decay_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_decay_->addListener (this);

    addAndMakeVisible (amp_release_ = new TwytchSlider ("amp_release"));
    amp_release_->setRange (0, 4, 0);
    amp_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_release_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_release_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_release_->addListener (this);

    addAndMakeVisible (amp_sustain_ = new TwytchSlider ("amp_sustain"));
    amp_sustain_->setRange (0, 1, 0);
    amp_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_sustain_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_sustain_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    amp_sustain_->addListener (this);

    addAndMakeVisible (fil_attack_ = new TwytchSlider ("fil_attack"));
    fil_attack_->setRange (0, 4, 0);
    fil_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_attack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_attack_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_attack_->addListener (this);

    addAndMakeVisible (fil_decay_ = new TwytchSlider ("fil_decay"));
    fil_decay_->setRange (0, 4, 0);
    fil_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_decay_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_decay_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_decay_->addListener (this);

    addAndMakeVisible (fil_release_ = new TwytchSlider ("fil_release"));
    fil_release_->setRange (0, 4, 0);
    fil_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_release_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_release_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_release_->addListener (this);

    addAndMakeVisible (fil_sustain_ = new TwytchSlider ("fil_sustain"));
    fil_sustain_->setRange (0, 1, 0);
    fil_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_sustain_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_sustain_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_sustain_->addListener (this);

    addAndMakeVisible (resonance_ = new TwytchSlider ("resonance"));
    resonance_->setRange (0, 1, 0);
    resonance_->setSliderStyle (Slider::LinearBar);
    resonance_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    resonance_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    resonance_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    resonance_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    resonance_->addListener (this);

    addAndMakeVisible (osc_1_waveform_ = new TwytchSlider ("osc_1_waveform"));
    osc_1_waveform_->setRange (0, 11, 1);
    osc_1_waveform_->setSliderStyle (Slider::LinearBar);
    osc_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc_1_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    osc_1_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    osc_1_waveform_->addListener (this);

    addAndMakeVisible (osc_2_waveform_ = new TwytchSlider ("osc_2_waveform"));
    osc_2_waveform_->setRange (0, 11, 1);
    osc_2_waveform_->setSliderStyle (Slider::LinearBar);
    osc_2_waveform_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    osc_2_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    osc_2_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    osc_2_waveform_->addListener (this);

    addAndMakeVisible (cutoff_ = new TwytchSlider ("cutoff"));
    cutoff_->setRange (28, 127, 0);
    cutoff_->setSliderStyle (Slider::LinearBar);
    cutoff_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    cutoff_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    cutoff_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    cutoff_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    cutoff_->addListener (this);

    addAndMakeVisible (fil_env_depth_ = new TwytchSlider ("fil_env_depth"));
    fil_env_depth_->setRange (-128, 128, 0);
    fil_env_depth_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_env_depth_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_env_depth_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_env_depth_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    fil_env_depth_->addListener (this);

    addAndMakeVisible (keytrack_ = new TwytchSlider ("keytrack"));
    keytrack_->setRange (-1, 1, 0);
    keytrack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    keytrack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    keytrack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    keytrack_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    keytrack_->addListener (this);

    addAndMakeVisible (osc_feedback_transpose_ = new TwytchSlider ("osc_feedback_transpose"));
    osc_feedback_transpose_->setRange (-24, 24, 1);
    osc_feedback_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_transpose_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_feedback_transpose_->addListener (this);

    addAndMakeVisible (osc_feedback_amount_ = new TwytchSlider ("osc_feedback_amount"));
    osc_feedback_amount_->setRange (-1, 1, 0);
    osc_feedback_amount_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_amount_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_amount_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_amount_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_feedback_amount_->addListener (this);

    addAndMakeVisible (osc_feedback_tune_ = new TwytchSlider ("osc_feedback_tune"));
    osc_feedback_tune_->setRange (-1, 1, 0);
    osc_feedback_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_tune_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_feedback_tune_->addListener (this);

    addAndMakeVisible (mono_lfo_1_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (mono_lfo_1_waveform_ = new TwytchSlider ("mono_lfo_1_waveform"));
    mono_lfo_1_waveform_->setRange (0, 11, 1);
    mono_lfo_1_waveform_->setSliderStyle (Slider::LinearBar);
    mono_lfo_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    mono_lfo_1_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    mono_lfo_1_waveform_->addListener (this);

    addAndMakeVisible (num_steps_ = new TwytchSlider ("num_steps"));
    num_steps_->setRange (1, 32, 1);
    num_steps_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    num_steps_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    num_steps_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    num_steps_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    num_steps_->addListener (this);

    addAndMakeVisible (step_frequency_ = new TwytchSlider ("step_frequency"));
    step_frequency_->setRange (-5, 6, 0);
    step_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    step_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    step_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    step_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    step_frequency_->addListener (this);

    addAndMakeVisible (mono_lfo_1_frequency_ = new TwytchSlider ("mono_lfo_1_frequency"));
    mono_lfo_1_frequency_->setRange (-7, 6, 0);
    mono_lfo_1_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_1_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mono_lfo_1_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    mono_lfo_1_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    mono_lfo_1_frequency_->addListener (this);

    addAndMakeVisible (poly_lfo_frequency_ = new TwytchSlider ("poly_lfo_frequency"));
    poly_lfo_frequency_->setRange (-7, 6, 0);
    poly_lfo_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    poly_lfo_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    poly_lfo_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    poly_lfo_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    poly_lfo_frequency_->addListener (this);

    addAndMakeVisible (filter_saturation_ = new TwytchSlider ("filter_saturation"));
    filter_saturation_->setRange (0, 60, 0);
    filter_saturation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    filter_saturation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filter_saturation_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    filter_saturation_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    filter_saturation_->addListener (this);

    addAndMakeVisible (formant_bypass_ = new ToggleButton ("formant_bypass"));
    formant_bypass_->setButtonText (TRANS("formant bypass"));
    formant_bypass_->addListener (this);
    formant_bypass_->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (legato_ = new TwytchSlider ("legato"));
    legato_->setRange (0, 1, 1);
    legato_->setSliderStyle (Slider::LinearBar);
    legato_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    legato_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    legato_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    legato_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    legato_->addListener (this);

    addAndMakeVisible (formant_xy_pad_ = new XYPad());
    addAndMakeVisible (formant_x_ = new TwytchSlider ("formant_x"));
    formant_x_->setRange (0, 1, 0);
    formant_x_->setSliderStyle (Slider::LinearBar);
    formant_x_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_x_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_x_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_x_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_x_->addListener (this);

    addAndMakeVisible (formant_y_ = new TwytchSlider ("formant_y"));
    formant_y_->setRange (0, 1, 0);
    formant_y_->setSliderStyle (Slider::LinearBar);
    formant_y_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    formant_y_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    formant_y_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_y_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    formant_y_->addListener (this);

    addAndMakeVisible (filter_type_ = new TwytchSlider ("filter_type"));
    filter_type_->setRange (0, 6, 1);
    filter_type_->setSliderStyle (Slider::LinearBar);
    filter_type_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filter_type_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    filter_type_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    filter_type_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    filter_type_->addListener (this);

    addAndMakeVisible (poly_lfo_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (poly_lfo_waveform_ = new TwytchSlider ("poly_lfo_waveform"));
    poly_lfo_waveform_->setRange (0, 11, 1);
    poly_lfo_waveform_->setSliderStyle (Slider::LinearBar);
    poly_lfo_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    poly_lfo_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    poly_lfo_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    poly_lfo_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    poly_lfo_waveform_->addListener (this);

    addAndMakeVisible (filter_envelope_mod_ = new TextButton ("filter_env"));
    filter_envelope_mod_->setButtonText (TRANS("M"));
    filter_envelope_mod_->addListener (this);

    addAndMakeVisible (amplitude_envelope_mod_ = new TextButton ("amplitude_env"));
    amplitude_envelope_mod_->setButtonText (TRANS("M"));
    amplitude_envelope_mod_->addListener (this);

    addAndMakeVisible (step_sequencer_mod_ = new TextButton ("step_sequencer"));
    step_sequencer_mod_->setButtonText (TRANS("M"));
    step_sequencer_mod_->addListener (this);

    addAndMakeVisible (mono_lfo_1_mod_ = new TextButton ("mono_lfo_1"));
    mono_lfo_1_mod_->setButtonText (TRANS("M"));
    mono_lfo_1_mod_->addListener (this);

    addAndMakeVisible (poly_lfo_mod_ = new TextButton ("poly_lfo"));
    poly_lfo_mod_->setButtonText (TRANS("M"));
    poly_lfo_mod_->addListener (this);

    addAndMakeVisible (osc_1_mod_ = new TextButton ("osc_1"));
    osc_1_mod_->setButtonText (TRANS("M"));
    osc_1_mod_->addListener (this);

    addAndMakeVisible (osc_2_mod_ = new TextButton ("osc_2"));
    osc_2_mod_->setButtonText (TRANS("M"));
    osc_2_mod_->addListener (this);

    addAndMakeVisible (pitch_wheel_mod_ = new TextButton ("pitch_wheel"));
    pitch_wheel_mod_->setButtonText (TRANS("M"));
    pitch_wheel_mod_->addListener (this);

    addAndMakeVisible (mod_wheel_mod_ = new TextButton ("mod_wheel"));
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

    addAndMakeVisible (osc_1_tune_ = new TwytchSlider ("osc_1_tune"));
    osc_1_tune_->setRange (-1, 1, 0);
    osc_1_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_1_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_1_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_1_tune_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_1_tune_->addListener (this);

    addAndMakeVisible (delay_frequency_ = new TwytchSlider ("delay_frequency"));
    delay_frequency_->setRange (-5, 1, 0);
    delay_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    delay_frequency_->addListener (this);

    addAndMakeVisible (mono_lfo_2_wave_display_ = new WaveFormSelector (128));
    addAndMakeVisible (mono_lfo_2_waveform_ = new TwytchSlider ("mono_lfo_2_waveform"));
    mono_lfo_2_waveform_->setRange (0, 11, 1);
    mono_lfo_2_waveform_->setSliderStyle (Slider::LinearBar);
    mono_lfo_2_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_waveform_->setColour (Slider::backgroundColourId, Colour (0xff190327));
    mono_lfo_2_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff452e60));
    mono_lfo_2_waveform_->addListener (this);

    addAndMakeVisible (mono_lfo_2_frequency_ = new TwytchSlider ("mono_lfo_2_frequency"));
    mono_lfo_2_frequency_->setRange (-7, 6, 0);
    mono_lfo_2_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_2_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mono_lfo_2_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    mono_lfo_2_frequency_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    mono_lfo_2_frequency_->addListener (this);

    addAndMakeVisible (mono_lfo_2_mod_ = new TextButton ("mono_lfo_2"));
    mono_lfo_2_mod_->setButtonText (TRANS("M"));
    mono_lfo_2_mod_->addListener (this);

    addAndMakeVisible (osc_1_transpose_ = new TwytchSlider ("osc_1_transpose"));
    osc_1_transpose_->setRange (-48, 48, 1);
    osc_1_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_1_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_1_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_1_transpose_->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    osc_1_transpose_->addListener (this);

    addAndMakeVisible (mono_lfo_1_sync_ = new ToggleButton ("mono_lfo_1_sync"));
    mono_lfo_1_sync_->setButtonText (String::empty);
    mono_lfo_1_sync_->addListener (this);

    addAndMakeVisible (mono_lfo_2_sync_ = new ToggleButton ("mono_lfo_2_sync"));
    mono_lfo_2_sync_->setButtonText (String::empty);
    mono_lfo_2_sync_->addListener (this);

    addAndMakeVisible (poly_lfo_sync_ = new ToggleButton ("poly_lfo_sync"));
    poly_lfo_sync_->setButtonText (String::empty);
    poly_lfo_sync_->addListener (this);

    addAndMakeVisible (step_sequencer_sync_ = new ToggleButton ("step_sequencer_sync"));
    step_sequencer_sync_->setButtonText (String::empty);
    step_sequencer_sync_->addListener (this);

    addAndMakeVisible (delay_sync_ = new ToggleButton ("delay_sync"));
    delay_sync_->setButtonText (String::empty);
    delay_sync_->addListener (this);


    //[UserPreSize]
    osc_1_transpose_->setBipolar();
    osc_1_tune_->setBipolar();
    osc_2_transpose_->setBipolar();
    osc_2_tune_->setBipolar();
    osc_mix_->setBipolar();

    osc_feedback_amount_->setBipolar();
    osc_feedback_transpose_->setBipolar();
    osc_feedback_tune_->setBipolar();

    keytrack_->setBipolar();
    fil_env_depth_->setBipolar();

    delay_feedback_->setBipolar();

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
    mono_lfo_1_wave_display_->setWaveSlider(mono_lfo_1_waveform_);
    mono_lfo_1_wave_display_->setName("mono_lfo_1_phase");
    mono_lfo_2_wave_display_->setWaveSlider(mono_lfo_2_waveform_);
    mono_lfo_2_wave_display_->setName("mono_lfo_2_phase");
    poly_lfo_wave_display_->setWaveSlider(poly_lfo_waveform_);

    step_sequencer_tempo_ = new TwytchSlider("step_sequencer_tempo");
    addAndMakeVisible(step_sequencer_tempo_);
    int num_tempos = sizeof(mopo::synced_freq_ratios) / sizeof(mopo::mopo_float);
    step_sequencer_tempo_->setRange(0, num_tempos - 1, 1);
    step_sequencer_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    step_sequencer_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    step_sequencer_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    step_sequencer_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffdddddd));
    step_sequencer_tempo_->addListener(this);

    mono_lfo_1_tempo_ = new TwytchSlider("mono_lfo_1_tempo");
    addAndMakeVisible(mono_lfo_1_tempo_);
    mono_lfo_1_tempo_->setRange(0, num_tempos - 1, 1);
    mono_lfo_1_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_1_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_1_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffdddddd));
    mono_lfo_1_tempo_->addListener(this);

    mono_lfo_2_tempo_ = new TwytchSlider("mono_lfo_2_tempo");
    addAndMakeVisible(mono_lfo_2_tempo_);
    mono_lfo_2_tempo_->setRange(0, num_tempos - 1, 1);
    mono_lfo_2_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_2_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_2_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffdddddd));
    mono_lfo_2_tempo_->addListener(this);

    poly_lfo_tempo_ = new TwytchSlider("poly_lfo_tempo");
    addAndMakeVisible(poly_lfo_tempo_);
    poly_lfo_tempo_->setRange(0, num_tempos - 1, 1);
    poly_lfo_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    poly_lfo_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    poly_lfo_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    poly_lfo_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffdddddd));
    poly_lfo_tempo_->addListener(this);

    delay_tempo_ = new TwytchSlider("delay_tempo");
    addAndMakeVisible(delay_tempo_);
    delay_tempo_->setRange(0, num_tempos - 1, 1);
    delay_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delay_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    delay_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    delay_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffdddddd));
    delay_tempo_->addListener(this);

    filter_response_->setCutoffSlider(cutoff_);
    filter_response_->setResonanceSlider(resonance_);
    filter_response_->setFilterTypeSlider(filter_type_);

    formant_xy_pad_->setXSlider(formant_x_);
    formant_xy_pad_->setYSlider(formant_y_);

    for (int i = 0; i < num_steps_->getMaximum(); ++i) {
        String num(i);
        if (num.length() == 1)
            num = String("0") + num;

        TwytchSlider* step = new TwytchSlider(String("step_seq_") + num);
        step->setRange(-1.0, 1.0);
        step->addListener(this);
        step_sequencer_sliders_.push_back(step);
        slider_lookup_[step->getName().toStdString()] = step;
    }
    step_sequencer_->setStepSliders(&step_sequencer_sliders_);
    step_sequencer_->setNumStepsSlider(num_steps_);
    step_sequencer_->setName("step_sequencer_step");

    for (int i = 0; i < getNumChildComponents(); ++i) {
        TwytchSlider* slider = dynamic_cast<TwytchSlider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;

        Button* button = dynamic_cast<Button*>(getChildComponent(i));
        if (button)
            button_lookup_[button->getName().toStdString()] = button;
    }

    setAllValues(controls);
    setDefaultDoubleClickValues();
    buttonClicked(step_sequencer_sync_);
    buttonClicked(mono_lfo_1_sync_);
    buttonClicked(mono_lfo_2_sync_);
    buttonClicked(poly_lfo_sync_);
    buttonClicked(delay_sync_);
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
    mono_lfo_1_wave_display_ = nullptr;
    mono_lfo_1_waveform_ = nullptr;
    num_steps_ = nullptr;
    step_frequency_ = nullptr;
    mono_lfo_1_frequency_ = nullptr;
    poly_lfo_frequency_ = nullptr;
    filter_saturation_ = nullptr;
    formant_bypass_ = nullptr;
    legato_ = nullptr;
    formant_xy_pad_ = nullptr;
    formant_x_ = nullptr;
    formant_y_ = nullptr;
    filter_type_ = nullptr;
    poly_lfo_wave_display_ = nullptr;
    poly_lfo_waveform_ = nullptr;
    filter_envelope_mod_ = nullptr;
    amplitude_envelope_mod_ = nullptr;
    step_sequencer_mod_ = nullptr;
    mono_lfo_1_mod_ = nullptr;
    poly_lfo_mod_ = nullptr;
    osc_1_mod_ = nullptr;
    osc_2_mod_ = nullptr;
    pitch_wheel_mod_ = nullptr;
    mod_wheel_mod_ = nullptr;
    note_mod_ = nullptr;
    velocity_mod_ = nullptr;
    aftertouch_mod_ = nullptr;
    osc_1_tune_ = nullptr;
    delay_frequency_ = nullptr;
    mono_lfo_2_wave_display_ = nullptr;
    mono_lfo_2_waveform_ = nullptr;
    mono_lfo_2_frequency_ = nullptr;
    mono_lfo_2_mod_ = nullptr;
    osc_1_transpose_ = nullptr;
    mono_lfo_1_sync_ = nullptr;
    mono_lfo_2_sync_ = nullptr;
    poly_lfo_sync_ = nullptr;
    step_sequencer_sync_ = nullptr;
    delay_sync_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SynthesisInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    step_sequencer_->showRealtimeFeedback();
    mono_lfo_1_wave_display_->showRealtimeFeedback();
    mono_lfo_2_wave_display_->showRealtimeFeedback();
    //[/UserPrePaint]

    g.fillAll (Colour (0xff1d0e29));

    g.setColour (Colour (0xff562b75));
    g.fillRect (672, 508, 120, 132);

    g.setColour (Colour (0xff664e79));
    g.drawRect (672, 508, 120, 132, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (550, 508, 120, 132);

    g.setColour (Colour (0xff664e79));
    g.drawRect (550, 508, 120, 132, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (612, 644, 180, 84);

    g.setColour (Colour (0xff664e79));
    g.drawRect (612, 644, 180, 84, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (428, 644, 180, 84);

    g.setColour (Colour (0xff664e79));
    g.drawRect (428, 644, 180, 84, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (4, 4, 420, 236);

    g.setColour (Colour (0xff664e79));
    g.drawRect (4, 4, 420, 236, 1);

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

    g.setColour (Colour (0xff562b75));
    g.fillRect (4, 244, 420, 68);

    g.setColour (Colour (0xff664e79));
    g.drawRect (4, 244, 420, 68, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (4, 316, 420, 188);

    g.setColour (Colour (0xff664e79));
    g.drawRect (4, 316, 420, 188, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (428, 4, 364, 164);

    g.setColour (Colour (0xff664e79));
    g.drawRect (428, 4, 364, 164, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (428, 172, 364, 164);

    g.setColour (Colour (0xff664e79));
    g.drawRect (428, 172, 364, 164, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (428, 340, 364, 164);

    g.setColour (Colour (0xff664e79));
    g.drawRect (428, 340, 364, 164, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (4, 508, 420, 132);

    g.setColour (Colour (0xff664e79));
    g.drawRect (4, 508, 420, 132, 1);

    g.setColour (Colour (0xff562b75));
    g.fillRect (428, 508, 120, 132);

    g.setColour (Colour (0xff664e79));
    g.drawRect (428, 508, 120, 132, 1);

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
                332, 364, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FILTER"),
                12, 316, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("MONO LFO 1"),
                436, 508, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("POLY LFO"),
                676, 508, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                508, 604, 44, 20,
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
                436, 644, 44, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FEEDBACK"),
                492, 708, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                428, 708, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("DRY/WET"),
                556, 708, 60, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("VOLUME"),
                620, 644, 60, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff562b75));
    g.fillRect (4, 644, 420, 84);

    g.setColour (Colour (0xff664379));
    g.drawRect (4, 644, 420, 84, 1);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("ARTICULATION"),
                12, 644, 92, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("POLYPHONY"),
                20, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PORTAMENTO"),
                108, 708, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("LEGATO"),
                212, 708, 92, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PORTAMENTO TYPE"),
                212, 676, 92, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PITCH BEND"),
                308, 708, 92, 12,
                Justification::centred, true);

    g.setColour (Colour (0xff562b75));
    g.fillRect (4, 732, 788, 36);

    g.setColour (Colour (0xff664379));
    g.drawRect (4, 732, 788, 36, 1);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PITCH WHEEL"),
                36, 740, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("MOD WHEEL"),
                188, 740, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("NOTE"),
                348, 740, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("VELOCITY"),
                508, 740, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("AFTERTOUCH"),
                652, 740, 92, 20,
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
                332, 428, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("OSC 1 TRANSPOSE"),
                28, 220, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("OSC 1 TUNE"),
                108, 220, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("MONO LFO 2"),
                556, 508, 84, 20,
                Justification::centredLeft, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                628, 604, 44, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                748, 604, 44, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffd4b0e0));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FILTER ENVELOPE DEPTH"),
                304, 484, 128, 20,
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
    polyphony_->setBounds (40, 664, 50, 50);
    portamento_->setBounds (128, 664, 50, 50);
    pitch_bend_range_->setBounds (328, 656, 50, 50);
    cross_modulation_->setBounds (192, 64, 50, 50);
    filter_response_->setBounds (16, 360, 300, 100);
    portamento_type_->setBounds (224, 656, 64, 16);
    osc_mix_->setBounds (16, 152, 400, 24);
    osc_2_transpose_->setBounds (256, 176, 50, 50);
    osc_2_tune_->setBounds (336, 176, 50, 50);
    volume_->setBounds (672, 664, 96, 56);
    delay_feedback_->setBounds (496, 664, 50, 50);
    delay_dry_wet_->setBounds (560, 664, 50, 50);
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
    fil_env_depth_->setBounds (352, 448, 50, 50);
    keytrack_->setBounds (352, 384, 50, 50);
    osc_feedback_transpose_->setBounds (112, 248, 50, 50);
    osc_feedback_amount_->setBounds (280, 248, 50, 50);
    osc_feedback_tune_->setBounds (192, 248, 50, 50);
    mono_lfo_1_wave_display_->setBounds (440, 542, 96, 48);
    mono_lfo_1_waveform_->setBounds (440, 528, 96, 14);
    num_steps_->setBounds (480, 464, 40, 40);
    step_frequency_->setBounds (616, 464, 40, 40);
    mono_lfo_1_frequency_->setBounds (480, 600, 40, 40);
    poly_lfo_frequency_->setBounds (720, 600, 40, 40);
    filter_saturation_->setBounds (352, 320, 50, 50);
    formant_bypass_->setBounds (0, 512, 120, 24);
    legato_->setBounds (224, 692, 64, 16);
    formant_xy_pad_->setBounds (120, 512, 280, 104);
    formant_x_->setBounds (120, 616, 280, 12);
    formant_y_->setBounds (400, 512, 12, 104);
    filter_type_->setBounds (16, 344, 300, 16);
    poly_lfo_wave_display_->setBounds (680, 542, 96, 48);
    poly_lfo_waveform_->setBounds (680, 528, 96, 14);
    filter_envelope_mod_->setBounds (432, 80, 24, 24);
    amplitude_envelope_mod_->setBounds (432, 248, 24, 24);
    step_sequencer_mod_->setBounds (432, 408, 24, 24);
    mono_lfo_1_mod_->setBounds (440, 608, 24, 24);
    poly_lfo_mod_->setBounds (680, 608, 24, 24);
    osc_1_mod_->setBounds (16, 184, 24, 24);
    osc_2_mod_->setBounds (392, 184, 24, 24);
    pitch_wheel_mod_->setBounds (24, 736, 24, 24);
    mod_wheel_mod_->setBounds (176, 736, 24, 24);
    note_mod_->setBounds (352, 736, 24, 24);
    velocity_mod_->setBounds (504, 736, 24, 24);
    aftertouch_mod_->setBounds (632, 736, 24, 24);
    osc_1_tune_->setBounds (128, 176, 50, 50);
    delay_frequency_->setBounds (432, 664, 50, 50);
    mono_lfo_2_wave_display_->setBounds (560, 542, 96, 48);
    mono_lfo_2_waveform_->setBounds (560, 528, 96, 14);
    mono_lfo_2_frequency_->setBounds (600, 600, 40, 40);
    mono_lfo_2_mod_->setBounds (560, 608, 24, 24);
    osc_1_transpose_->setBounds (48, 176, 50, 50);
    mono_lfo_1_sync_->setBounds (520, 616, 24, 24);
    mono_lfo_2_sync_->setBounds (640, 616, 24, 24);
    poly_lfo_sync_->setBounds (760, 616, 24, 24);
    step_sequencer_sync_->setBounds (664, 480, 24, 24);
    delay_sync_->setBounds (424, 704, 24, 24);
    //[UserResized] Add your own custom resize handling here..
    step_sequencer_tempo_->setBounds(step_frequency_->getBounds());
    mono_lfo_1_tempo_->setBounds(mono_lfo_1_frequency_->getBounds());
    mono_lfo_2_tempo_->setBounds(mono_lfo_2_frequency_->getBounds());
    poly_lfo_tempo_->setBounds(poly_lfo_frequency_->getBounds());
    delay_tempo_->setBounds(delay_frequency_->getBounds());
    //[/UserResized]
}

void SynthesisInterface::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    std::string name = sliderThatWasMoved->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
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
    else if (sliderThatWasMoved == mono_lfo_1_waveform_)
    {
        //[UserSliderCode_mono_lfo_1_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_1_waveform_]
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
    else if (sliderThatWasMoved == mono_lfo_1_frequency_)
    {
        //[UserSliderCode_mono_lfo_1_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_1_frequency_]
    }
    else if (sliderThatWasMoved == poly_lfo_frequency_)
    {
        //[UserSliderCode_poly_lfo_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_poly_lfo_frequency_]
    }
    else if (sliderThatWasMoved == filter_saturation_)
    {
        //[UserSliderCode_filter_saturation_] -- add your slider handling code here..
        //[/UserSliderCode_filter_saturation_]
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
    else if (sliderThatWasMoved == poly_lfo_waveform_)
    {
        //[UserSliderCode_poly_lfo_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_poly_lfo_waveform_]
    }
    else if (sliderThatWasMoved == osc_1_tune_)
    {
        //[UserSliderCode_osc_1_tune_] -- add your slider handling code here..
        //[/UserSliderCode_osc_1_tune_]
    }
    else if (sliderThatWasMoved == delay_frequency_)
    {
        //[UserSliderCode_delay_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_delay_frequency_]
    }
    else if (sliderThatWasMoved == mono_lfo_2_waveform_)
    {
        //[UserSliderCode_mono_lfo_2_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_2_waveform_]
    }
    else if (sliderThatWasMoved == mono_lfo_2_frequency_)
    {
        //[UserSliderCode_mono_lfo_2_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_2_frequency_]
    }
    else if (sliderThatWasMoved == osc_1_transpose_)
    {
        //[UserSliderCode_osc_1_transpose_] -- add your slider handling code here..
        //[/UserSliderCode_osc_1_transpose_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SynthesisInterface::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    std::string name = buttonThatWasClicked->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();

    if (buttonThatWasClicked == formant_bypass_) {
        if (parent)
            parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    }
    else if (buttonThatWasClicked == step_sequencer_sync_) {
        bool pressed = buttonThatWasClicked->getToggleState();
        if (parent)
            parent->valueChanged(name, pressed ? 1.0 : 0.0);
        step_frequency_->setVisible(!pressed);
        step_sequencer_tempo_->setVisible(pressed);
    }
    else if (buttonThatWasClicked == mono_lfo_1_sync_) {
        bool pressed = buttonThatWasClicked->getToggleState();
        if (parent)
            parent->valueChanged(name, pressed ? 1.0 : 0.0);
        mono_lfo_1_frequency_->setVisible(!pressed);
        mono_lfo_1_tempo_->setVisible(pressed);
    }
    else if (buttonThatWasClicked == mono_lfo_2_sync_) {
        bool pressed = buttonThatWasClicked->getToggleState();
        if (parent)
            parent->valueChanged(name, pressed ? 1.0 : 0.0);
        mono_lfo_2_frequency_->setVisible(!pressed);
        mono_lfo_2_tempo_->setVisible(pressed);
    }
    else if (buttonThatWasClicked == poly_lfo_sync_) {
        bool pressed = buttonThatWasClicked->getToggleState();
        if (parent)
            parent->valueChanged(name, pressed ? 1.0 : 0.0);
        poly_lfo_frequency_->setVisible(!pressed);
        poly_lfo_tempo_->setVisible(pressed);
    }
    else if (buttonThatWasClicked == delay_sync_) {
        bool pressed = buttonThatWasClicked->getToggleState();
        if (parent)
            parent->valueChanged(name, pressed ? 1.0 : 0.0);
        delay_frequency_->setVisible(!pressed);
        delay_tempo_->setVisible(pressed);
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
    else if (buttonThatWasClicked == mono_lfo_1_mod_)
    {
        //[UserButtonCode_mono_lfo_1_mod_] -- add your button handler code here..
        //[/UserButtonCode_mono_lfo_1_mod_]
    }
    else if (buttonThatWasClicked == poly_lfo_mod_)
    {
        //[UserButtonCode_poly_lfo_mod_] -- add your button handler code here..
        //[/UserButtonCode_poly_lfo_mod_]
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
    else if (buttonThatWasClicked == mono_lfo_2_mod_)
    {
        //[UserButtonCode_mono_lfo_2_mod_] -- add your button handler code here..
        //[/UserButtonCode_mono_lfo_2_mod_]
    }
    else if (buttonThatWasClicked == mono_lfo_1_sync_)
    {
        //[UserButtonCode_mono_lfo_1_sync_] -- add your button handler code here..
        //[/UserButtonCode_mono_lfo_1_sync_]
    }
    else if (buttonThatWasClicked == mono_lfo_2_sync_)
    {
        //[UserButtonCode_mono_lfo_2_sync_] -- add your button handler code here..
        //[/UserButtonCode_mono_lfo_2_sync_]
    }
    else if (buttonThatWasClicked == poly_lfo_sync_)
    {
        //[UserButtonCode_poly_lfo_sync_] -- add your button handler code here..
        //[/UserButtonCode_poly_lfo_sync_]
    }
    else if (buttonThatWasClicked == step_sequencer_sync_)
    {
        //[UserButtonCode_step_sequencer_sync_] -- add your button handler code here..
        //[/UserButtonCode_step_sequencer_sync_]
    }
    else if (buttonThatWasClicked == delay_sync_)
    {
        //[UserButtonCode_delay_sync_] -- add your button handler code here..
        //[/UserButtonCode_delay_sync_]
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

void SynthesisInterface::setValue(std::string name, mopo::mopo_float value,
                                  NotificationType notification) {
    if (slider_lookup_.count(name))
        slider_lookup_[name]->setValue(value, notification);
}

void SynthesisInterface::setDefaultDoubleClickValues() {
    cross_modulation_->setDoubleClickReturnValue(true, 0.0f);
    osc_mix_->setDoubleClickReturnValue(true, 0.5f);
    osc_1_transpose_->setDoubleClickReturnValue(true, 0.0f);
    osc_1_tune_->setDoubleClickReturnValue(true, 0.0f);
    osc_1_waveform_->setDoubleClickReturnValue(true, 0.0f);
    osc_2_transpose_->setDoubleClickReturnValue(true, 0.0f);
    osc_2_tune_->setDoubleClickReturnValue(true, 0.0f);
    osc_2_waveform_->setDoubleClickReturnValue(true, 0.0f);

    osc_feedback_amount_->setDoubleClickReturnValue(true, 0.0f);
    osc_feedback_transpose_->setDoubleClickReturnValue(true, 0.0f);
    osc_feedback_tune_->setDoubleClickReturnValue(true, 0.0f);

    filter_saturation_->setDoubleClickReturnValue(true, 0.0f);
    keytrack_->setDoubleClickReturnValue(true, 0.0f);
    cutoff_->setDoubleClickReturnValue(true, 80);
    resonance_->setDoubleClickReturnValue(true, 0.5f);

    formant_x_->setDoubleClickReturnValue(true, 0.5f);
    formant_y_->setDoubleClickReturnValue(true, 0.5f);

    portamento_->setDoubleClickReturnValue(true, -7.0f);
    pitch_bend_range_->setDoubleClickReturnValue(true, 2.0f);

    fil_attack_->setDoubleClickReturnValue(true, 0.01f);
    fil_decay_->setDoubleClickReturnValue(true, 0.5f);
    fil_sustain_->setDoubleClickReturnValue(true, 1.0f);
    fil_release_->setDoubleClickReturnValue(true, 0.01f);
    fil_env_depth_->setDoubleClickReturnValue(true, 0.0f);

    amp_attack_->setDoubleClickReturnValue(true, 0.01f);
    amp_decay_->setDoubleClickReturnValue(true, 0.5f);
    amp_sustain_->setDoubleClickReturnValue(true, 1.0f);
    amp_release_->setDoubleClickReturnValue(true, 0.01f);
    velocity_track_->setDoubleClickReturnValue(true, 0.0f);

    num_steps_->setDoubleClickReturnValue(true, 16.0f);
    step_frequency_->setDoubleClickReturnValue(true, 4.0f);

    mono_lfo_1_frequency_->setDoubleClickReturnValue(true, 0.0f);
    mono_lfo_2_frequency_->setDoubleClickReturnValue(true, 0.0f);
    poly_lfo_frequency_->setDoubleClickReturnValue(true, 0.0f);

    delay_dry_wet_->setDoubleClickReturnValue(true, 0.0f);
    delay_frequency_->setDoubleClickReturnValue(true, -3.0f);
    delay_feedback_->setDoubleClickReturnValue(true, 0.0f);
}

TwytchSlider* SynthesisInterface::getSlider(std::string name) {
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
  <BACKGROUND backgroundColour="ff1d0e29">
    <RECT pos="672 508 120 132" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="550 508 120 132" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="612 644 180 84" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="428 644 180 84" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="4 4 420 236" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <TEXT pos="236 220 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSC 2 TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="316 220 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSC 2 TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="172 108 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="X MOD"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="4 244 420 68" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="4 316 420 188" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="428 4 364 164" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="428 172 364 164" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="428 340 364 164" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="4 508 420 132" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
    <RECT pos="428 508 120 132" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664e79"/>
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
    <TEXT pos="332 364 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="SATURATION"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="12 316 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="FILTER"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="436 508 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="MONO LFO 1"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="676 508 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="POLY LFO"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="508 604 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="FREQ"
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
    <TEXT pos="436 644 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="DELAY"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="492 708 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="FEEDBACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="428 708 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="556 708 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="DRY/WET"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="620 644 60 20" fill="solid: ffd4b0e0" hasStroke="0" text="VOLUME"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <RECT pos="4 644 420 84" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664379"/>
    <TEXT pos="12 644 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="ARTICULATION"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="20 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="POLYPHONY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="108 708 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="PORTAMENTO"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="212 708 92 12" fill="solid: ffd4b0e0" hasStroke="0" text="LEGATO"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="212 676 92 12" fill="solid: ffd4b0e0" hasStroke="0" text="PORTAMENTO TYPE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="308 708 92 12" fill="solid: ffd4b0e0" hasStroke="0" text="PITCH BEND"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="4 732 788 36" fill="solid: ff562b75" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff664379"/>
    <TEXT pos="36 740 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="PITCH WHEEL"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="188 740 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="MOD WHEEL"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="348 740 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="NOTE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="508 740 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="VELOCITY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="652 740 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="AFTERTOUCH"
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
    <TEXT pos="332 428 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="KEY TRACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="28 220 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSC 1 TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="108 220 92 20" fill="solid: ffd4b0e0" hasStroke="0" text="OSC 1 TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="556 508 84 20" fill="solid: ffd4b0e0" hasStroke="0" text="MONO LFO 2"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="628 604 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="748 604 44 20" fill="solid: ffd4b0e0" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="304 484 128 20" fill="solid: ffd4b0e0" hasStroke="0" text="FILTER ENVELOPE DEPTH"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <JUCERCOMP name="step_sequencer" id="83a23936a8f464b5" memberName="step_sequencer_"
             virtualName="GraphicalStepSequencer" explicitFocusOrder="0" pos="464 360 300 100"
             sourceFile="graphical_step_sequencer.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="b4880edb8b39ec9d" memberName="amplitude_envelope_"
             virtualName="GraphicalEnvelope" explicitFocusOrder="0" pos="464 196 300 100"
             sourceFile="graphical_envelope.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="87feb60c88df4fcc" memberName="filter_envelope_" virtualName="GraphicalEnvelope"
             explicitFocusOrder="0" pos="464 32 300 100" sourceFile="graphical_envelope.cpp"
             constructorParams=""/>
  <JUCERCOMP name="osc_1_wave_display" id="55100715382ea344" memberName="osc_1_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="16 40 176 112"
             sourceFile="wave_form_selector.cpp" constructorParams="256"/>
  <JUCERCOMP name="osc_2_wave_display" id="c0c3e4a3ab2f045f" memberName="osc_2_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="240 40 176 112"
             sourceFile="wave_form_selector.cpp" constructorParams="256"/>
  <SLIDER name="polyphony" id="952bde38857bdba7" memberName="polyphony_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="40 664 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="1" max="32"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="portamento" id="9de85cc1c5f64eaa" memberName="portamento_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="128 664 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-9" max="-1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitch_bend_range" id="e53afc6d1a04c708" memberName="pitch_bend_range_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="328 656 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="48"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cross_modulation" id="1d3e4b59d6e470fb" memberName="cross_modulation_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="192 64 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="filter_response" id="e5ebb41c4c259ce1" memberName="filter_response_"
             virtualName="FilterResponse" explicitFocusOrder="0" pos="16 360 300 100"
             sourceFile="filter_response.cpp" constructorParams="300"/>
  <SLIDER name="portamento_type" id="909956998c46045e" memberName="portamento_type_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="224 656 64 16"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="2" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_mix" id="211f463b59b2454f" memberName="osc_mix_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="16 152 400 24" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_2_transpose" id="555c8ee21acbf804" memberName="osc_2_transpose_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="256 176 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-48"
          max="48" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_2_tune" id="a8bc3bcffe7146f" memberName="osc_2_tune_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="336 176 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="volume" id="7cc7edfbfc537ee7" memberName="volume_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="672 664 96 56" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay_feedback" id="c89eb62eea2ab491" memberName="delay_feedback_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="496 664 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay_dry_wet" id="dbc9d35179b5bac7" memberName="delay_dry_wet_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="560 664 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="velocity_track" id="113bdc65c4c0f18f" memberName="velocity_track_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="680 301 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_attack" id="f6cda312a5619a33" memberName="amp_attack_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="464 300 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_decay" id="6de7c8b3a5a4826d" memberName="amp_decay_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="504 300 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_release" id="86b41ada65ec49c0" memberName="amp_release_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="584 300 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_sustain" id="3853f65a726f763" memberName="amp_sustain_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="544 300 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_attack" id="bc169bc84bd26782" memberName="fil_attack_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="464 136 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_decay" id="f5b3d1ec7fc18e1" memberName="fil_decay_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="504 136 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_release" id="c878127a7ada93f0" memberName="fil_release_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="584 136 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_sustain" id="5b25f915f3694f34" memberName="fil_sustain_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="544 136 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="resonance" id="6c1c82a2d59d4b6e" memberName="resonance_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="316 360 12 100"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_1_waveform" id="ea97519d003b4224" memberName="osc_1_waveform_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="16 24 176 16"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="osc_2_waveform" id="a38d2af584df969a" memberName="osc_2_waveform_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="240 24 176 16"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cutoff" id="4ccde767164ea675" memberName="cutoff_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="16 460 300 12" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="28" max="127"
          int="0" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_env_depth" id="ac3a5967de6a1a92" memberName="fil_env_depth_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="352 448 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-128"
          max="128" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="keytrack" id="33bbd8b71aa721c1" memberName="keytrack_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="352 384 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_feedback_transpose" id="e747becfc7a4f3f7" memberName="osc_feedback_transpose_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="112 248 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-24"
          max="24" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_feedback_amount" id="a94cee38c880759c" memberName="osc_feedback_amount_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="280 248 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_feedback_tune" id="90dc1d31f03abf4e" memberName="osc_feedback_tune_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="192 248 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="mono_lfo_1" id="24d32b65108fb2a5" memberName="mono_lfo_1_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="440 542 96 48"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="mono_lfo_1_waveform" id="4ed06bb2c6901afe" memberName="mono_lfo_1_waveform_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="440 528 96 14"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="num_steps" id="8be29885961d7617" memberName="num_steps_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="480 464 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="1" max="32"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="step_frequency" id="788574c1265fb47" memberName="step_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="616 464 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-5" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="mono_lfo_1_frequency" id="ebc4a57528113c39" memberName="mono_lfo_1_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="480 600 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-7" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="poly_lfo_frequency" id="41dc8f7fbc0e8586" memberName="poly_lfo_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="720 600 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-7" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="filter_saturation" id="b5014a266e860882" memberName="filter_saturation_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="352 320 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="0" max="60"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="formant_bypass" id="a27029ddc5597777" memberName="formant_bypass_"
                virtualName="" explicitFocusOrder="0" pos="0 512 120 24" txtcol="ffffffff"
                buttonText="formant bypass" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <SLIDER name="legato" id="5974d3f0077190f" memberName="legato_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="224 692 64 16" bkgcol="ff190327"
          trackcol="ff9765bc" textboxoutline="ff452e60" min="0" max="1"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="formant_xy_pad" id="202ea6e8e33b6ac7" memberName="formant_xy_pad_"
             virtualName="XYPad" explicitFocusOrder="0" pos="120 512 280 104"
             sourceFile="xy_pad.cpp" constructorParams=""/>
  <SLIDER name="formant_x" id="d182d63c43cb241f" memberName="formant_x_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="120 616 280 12"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant_y" id="f9e64695877940a6" memberName="formant_y_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="400 512 12 104"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="filter_type" id="8d1283d4f2ace0ec" memberName="filter_type_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="16 344 300 16"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <JUCERCOMP name="poly_lfo_wave_display" id="d0c6b9dad7409074" memberName="poly_lfo_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="680 542 96 48"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="poly_lfo_waveform" id="315f17d5a0e21167" memberName="poly_lfo_waveform_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="680 528 96 14"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <TEXTBUTTON name="filter_env" id="b1c0e1b81ba12955" memberName="filter_envelope_mod_"
              virtualName="" explicitFocusOrder="0" pos="432 80 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="amplitude_env" id="f8f7a3ca2ba5265d" memberName="amplitude_envelope_mod_"
              virtualName="" explicitFocusOrder="0" pos="432 248 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="step_sequencer" id="4fd19f52e690cd89" memberName="step_sequencer_mod_"
              virtualName="" explicitFocusOrder="0" pos="432 408 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="mono_lfo_1" id="1ea938f771b995ba" memberName="mono_lfo_1_mod_"
              virtualName="" explicitFocusOrder="0" pos="440 608 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="poly_lfo" id="db902bdbd3d218a6" memberName="poly_lfo_mod_"
              virtualName="" explicitFocusOrder="0" pos="680 608 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="osc_1" id="4dcc36c544b0bbf7" memberName="osc_1_mod_" virtualName=""
              explicitFocusOrder="0" pos="16 184 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="osc_2" id="19039c3681943288" memberName="osc_2_mod_" virtualName=""
              explicitFocusOrder="0" pos="392 184 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pitch_wheel" id="527add472856006" memberName="pitch_wheel_mod_"
              virtualName="" explicitFocusOrder="0" pos="24 736 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="mod_wheel" id="38b300e016b7fbb" memberName="mod_wheel_mod_"
              virtualName="" explicitFocusOrder="0" pos="176 736 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="note" id="f119ef83db8634e0" memberName="note_mod_" virtualName=""
              explicitFocusOrder="0" pos="352 736 24 24" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="velocity" id="fc46bb54c2093224" memberName="velocity_mod_"
              virtualName="" explicitFocusOrder="0" pos="504 736 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="aftertouch" id="5e4c99bb63fbb5c6" memberName="aftertouch_mod_"
              virtualName="" explicitFocusOrder="0" pos="632 736 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="osc_1_tune" id="19b20e4d54ff8b49" memberName="osc_1_tune_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="128 176 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay_frequency" id="1119b78679f3c8ca" memberName="delay_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="432 664 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-5" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="mono_lfo_2" id="e0735b9f144e0bed" memberName="mono_lfo_2_wave_display_"
             virtualName="WaveFormSelector" explicitFocusOrder="0" pos="560 542 96 48"
             sourceFile="wave_form_selector.cpp" constructorParams="128"/>
  <SLIDER name="mono_lfo_2_waveform" id="ffccb388bb3729c" memberName="mono_lfo_2_waveform_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="560 528 96 14"
          bkgcol="ff190327" trackcol="ff9765bc" textboxoutline="ff452e60"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="mono_lfo_2_frequency" id="2367ff3311d37806" memberName="mono_lfo_2_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="600 600 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-7" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="mono_lfo_2" id="f62fd0363da4f3a6" memberName="mono_lfo_2_mod_"
              virtualName="" explicitFocusOrder="0" pos="560 608 24 24" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="osc_1_transpose" id="48a41a977b14ab08" memberName="osc_1_transpose_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="48 176 50 50"
          rotarysliderfill="7fffffff" textboxtext="ffdddddd" min="-48"
          max="48" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="mono_lfo_1_sync" id="ab29827e97077bb3" memberName="mono_lfo_1_sync_"
                virtualName="" explicitFocusOrder="0" pos="520 616 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="mono_lfo_2_sync" id="8590601e4f38107a" memberName="mono_lfo_2_sync_"
                virtualName="" explicitFocusOrder="0" pos="640 616 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="poly_lfo_sync" id="eb06cf065ff5cbe4" memberName="poly_lfo_sync_"
                virtualName="" explicitFocusOrder="0" pos="760 616 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="step_sequencer_sync" id="36872088faa7a11" memberName="step_sequencer_sync_"
                virtualName="" explicitFocusOrder="0" pos="664 480 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="delay_sync" id="5dec1a6032af38fd" memberName="delay_sync_"
                virtualName="" explicitFocusOrder="0" pos="424 704 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
