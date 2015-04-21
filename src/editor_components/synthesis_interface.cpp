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
    addAndMakeVisible (osc_1_wave_display_ = new WaveViewer (256));
    addAndMakeVisible (osc_2_wave_display_ = new WaveViewer (256));
    addAndMakeVisible (polyphony_ = new TwytchSlider ("polyphony"));
    polyphony_->setRange (1, 32, 1);
    polyphony_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    polyphony_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    polyphony_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    polyphony_->setColour (Slider::textBoxTextColourId, Colours::white);
    polyphony_->addListener (this);

    addAndMakeVisible (portamento_ = new TwytchSlider ("portamento"));
    portamento_->setRange (-9, -1, 0);
    portamento_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    portamento_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    portamento_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    portamento_->setColour (Slider::textBoxTextColourId, Colours::white);
    portamento_->addListener (this);

    addAndMakeVisible (pitch_bend_range_ = new TwytchSlider ("pitch_bend_range"));
    pitch_bend_range_->setRange (0, 48, 1);
    pitch_bend_range_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    pitch_bend_range_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    pitch_bend_range_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    pitch_bend_range_->setColour (Slider::textBoxTextColourId, Colours::white);
    pitch_bend_range_->addListener (this);

    addAndMakeVisible (cross_modulation_ = new TwytchSlider ("cross_modulation"));
    cross_modulation_->setRange (0, 1, 0);
    cross_modulation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    cross_modulation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    cross_modulation_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    cross_modulation_->setColour (Slider::textBoxTextColourId, Colours::white);
    cross_modulation_->addListener (this);

    addAndMakeVisible (portamento_type_ = new TwytchSlider ("portamento_type"));
    portamento_type_->setRange (0, 2, 1);
    portamento_type_->setSliderStyle (Slider::LinearBar);
    portamento_type_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    portamento_type_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    portamento_type_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    portamento_type_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    portamento_type_->addListener (this);

    addAndMakeVisible (osc_mix_ = new TwytchSlider ("osc_mix"));
    osc_mix_->setRange (0, 1, 0);
    osc_mix_->setSliderStyle (Slider::LinearBar);
    osc_mix_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_mix_->setColour (Slider::backgroundColourId, Colour (0xff303030));
    osc_mix_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_mix_->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
    osc_mix_->addListener (this);

    addAndMakeVisible (osc_2_transpose_ = new TwytchSlider ("osc_2_transpose"));
    osc_2_transpose_->setRange (-48, 48, 1);
    osc_2_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_2_transpose_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_2_transpose_->addListener (this);

    addAndMakeVisible (osc_2_tune_ = new TwytchSlider ("osc_2_tune"));
    osc_2_tune_->setRange (-1, 1, 0);
    osc_2_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_2_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_2_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_2_tune_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_2_tune_->addListener (this);

    addAndMakeVisible (volume_ = new TwytchSlider ("volume"));
    volume_->setRange (0, 1, 0);
    volume_->setSliderStyle (Slider::LinearBar);
    volume_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volume_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    volume_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    volume_->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
    volume_->addListener (this);

    addAndMakeVisible (delay_feedback_ = new TwytchSlider ("delay_feedback"));
    delay_feedback_->setRange (-1, 1, 0);
    delay_feedback_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_feedback_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_feedback_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_feedback_->setColour (Slider::textBoxTextColourId, Colours::white);
    delay_feedback_->addListener (this);

    addAndMakeVisible (delay_dry_wet_ = new TwytchSlider ("delay_dry_wet"));
    delay_dry_wet_->setRange (0, 1, 0);
    delay_dry_wet_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_dry_wet_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_dry_wet_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_dry_wet_->setColour (Slider::textBoxTextColourId, Colours::white);
    delay_dry_wet_->addListener (this);

    addAndMakeVisible (velocity_track_ = new TwytchSlider ("velocity_track"));
    velocity_track_->setRange (-1, 1, 0);
    velocity_track_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    velocity_track_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    velocity_track_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    velocity_track_->setColour (Slider::textBoxTextColourId, Colours::white);
    velocity_track_->addListener (this);

    addAndMakeVisible (amp_attack_ = new TwytchSlider ("amp_attack"));
    amp_attack_->setRange (0, 4, 0);
    amp_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_attack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_attack_->setColour (Slider::textBoxTextColourId, Colours::white);
    amp_attack_->addListener (this);

    addAndMakeVisible (amp_decay_ = new TwytchSlider ("amp_decay"));
    amp_decay_->setRange (0, 4, 0);
    amp_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_decay_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_decay_->setColour (Slider::textBoxTextColourId, Colours::white);
    amp_decay_->addListener (this);

    addAndMakeVisible (amp_release_ = new TwytchSlider ("amp_release"));
    amp_release_->setRange (0, 4, 0);
    amp_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_release_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_release_->setColour (Slider::textBoxTextColourId, Colours::white);
    amp_release_->addListener (this);

    addAndMakeVisible (amp_sustain_ = new TwytchSlider ("amp_sustain"));
    amp_sustain_->setRange (0, 1, 0);
    amp_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    amp_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    amp_sustain_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    amp_sustain_->setColour (Slider::textBoxTextColourId, Colours::white);
    amp_sustain_->addListener (this);

    addAndMakeVisible (fil_attack_ = new TwytchSlider ("fil_attack"));
    fil_attack_->setRange (0, 4, 0);
    fil_attack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_attack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_attack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_attack_->setColour (Slider::textBoxTextColourId, Colours::white);
    fil_attack_->addListener (this);

    addAndMakeVisible (fil_decay_ = new TwytchSlider ("fil_decay"));
    fil_decay_->setRange (0, 4, 0);
    fil_decay_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_decay_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_decay_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_decay_->setColour (Slider::textBoxTextColourId, Colours::white);
    fil_decay_->addListener (this);

    addAndMakeVisible (fil_release_ = new TwytchSlider ("fil_release"));
    fil_release_->setRange (0, 4, 0);
    fil_release_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_release_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_release_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_release_->setColour (Slider::textBoxTextColourId, Colours::white);
    fil_release_->addListener (this);

    addAndMakeVisible (fil_sustain_ = new TwytchSlider ("fil_sustain"));
    fil_sustain_->setRange (0, 1, 0);
    fil_sustain_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_sustain_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_sustain_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_sustain_->setColour (Slider::textBoxTextColourId, Colours::white);
    fil_sustain_->addListener (this);

    addAndMakeVisible (resonance_ = new TwytchSlider ("resonance"));
    resonance_->setRange (0, 1, 0);
    resonance_->setSliderStyle (Slider::LinearBar);
    resonance_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    resonance_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    resonance_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    resonance_->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    resonance_->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
    resonance_->addListener (this);

    addAndMakeVisible (osc_1_waveform_ = new WaveSelector ("osc_1_waveform"));
    osc_1_waveform_->setRange (0, 11, 1);
    osc_1_waveform_->setSliderStyle (Slider::LinearBar);
    osc_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    osc_1_waveform_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    osc_1_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff424242));
    osc_1_waveform_->addListener (this);

    addAndMakeVisible (osc_2_waveform_ = new WaveSelector ("osc_2_waveform"));
    osc_2_waveform_->setRange (0, 11, 1);
    osc_2_waveform_->setSliderStyle (Slider::LinearBar);
    osc_2_waveform_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    osc_2_waveform_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    osc_2_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    osc_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff424242));
    osc_2_waveform_->addListener (this);

    addAndMakeVisible (cutoff_ = new TwytchSlider ("cutoff"));
    cutoff_->setRange (28, 127, 0);
    cutoff_->setSliderStyle (Slider::LinearBar);
    cutoff_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    cutoff_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    cutoff_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    cutoff_->setColour (Slider::textBoxBackgroundColourId, Colour (0x00000000));
    cutoff_->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
    cutoff_->addListener (this);

    addAndMakeVisible (fil_env_depth_ = new TwytchSlider ("fil_env_depth"));
    fil_env_depth_->setRange (-128, 128, 0);
    fil_env_depth_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    fil_env_depth_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    fil_env_depth_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    fil_env_depth_->setColour (Slider::textBoxTextColourId, Colours::white);
    fil_env_depth_->addListener (this);

    addAndMakeVisible (keytrack_ = new TwytchSlider ("keytrack"));
    keytrack_->setRange (-1, 1, 0);
    keytrack_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    keytrack_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    keytrack_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    keytrack_->setColour (Slider::textBoxTextColourId, Colours::white);
    keytrack_->addListener (this);

    addAndMakeVisible (osc_feedback_transpose_ = new TwytchSlider ("osc_feedback_transpose"));
    osc_feedback_transpose_->setRange (-24, 24, 1);
    osc_feedback_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_transpose_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_feedback_transpose_->addListener (this);

    addAndMakeVisible (osc_feedback_amount_ = new TwytchSlider ("osc_feedback_amount"));
    osc_feedback_amount_->setRange (-1, 1, 0);
    osc_feedback_amount_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_amount_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_amount_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_amount_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_feedback_amount_->addListener (this);

    addAndMakeVisible (osc_feedback_tune_ = new TwytchSlider ("osc_feedback_tune"));
    osc_feedback_tune_->setRange (-1, 1, 0);
    osc_feedback_tune_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_tune_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_feedback_tune_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_feedback_tune_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_feedback_tune_->addListener (this);

    addAndMakeVisible (mono_lfo_1_wave_display_ = new WaveViewer (32));
    addAndMakeVisible (mono_lfo_1_waveform_ = new WaveSelector ("mono_lfo_1_waveform"));
    mono_lfo_1_waveform_->setRange (0, 11, 1);
    mono_lfo_1_waveform_->setSliderStyle (Slider::LinearBar);
    mono_lfo_1_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_waveform_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    mono_lfo_1_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_1_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff424242));
    mono_lfo_1_waveform_->addListener (this);

    addAndMakeVisible (num_steps_ = new TwytchSlider ("num_steps"));
    num_steps_->setRange (1, 32, 1);
    num_steps_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    num_steps_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    num_steps_->setColour (Slider::backgroundColourId, Colour (0x00000000));
    num_steps_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    num_steps_->setColour (Slider::textBoxTextColourId, Colours::white);
    num_steps_->addListener (this);

    addAndMakeVisible (step_frequency_ = new TwytchSlider ("step_frequency"));
    step_frequency_->setRange (-5, 6, 0);
    step_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    step_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    step_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    step_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    step_frequency_->addListener (this);

    addAndMakeVisible (mono_lfo_1_frequency_ = new TwytchSlider ("mono_lfo_1_frequency"));
    mono_lfo_1_frequency_->setRange (-7, 6, 0);
    mono_lfo_1_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_1_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mono_lfo_1_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    mono_lfo_1_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    mono_lfo_1_frequency_->addListener (this);

    addAndMakeVisible (filter_saturation_ = new TwytchSlider ("filter_saturation"));
    filter_saturation_->setRange (-20, 60, 0);
    filter_saturation_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    filter_saturation_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    filter_saturation_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    filter_saturation_->setColour (Slider::textBoxTextColourId, Colours::white);
    filter_saturation_->addListener (this);

    addAndMakeVisible (formant_on_ = new ToggleButton ("formant_on"));
    formant_on_->setButtonText (String::empty);
    formant_on_->addListener (this);
    formant_on_->setToggleState (true, dontSendNotification);
    formant_on_->setColour (ToggleButton::textColourId, Colour (0xff777777));

    addAndMakeVisible (legato_ = new TwytchSlider ("legato"));
    legato_->setRange (0, 1, 1);
    legato_->setSliderStyle (Slider::LinearBar);
    legato_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    legato_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    legato_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    legato_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    legato_->addListener (this);

    addAndMakeVisible (formant_xy_pad_ = new XYPad());
    addAndMakeVisible (formant_x_ = new TwytchSlider ("formant_x"));
    formant_x_->setRange (0, 1, 0);
    formant_x_->setSliderStyle (Slider::LinearBar);
    formant_x_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    formant_x_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    formant_x_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_x_->setColour (Slider::textBoxOutlineColourId, Colour (0x00777777));
    formant_x_->addListener (this);

    addAndMakeVisible (formant_y_ = new TwytchSlider ("formant_y"));
    formant_y_->setRange (0, 1, 0);
    formant_y_->setSliderStyle (Slider::LinearBar);
    formant_y_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    formant_y_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    formant_y_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    formant_y_->setColour (Slider::textBoxOutlineColourId, Colour (0x00777777));
    formant_y_->addListener (this);

    addAndMakeVisible (filter_type_ = new FilterSelector ("filter_type"));
    filter_type_->setRange (0, 6, 1);
    filter_type_->setSliderStyle (Slider::LinearBar);
    filter_type_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    filter_type_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    filter_type_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    filter_type_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    filter_type_->addListener (this);

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
    osc_1_tune_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_1_tune_->addListener (this);

    addAndMakeVisible (delay_frequency_ = new TwytchSlider ("delay_frequency"));
    delay_frequency_->setRange (-2, 5, 0);
    delay_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    delay_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    delay_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    delay_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    delay_frequency_->addListener (this);

    addAndMakeVisible (mono_lfo_2_wave_display_ = new WaveViewer (32));
    addAndMakeVisible (mono_lfo_2_waveform_ = new WaveSelector ("mono_lfo_2_waveform"));
    mono_lfo_2_waveform_->setRange (0, 11, 1);
    mono_lfo_2_waveform_->setSliderStyle (Slider::LinearBar);
    mono_lfo_2_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_waveform_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    mono_lfo_2_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_2_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff424242));
    mono_lfo_2_waveform_->addListener (this);

    addAndMakeVisible (osc_1_transpose_ = new TwytchSlider ("osc_1_transpose"));
    osc_1_transpose_->setRange (-48, 48, 1);
    osc_1_transpose_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    osc_1_transpose_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    osc_1_transpose_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    osc_1_transpose_->setColour (Slider::textBoxTextColourId, Colours::white);
    osc_1_transpose_->addListener (this);

    addAndMakeVisible (mono_lfo_2_frequency_ = new TwytchSlider ("mono_lfo_2_frequency"));
    mono_lfo_2_frequency_->setRange (-7, 6, 0);
    mono_lfo_2_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_2_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mono_lfo_2_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    mono_lfo_2_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    mono_lfo_2_frequency_->addListener (this);

    addAndMakeVisible (mono_lfo_2_mod_ = new TextButton ("mono_lfo_2"));
    mono_lfo_2_mod_->setButtonText (TRANS("M"));
    mono_lfo_2_mod_->addListener (this);

    addAndMakeVisible (stutter_frequency_ = new TwytchSlider ("stutter_frequency"));
    stutter_frequency_->setRange (4, 100, 0);
    stutter_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    stutter_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    stutter_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    stutter_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    stutter_frequency_->addListener (this);

    addAndMakeVisible (stutter_on_ = new ToggleButton ("stutter_on"));
    stutter_on_->setButtonText (String::empty);
    stutter_on_->addListener (this);
    stutter_on_->setToggleState (true, dontSendNotification);
    stutter_on_->setColour (ToggleButton::textColourId, Colour (0xff777777));

    addAndMakeVisible (stutter_resample_frequency_ = new TwytchSlider ("stutter_resample_frequency"));
    stutter_resample_frequency_->setRange (0.5, 20, 0);
    stutter_resample_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    stutter_resample_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    stutter_resample_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    stutter_resample_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    stutter_resample_frequency_->addListener (this);

    addAndMakeVisible (step_smoothing_ = new TwytchSlider ("step_smoothing"));
    step_smoothing_->setRange (0, 0.5, 0);
    step_smoothing_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    step_smoothing_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    step_smoothing_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    step_smoothing_->setColour (Slider::textBoxTextColourId, Colours::white);
    step_smoothing_->addListener (this);

    addAndMakeVisible (mono_lfo_1_sync_ = new TempoSelector ("mono_lfo_1_sync"));
    mono_lfo_1_sync_->setRange (0, 6, 1);
    mono_lfo_1_sync_->setSliderStyle (Slider::LinearBar);
    mono_lfo_1_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    mono_lfo_1_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_1_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    mono_lfo_1_sync_->addListener (this);

    addAndMakeVisible (mono_lfo_2_sync_ = new TempoSelector ("mono_lfo_2_sync"));
    mono_lfo_2_sync_->setRange (0, 6, 1);
    mono_lfo_2_sync_->setSliderStyle (Slider::LinearBar);
    mono_lfo_2_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    mono_lfo_2_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_2_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    mono_lfo_2_sync_->addListener (this);

    addAndMakeVisible (delay_sync_ = new TempoSelector ("delay_sync"));
    delay_sync_->setRange (0, 6, 1);
    delay_sync_->setSliderStyle (Slider::LinearBar);
    delay_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    delay_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    delay_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    delay_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    delay_sync_->addListener (this);

    addAndMakeVisible (step_sequencer_sync_ = new TempoSelector ("step_sequencer_sync"));
    step_sequencer_sync_->setRange (0, 6, 1);
    step_sequencer_sync_->setSliderStyle (Slider::LinearBar);
    step_sequencer_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    step_sequencer_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    step_sequencer_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    step_sequencer_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    step_sequencer_sync_->addListener (this);

    addAndMakeVisible (filter_response_ = new FilterResponse (300));
    addAndMakeVisible (poly_lfo_wave_display_ = new WaveViewer (32));
    addAndMakeVisible (poly_lfo_waveform_ = new WaveSelector ("poly_lfo_waveform"));
    poly_lfo_waveform_->setRange (0, 11, 1);
    poly_lfo_waveform_->setSliderStyle (Slider::LinearBar);
    poly_lfo_waveform_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    poly_lfo_waveform_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    poly_lfo_waveform_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    poly_lfo_waveform_->setColour (Slider::textBoxOutlineColourId, Colour (0xff424242));
    poly_lfo_waveform_->addListener (this);

    addAndMakeVisible (poly_lfo_frequency_ = new TwytchSlider ("poly_lfo_frequency"));
    poly_lfo_frequency_->setRange (-7, 6, 0);
    poly_lfo_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    poly_lfo_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    poly_lfo_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
    poly_lfo_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
    poly_lfo_frequency_->addListener (this);

    addAndMakeVisible (poly_lfo_mod_ = new TextButton ("poly_lfo"));
    poly_lfo_mod_->setButtonText (TRANS("M"));
    poly_lfo_mod_->addListener (this);

    addAndMakeVisible (poly_lfo_sync_ = new TempoSelector ("poly_lfo_sync"));
    poly_lfo_sync_->setRange (0, 6, 1);
    poly_lfo_sync_->setSliderStyle (Slider::LinearBar);
    poly_lfo_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
    poly_lfo_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
    poly_lfo_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    poly_lfo_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xff777777));
    poly_lfo_sync_->addListener (this);

    addAndMakeVisible (mono_lfo_1_amplitude_ = new TwytchSlider ("mono_lfo_1_amplitude"));
    mono_lfo_1_amplitude_->setRange (-1, 1, 0);
    mono_lfo_1_amplitude_->setSliderStyle (Slider::LinearBar);
    mono_lfo_1_amplitude_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mono_lfo_1_amplitude_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    mono_lfo_1_amplitude_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_1_amplitude_->setColour (Slider::textBoxOutlineColourId, Colour (0x00777777));
    mono_lfo_1_amplitude_->addListener (this);

    addAndMakeVisible (mono_lfo_2_amplitude_ = new TwytchSlider ("mono_lfo_2_amplitude"));
    mono_lfo_2_amplitude_->setRange (-1, 1, 0);
    mono_lfo_2_amplitude_->setSliderStyle (Slider::LinearBar);
    mono_lfo_2_amplitude_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    mono_lfo_2_amplitude_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    mono_lfo_2_amplitude_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    mono_lfo_2_amplitude_->setColour (Slider::textBoxOutlineColourId, Colour (0x00777777));
    mono_lfo_2_amplitude_->addListener (this);

    addAndMakeVisible (poly_lfo_amplitude_ = new TwytchSlider ("poly_lfo_amplitude"));
    poly_lfo_amplitude_->setRange (-1, 1, 0);
    poly_lfo_amplitude_->setSliderStyle (Slider::LinearBar);
    poly_lfo_amplitude_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    poly_lfo_amplitude_->setColour (Slider::backgroundColourId, Colour (0xff424242));
    poly_lfo_amplitude_->setColour (Slider::trackColourId, Colour (0xff9765bc));
    poly_lfo_amplitude_->setColour (Slider::textBoxOutlineColourId, Colour (0x00777777));
    poly_lfo_amplitude_->addListener (this);


    //[UserPreSize]
    createTempoSliders();
    setSliderUnits();
    markBipolarSliders();
    createStepSequencerSliders();

    resonance_->setSliderStyle(Slider::LinearBarVertical);
    formant_y_->setSliderStyle(Slider::LinearBarVertical);
    mono_lfo_1_amplitude_->setSliderStyle(Slider::LinearBarVertical);
    mono_lfo_2_amplitude_->setSliderStyle(Slider::LinearBarVertical);
    poly_lfo_amplitude_->setSliderStyle(Slider::LinearBarVertical);

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
    mono_lfo_1_wave_display_->setAmplitudeSlider(mono_lfo_1_amplitude_);
    mono_lfo_1_wave_display_->setName("mono_lfo_1_phase");
    mono_lfo_2_wave_display_->setWaveSlider(mono_lfo_2_waveform_);
    mono_lfo_2_wave_display_->setAmplitudeSlider(mono_lfo_2_amplitude_);
    mono_lfo_2_wave_display_->setName("mono_lfo_2_phase");
    poly_lfo_wave_display_->setWaveSlider(poly_lfo_waveform_);
    poly_lfo_wave_display_->setAmplitudeSlider(poly_lfo_amplitude_);
    filter_response_->setCutoffSlider(cutoff_);
    filter_response_->setResonanceSlider(resonance_);
    filter_response_->setFilterTypeSlider(filter_type_);

    formant_xy_pad_->setXSlider(formant_x_);
    formant_xy_pad_->setYSlider(formant_y_);

    mono_lfo_1_sync_->setTempoSlider(mono_lfo_1_tempo_);
    mono_lfo_1_sync_->setFreeSlider(mono_lfo_1_frequency_);
    mono_lfo_2_sync_->setTempoSlider(mono_lfo_2_tempo_);
    mono_lfo_2_sync_->setFreeSlider(mono_lfo_2_frequency_);
    poly_lfo_sync_->setTempoSlider(poly_lfo_tempo_);
    poly_lfo_sync_->setFreeSlider(poly_lfo_frequency_);
    step_sequencer_sync_->setTempoSlider(step_sequencer_tempo_);
    step_sequencer_sync_->setFreeSlider(step_frequency_);
    delay_sync_->setTempoSlider(delay_tempo_);
    delay_sync_->setFreeSlider(delay_frequency_);

    for (int i = 0; i < getNumChildComponents(); ++i) {
        TwytchSlider* slider = dynamic_cast<TwytchSlider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;

        Button* button = dynamic_cast<Button*>(getChildComponent(i));
        if (button)
            button_lookup_[button->getName().toStdString()] = button;
    }

    setDefaultDoubleClickValues();
    setAllValues(controls);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    setOpaque(true);
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
    filter_saturation_ = nullptr;
    formant_on_ = nullptr;
    legato_ = nullptr;
    formant_xy_pad_ = nullptr;
    formant_x_ = nullptr;
    formant_y_ = nullptr;
    filter_type_ = nullptr;
    filter_envelope_mod_ = nullptr;
    amplitude_envelope_mod_ = nullptr;
    step_sequencer_mod_ = nullptr;
    mono_lfo_1_mod_ = nullptr;
    pitch_wheel_mod_ = nullptr;
    mod_wheel_mod_ = nullptr;
    note_mod_ = nullptr;
    velocity_mod_ = nullptr;
    aftertouch_mod_ = nullptr;
    osc_1_tune_ = nullptr;
    delay_frequency_ = nullptr;
    mono_lfo_2_wave_display_ = nullptr;
    mono_lfo_2_waveform_ = nullptr;
    osc_1_transpose_ = nullptr;
    mono_lfo_2_frequency_ = nullptr;
    mono_lfo_2_mod_ = nullptr;
    stutter_frequency_ = nullptr;
    stutter_on_ = nullptr;
    stutter_resample_frequency_ = nullptr;
    step_smoothing_ = nullptr;
    mono_lfo_1_sync_ = nullptr;
    mono_lfo_2_sync_ = nullptr;
    delay_sync_ = nullptr;
    step_sequencer_sync_ = nullptr;
    filter_response_ = nullptr;
    poly_lfo_wave_display_ = nullptr;
    poly_lfo_waveform_ = nullptr;
    poly_lfo_frequency_ = nullptr;
    poly_lfo_mod_ = nullptr;
    poly_lfo_sync_ = nullptr;
    mono_lfo_1_amplitude_ = nullptr;
    mono_lfo_2_amplitude_ = nullptr;
    poly_lfo_amplitude_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    for (Slider* slider : step_sequencer_sliders_)
      delete slider;
    //[/Destructor]
}

//==============================================================================
void SynthesisInterface::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    step_sequencer_->showRealtimeFeedback();
    mono_lfo_1_wave_display_->showRealtimeFeedback();
    mono_lfo_2_wave_display_->showRealtimeFeedback();

    g.setColour(Colour(0xff303030));
    g.fillRect(0, 0, getWidth(), getHeight());

    //[/UserPrePaint]

    g.setColour (Colour (0xff424242));
    g.fillRect (266 - (116 / 2), 470, 116, 126);

    g.setColour (Colour (0xff424242));
    g.fillRect (4, 470, 200, 126);

    g.setColour (Colour (0xff424242));
    g.fillRect (651 - (126 / 2), 408, 126, 58);

    g.setColour (Colour (0xff009688));
    g.fillRect (651 - (126 / 2), 428 - 20, 126, 20);

    g.setColour (Colour (0xff424242));
    g.fillRect (651 - (126 / 2), 236, 126, 168);

    g.setColour (Colour (0xff009688));
    g.fillRect (651 - (126 / 2), 257 - 20, 126, 20);

    g.setColour (Colour (0xff424242));
    g.fillRect (651 - (126 / 2), 4, 126, 229);

    g.setColour (Colour (0xff009688));
    g.fillRect (714 - 126, 24 - 20, 126, 20);

    g.setColour (Colour (0xff424242));
    g.fillRect (328, 470, 126, 126);

    g.setColour (Colour (0xff009688));
    g.fillRect (454 - 126, 490 - 20, 126, 20);

    g.setColour (Colour (0xff424242));
    g.fillRect (456 - (256 / 2), 156, 256, 148);

    g.setColour (Colour (0xff009688));
    g.fillRect (456 - (256 / 2), 176 - 20, 256, 20);

    g.setColour (Colour (0xff424242));
    g.fillRect (456 - (256 / 2), 4, 256, 148);

    g.setColour (Colour (0xff424242));
    g.fillRect (164 - (320 / 2), 308, 320, 158);

    g.setColour (Colour (0xff424242));
    g.fillRect (164 - (320 / 2), 4, 320, 204);

    g.setColour (Colour (0xff009688));
    g.fillRect (164 - (320 / 2), 24 - 20, 320, 20);

    g.setColour (Colour (0xff424242));
    g.fillRect (521 - (126 / 2), 470, 126, 126);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TRANSPOSE"),
                227 - (70 / 2), 192, 70, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TUNE"),
                289 - (40 / 2), 192, 40, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("MOD"),
                164 - (50 / 2), 104, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff424242));
    g.fillRect (164 - (320 / 2), 212, 320, 92);

    g.setColour (Colour (0xff424242));
    g.fillRect (456 - (256 / 2), 308, 256, 158);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("OSCILLATORS"),
                164 - (100 / 2), 14 - (20 / 2), 100, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TRANSPOSE"),
                62 - (80 / 2), 288, 80, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TUNE"),
                130 - (50 / 2), 288, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("AMOUNT"),
                198 - (60 / 2), 288, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("SATURATION"),
                266 - (92 / 2), 288, 92, 10,
                Justification::centred, true);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("MONO LFO 1"),
                391 - (84 / 2), 470, 84, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                410 - (30 / 2), 586, 30, 10,
                Justification::centred, true);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("AMPLITUDE ENVELOPE"),
                456 - (150 / 2), 156, 150, 20,
                Justification::centred, true);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("DELAY"),
                651 - (44 / 2), 236, 44, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FEEDBACK"),
                618 - (60 / 2), 376, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                651 - (60 / 2), 312, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("DRY/WET"),
                687 - (60 / 2), 376, 60, 10,
                Justification::centred, true);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("VOLUME"),
                651 - (60 / 2), 408, 60, 20,
                Justification::centred, true);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("ARTICULATION"),
                651 - (92 / 2), 4, 92, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("POLYPHONY"),
                618 - (60 / 2), 88, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PORTA"),
                618 - (46 / 2), 160, 46, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("LEGATO"),
                684 - (60 / 2), 214, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PORT TYPE"),
                618 - (60 / 2), 214, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PITCH BEND"),
                684 - (60 / 2), 88, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff424242));
    g.fillRect (4, 622 - (44 / 2), 708, 44);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("PITCH WHEEL"),
                58, 622 - (12 / 2), 80, 12,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("MOD WHEEL"),
                211, 622 - (12 / 2), 60, 12,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("NOTE"),
                380, 622 - (12 / 2), 40, 12,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("VELOCITY"),
                510, 622 - (12 / 2), 60, 12,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("AFTERTOUCH"),
                640, 622 - (12 / 2), 70, 12,
                Justification::centredLeft, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("STEPS"),
                404 - (60 / 2), 450, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                468 - (60 / 2), 450, 60, 12,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("VEL TRACK"),
                684 - (60 / 2), 160, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("KEY TRACK"),
                287 - (60 / 2), 448, 60, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TRANSPOSE"),
                39 - (70 / 2), 192, 70, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("TUNE"),
                101 - (40 / 2), 192, 40, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("ENV DEPTH"),
                287 - (56 / 2), 384, 56, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                538 - (30 / 2), 586, 30, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                234 - (40 / 2), 560, 40, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("RESAMPLE "),
                298 - (50 / 2), 560, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff009688));
    g.fillRect (164 - (320 / 2), 232 - 20, 320, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FEEDBACK"),
                164 - (84 / 2), 212, 84, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff009688));
    g.fillRect (456 - (256 / 2), 24 - 20, 256, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FILTER ENVELOPE"),
                456 - (140 / 2), 14 - (20 / 2), 140, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff009688));
    g.fillRect (456 - (256 / 2), 328 - 20, 256, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("STEP SEQUENCER"),
                456 - (110 / 2), 307, 110, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff009688));
    g.fillRect (521 - (126 / 2), 490 - 20, 126, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("MONO LFO 2"),
                521 - (84 / 2), 470, 84, 20,
                Justification::centred, true);

    g.setColour (Colour (0xffb14a06));
    g.strokePath (internalPath1, PathStrokeType (1.000f));

    g.setColour (Colour (0xffb14a06));
    g.strokePath (internalPath2, PathStrokeType (1.000f));

    g.setColour (Colour (0xffb14a06));
    g.strokePath (internalPath3, PathStrokeType (1.000f));

    g.setColour (Colour (0xffb14a06));
    g.strokePath (internalPath4, PathStrokeType (1.000f));

    g.setColour (Colour (0xff009688));
    g.fillRect (104 - (200 / 2), 490 - 20, 200, 20);

    g.setColour (Colour (0xff009688));
    g.fillRect (266 - (116 / 2), 490 - 20, 116, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("STUTTER"),
                266 - (84 / 2), 470, 84, 20,
                Justification::centred, true);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FORMANT"),
                104 - (84 / 2), 470, 84, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("SMOOTHING"),
                540 - (60 / 2), 450, 60, 12,
                Justification::centred, true);

    g.setColour (Colour (0xffcccccc));
    g.fillRect (668, 511 - (1 / 2), 24, 1);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("MIX"),
                164 - (50 / 2), 142, 50, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff009688));
    g.fillRect (164 - (320 / 2), 328 - 20, 320, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("FILTER"),
                164 - (84 / 2), 308, 84, 20,
                Justification::centred, true);

    g.setColour (Colour (0xff424242));
    g.fillRect (649 - (126 / 2), 470, 126, 126);

    g.setColour (Colour (0xff777777));
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 11.40f, Font::plain));
    g.drawText (TRANS("FREQ"),
                668 - (30 / 2), 586, 30, 10,
                Justification::centred, true);

    g.setColour (Colour (0xff009688));
    g.fillRect (651 - (126 / 2), 490 - 20, 126, 20);

    g.setColour (Colours::white);
    g.setFont (Font (Font::getDefaultSansSerifFontName(), 14.40f, Font::plain));
    g.drawText (TRANS("POLY LFO"),
                651 - (84 / 2), 470, 84, 20,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    g.setColour(Colours::white);
    //[/UserPaint]
}

void SynthesisInterface::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    step_sequencer_->setBounds (456 - (256 / 2), 328, 256, 80);
    amplitude_envelope_->setBounds (456 - (256 / 2), 176, 256, 88);
    filter_envelope_->setBounds (456 - (256 / 2), 24, 256, 88);
    osc_1_wave_display_->setBounds (4, 40, 128, 80);
    osc_2_wave_display_->setBounds (196, 40, 128, 80);
    polyphony_->setBounds (618 - (46 / 2), 40, 46, 46);
    portamento_->setBounds (618 - (46 / 2), 112, 46, 46);
    pitch_bend_range_->setBounds (684 - (46 / 2), 40, 46, 46);
    cross_modulation_->setBounds (164 - (46 / 2), 56, 46, 46);
    portamento_type_->setBounds (618 - (40 / 2), 192, 40, 16);
    osc_mix_->setBounds (164 - (320 / 2), 120, 320, 16);
    osc_2_transpose_->setBounds (227 - (46 / 2), 144, 46, 46);
    osc_2_tune_->setBounds (289 - (46 / 2), 144, 46, 46);
    volume_->setBounds (588, 428, 126, 38);
    delay_feedback_->setBounds (618 - (46 / 2), 328, 46, 46);
    delay_dry_wet_->setBounds (687 - (46 / 2), 328, 46, 46);
    velocity_track_->setBounds (684 - (46 / 2), 112, 46, 46);
    amp_attack_->setBounds (384, 270, 32, 32);
    amp_decay_->setBounds (432, 270, 32, 32);
    amp_release_->setBounds (528, 270, 32, 32);
    amp_sustain_->setBounds (480, 270, 32, 32);
    fil_attack_->setBounds (384, 118, 32, 32);
    fil_decay_->setBounds (432, 118, 32, 32);
    fil_release_->setBounds (528, 118, 32, 32);
    fil_sustain_->setBounds (480, 118, 32, 32);
    resonance_->setBounds (244, 344, 10, 112);
    osc_1_waveform_->setBounds (4, 24, 128, 16);
    osc_2_waveform_->setBounds (196, 24, 128, 16);
    cutoff_->setBounds (4, 456, 240, 10);
    fil_env_depth_->setBounds (287 - (46 / 2), 336, 46, 46);
    keytrack_->setBounds (287 - (46 / 2), 400, 46, 46);
    osc_feedback_transpose_->setBounds (62 - (46 / 2), 240, 46, 46);
    osc_feedback_amount_->setBounds (198 - (46 / 2), 240, 46, 46);
    osc_feedback_tune_->setBounds (130 - (46 / 2), 240, 46, 46);
    mono_lfo_1_wave_display_->setBounds (338, 500, 116, 48);
    mono_lfo_1_waveform_->setBounds (338, 490, 116, 10);
    num_steps_->setBounds (404 - (40 / 2), 410, 40, 40);
    step_frequency_->setBounds (468 - (40 / 2), 410, 40, 40);
    mono_lfo_1_frequency_->setBounds (410 - (36 / 2), 548, 36, 36);
    filter_saturation_->setBounds (266 - (46 / 2), 240, 46, 46);
    formant_on_->setBounds (8, 472, 16, 16);
    legato_->setBounds (684 - (40 / 2), 192, 40, 16);
    formant_xy_pad_->setBounds (4, 490, 190, 96);
    formant_x_->setBounds (4, 586, 190, 10);
    formant_y_->setBounds (194, 490, 10, 96);
    filter_type_->setBounds (4, 328, 250, 16);
    filter_envelope_mod_->setBounds (338, 118, 32, 32);
    amplitude_envelope_mod_->setBounds (338, 270, 32, 32);
    step_sequencer_mod_->setBounds (338, 416, 32, 32);
    mono_lfo_1_mod_->setBounds (338, 556, 32, 32);
    pitch_wheel_mod_->setBounds (16, 606, 32, 32);
    mod_wheel_mod_->setBounds (169, 606, 32, 32);
    note_mod_->setBounds (338, 608, 32, 32);
    velocity_mod_->setBounds (468, 608, 32, 32);
    aftertouch_mod_->setBounds (598, 608, 32, 32);
    osc_1_tune_->setBounds (101 - (46 / 2), 144, 46, 46);
    delay_frequency_->setBounds (652 - (46 / 2), 264, 46, 46);
    mono_lfo_2_wave_display_->setBounds (468, 500, 116, 48);
    mono_lfo_2_waveform_->setBounds (468, 490, 116, 10);
    osc_1_transpose_->setBounds (39 - (46 / 2), 144, 46, 46);
    mono_lfo_2_frequency_->setBounds (538 - (36 / 2), 548, 36, 36);
    mono_lfo_2_mod_->setBounds (468, 556, 32, 32);
    stutter_frequency_->setBounds (233 - (46 / 2), 512, 46, 46);
    stutter_on_->setBounds (212, 472, 16, 16);
    stutter_resample_frequency_->setBounds (297 - (46 / 2), 512, 46, 46);
    step_smoothing_->setBounds (540 - (40 / 2), 410, 40, 40);
    mono_lfo_1_sync_->setBounds (438, 548, 16, 16);
    mono_lfo_2_sync_->setBounds (568, 548, 16, 16);
    delay_sync_->setBounds (680, 280, 16, 16);
    step_sequencer_sync_->setBounds (488, 424, 16, 16);
    filter_response_->setBounds (4, 344, 240, 112);
    poly_lfo_wave_display_->setBounds (598, 500, 116, 48);
    poly_lfo_waveform_->setBounds (598, 490, 116, 10);
    poly_lfo_frequency_->setBounds (668 - (36 / 2), 548, 36, 36);
    poly_lfo_mod_->setBounds (598, 556, 32, 32);
    poly_lfo_sync_->setBounds (698, 548, 16, 16);
    mono_lfo_1_amplitude_->setBounds (328, 500, 10, 48);
    mono_lfo_2_amplitude_->setBounds (458, 500, 10, 48);
    poly_lfo_amplitude_->setBounds (588, 500, 10, 48);
    internalPath1.clear();
    internalPath1.startNewSubPath (194.0f, 44.0f);
    internalPath1.lineTo (186.0f, 44.0f);
    internalPath1.lineTo (178.0f, 60.0f);

    internalPath2.clear();
    internalPath2.startNewSubPath (134.0f, 44.0f);
    internalPath2.lineTo (142.0f, 44.0f);
    internalPath2.lineTo (150.0f, 60.0f);

    internalPath3.clear();
    internalPath3.startNewSubPath (194.0f, 116.0f);
    internalPath3.lineTo (186.0f, 116.0f);
    internalPath3.lineTo (178.0f, 100.0f);

    internalPath4.clear();
    internalPath4.startNewSubPath (134.0f, 116.0f);
    internalPath4.lineTo (142.0f, 116.0f);
    internalPath4.lineTo (150.0f, 100.0f);

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
    else if (sliderThatWasMoved == osc_1_transpose_)
    {
        //[UserSliderCode_osc_1_transpose_] -- add your slider handling code here..
        //[/UserSliderCode_osc_1_transpose_]
    }
    else if (sliderThatWasMoved == mono_lfo_2_frequency_)
    {
        //[UserSliderCode_mono_lfo_2_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_2_frequency_]
    }
    else if (sliderThatWasMoved == stutter_frequency_)
    {
        //[UserSliderCode_stutter_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_stutter_frequency_]
    }
    else if (sliderThatWasMoved == stutter_resample_frequency_)
    {
        //[UserSliderCode_stutter_resample_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_stutter_resample_frequency_]
    }
    else if (sliderThatWasMoved == step_smoothing_)
    {
        //[UserSliderCode_step_smoothing_] -- add your slider handling code here..
        //[/UserSliderCode_step_smoothing_]
    }
    else if (sliderThatWasMoved == mono_lfo_1_sync_)
    {
        //[UserSliderCode_mono_lfo_1_sync_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_1_sync_]
    }
    else if (sliderThatWasMoved == mono_lfo_2_sync_)
    {
        //[UserSliderCode_mono_lfo_2_sync_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_2_sync_]
    }
    else if (sliderThatWasMoved == delay_sync_)
    {
        //[UserSliderCode_delay_sync_] -- add your slider handling code here..
        //[/UserSliderCode_delay_sync_]
    }
    else if (sliderThatWasMoved == step_sequencer_sync_)
    {
        //[UserSliderCode_step_sequencer_sync_] -- add your slider handling code here..
        //[/UserSliderCode_step_sequencer_sync_]
    }
    else if (sliderThatWasMoved == poly_lfo_waveform_)
    {
        //[UserSliderCode_poly_lfo_waveform_] -- add your slider handling code here..
        //[/UserSliderCode_poly_lfo_waveform_]
    }
    else if (sliderThatWasMoved == poly_lfo_frequency_)
    {
        //[UserSliderCode_poly_lfo_frequency_] -- add your slider handling code here..
        //[/UserSliderCode_poly_lfo_frequency_]
    }
    else if (sliderThatWasMoved == poly_lfo_sync_)
    {
        //[UserSliderCode_poly_lfo_sync_] -- add your slider handling code here..
        //[/UserSliderCode_poly_lfo_sync_]
    }
    else if (sliderThatWasMoved == mono_lfo_1_amplitude_)
    {
        //[UserSliderCode_mono_lfo_1_amplitude_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_1_amplitude_]
    }
    else if (sliderThatWasMoved == mono_lfo_2_amplitude_)
    {
        //[UserSliderCode_mono_lfo_2_amplitude_] -- add your slider handling code here..
        //[/UserSliderCode_mono_lfo_2_amplitude_]
    }
    else if (sliderThatWasMoved == poly_lfo_amplitude_)
    {
        //[UserSliderCode_poly_lfo_amplitude_] -- add your slider handling code here..
        //[/UserSliderCode_poly_lfo_amplitude_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SynthesisInterface::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    std::string name = buttonThatWasClicked->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();

    if (buttonThatWasClicked == formant_on_) {
        if (parent)
            parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    }
    else if (buttonThatWasClicked == stutter_on_) {
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

    if (buttonThatWasClicked == formant_on_)
    {
        //[UserButtonCode_formant_on_] -- add your button handler code here..
        //[/UserButtonCode_formant_on_]
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
    else if (buttonThatWasClicked == stutter_on_)
    {
        //[UserButtonCode_stutter_on_] -- add your button handler code here..
        //[/UserButtonCode_stutter_on_]
    }
    else if (buttonThatWasClicked == poly_lfo_mod_)
    {
        //[UserButtonCode_poly_lfo_mod_] -- add your button handler code here..
        //[/UserButtonCode_poly_lfo_mod_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SynthesisInterface::createTempoSliders() {
    step_sequencer_tempo_ = new TwytchSlider("step_sequencer_tempo");
    addAndMakeVisible(step_sequencer_tempo_);
    int num_tempos = sizeof(mopo::synced_freq_ratios) / sizeof(mopo::Value);
    step_sequencer_tempo_->setRange(0, num_tempos - 1, 1);
    step_sequencer_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    step_sequencer_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    step_sequencer_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    step_sequencer_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
    step_sequencer_tempo_->addListener(this);

    mono_lfo_1_tempo_ = new TwytchSlider("mono_lfo_1_tempo");
    addAndMakeVisible(mono_lfo_1_tempo_);
    mono_lfo_1_tempo_->setRange(0, num_tempos - 1, 1);
    mono_lfo_1_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_1_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_1_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
    mono_lfo_1_tempo_->addListener(this);

    mono_lfo_2_tempo_ = new TwytchSlider("mono_lfo_2_tempo");
    addAndMakeVisible(mono_lfo_2_tempo_);
    mono_lfo_2_tempo_->setRange(0, num_tempos - 1, 1);
    mono_lfo_2_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_2_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_2_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
    mono_lfo_2_tempo_->addListener(this);

    poly_lfo_tempo_ = new TwytchSlider("poly_lfo_tempo");
    addAndMakeVisible(poly_lfo_tempo_);
    poly_lfo_tempo_->setRange(0, num_tempos - 1, 1);
    poly_lfo_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    poly_lfo_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    poly_lfo_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    poly_lfo_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
    poly_lfo_tempo_->addListener(this);

    delay_tempo_ = new TwytchSlider("delay_tempo");
    addAndMakeVisible(delay_tempo_);
    delay_tempo_->setRange(0, num_tempos - 1, 1);
    delay_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delay_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    delay_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    delay_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
    delay_tempo_->addListener(this);
}

void SynthesisInterface::setSliderUnits() {
    osc_1_transpose_->setUnits("semitones");
    osc_2_transpose_->setUnits("semitones");
    osc_1_tune_->setPostMultiply(100.0);
    osc_1_tune_->setUnits("cents");
    osc_2_tune_->setPostMultiply(100.0);
    osc_2_tune_->setUnits("cents");
    osc_feedback_transpose_->setUnits("semitones");
    osc_feedback_tune_->setUnits("cents");
    osc_feedback_tune_->setPostMultiply(100.0);
    filter_saturation_->setUnits("dB");

    fil_env_depth_->setUnits("semitones");
    portamento_->setUnits("secs/semitone");
    portamento_->setScalingType(TwytchSlider::kExponential);
    pitch_bend_range_->setUnits("semitones");

    polyphony_->setUnits("voices");

    keytrack_->setUnits("%");
    keytrack_->setPostMultiply(100.0);
    velocity_track_->setUnits("%");
    velocity_track_->setPostMultiply(100.0);

    mono_lfo_1_frequency_->setUnits("Hz");
    mono_lfo_2_frequency_->setUnits("Hz");
    poly_lfo_frequency_->setUnits("Hz");
    step_frequency_->setUnits("Hz");
    delay_frequency_->setUnits("Hz");
    mono_lfo_1_frequency_->setScalingType(TwytchSlider::kExponential);
    mono_lfo_2_frequency_->setScalingType(TwytchSlider::kExponential);
    poly_lfo_frequency_->setScalingType(TwytchSlider::kExponential);
    step_frequency_->setScalingType(TwytchSlider::kExponential);
    delay_frequency_->setScalingType(TwytchSlider::kExponential);

    amp_attack_->setUnits("secs");
    amp_decay_->setUnits("secs");
    amp_release_->setUnits("secs");
    fil_attack_->setUnits("secs");
    fil_decay_->setUnits("secs");
    fil_release_->setUnits("secs");
    amp_attack_->setScalingType(TwytchSlider::kPolynomial);
    amp_decay_->setScalingType(TwytchSlider::kPolynomial);
    amp_release_->setScalingType(TwytchSlider::kPolynomial);
    fil_attack_->setScalingType(TwytchSlider::kPolynomial);
    fil_decay_->setScalingType(TwytchSlider::kPolynomial);
    fil_release_->setScalingType(TwytchSlider::kPolynomial);

    mono_lfo_1_tempo_->setStringLookup(mopo::strings::synced_frequencies);
    mono_lfo_2_tempo_->setStringLookup(mopo::strings::synced_frequencies);
    poly_lfo_tempo_->setStringLookup(mopo::strings::synced_frequencies);
    step_sequencer_tempo_->setStringLookup(mopo::strings::synced_frequencies);
    delay_tempo_->setStringLookup(mopo::strings::synced_frequencies);

    mono_lfo_1_sync_->setStringLookup(mopo::strings::freq_sync_styles);
    mono_lfo_2_sync_->setStringLookup(mopo::strings::freq_sync_styles);
    poly_lfo_sync_->setStringLookup(mopo::strings::freq_sync_styles);
    step_sequencer_sync_->setStringLookup(mopo::strings::freq_sync_styles);
    delay_sync_->setStringLookup(mopo::strings::freq_sync_styles);

    legato_->setStringLookup(mopo::strings::off_on);
    portamento_type_->setStringLookup(mopo::strings::off_auto_on);

    osc_1_waveform_->setStringLookup(mopo::strings::waveforms);
    osc_2_waveform_->setStringLookup(mopo::strings::waveforms);
    mono_lfo_1_waveform_->setStringLookup(mopo::strings::waveforms);
    mono_lfo_2_waveform_->setStringLookup(mopo::strings::waveforms);
    poly_lfo_waveform_->setStringLookup(mopo::strings::waveforms);

    filter_type_->setStringLookup(mopo::strings::filter_types);
}

void SynthesisInterface::markBipolarSliders() {
    osc_1_transpose_->setBipolar();
    osc_1_tune_->setBipolar();
    osc_2_transpose_->setBipolar();
    osc_2_tune_->setBipolar();
    osc_mix_->setBipolar();

    mono_lfo_1_amplitude_->setBipolar();
    mono_lfo_2_amplitude_->setBipolar();
    poly_lfo_amplitude_->setBipolar();

    osc_feedback_amount_->setBipolar();
    osc_feedback_transpose_->setBipolar();
    osc_feedback_tune_->setBipolar();

    keytrack_->setBipolar();
    fil_env_depth_->setBipolar();

    delay_feedback_->setBipolar();
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

void SynthesisInterface::createStepSequencerSliders() {
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
    step_sequencer_->setStepSliders(step_sequencer_sliders_);
    step_sequencer_->setNumStepsSlider(num_steps_);
    step_sequencer_->setName("step_sequencer_step");
}

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
                 variableInitialisers="" snapPixels="2" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="0">
    <RECT pos="266c 470 116 126" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="4 470 200 126" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="651c 408 126 58" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="651c 428r 126 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="651c 236 126 168" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="651c 257r 126 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="651c 4 126 229" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="714r 24r 126 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="328 470 126 126" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="454r 490r 126 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="456c 156 256 148" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="456c 176r 256 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="456c 4 256 148" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="164c 308 320 158" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="164c 4 320 204" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="164c 24r 320 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="521c 470 126 126" fill="solid: ff424242" hasStroke="0"/>
    <TEXT pos="227c 192 70 10" fill="solid: ff777777" hasStroke="0" text="TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="289c 192 40 10" fill="solid: ff777777" hasStroke="0" text="TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="164c 104 50 10" fill="solid: ff777777" hasStroke="0" text="MOD"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="164c 212 320 92" fill="solid: ff424242" hasStroke="0"/>
    <RECT pos="456c 308 256 158" fill="solid: ff424242" hasStroke="0"/>
    <TEXT pos="164c 14c 100 20" fill="solid: ffffffff" hasStroke="0" text="OSCILLATORS"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="62c 288 80 10" fill="solid: ff777777" hasStroke="0" text="TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="130c 288 50 10" fill="solid: ff777777" hasStroke="0" text="TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="198c 288 60 10" fill="solid: ff777777" hasStroke="0" text="AMOUNT"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="266c 288 92 10" fill="solid: ff777777" hasStroke="0" text="SATURATION"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="391c 470 84 20" fill="solid: ffffffff" hasStroke="0" text="MONO LFO 1"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="410c 586 30 10" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="456c 156 150 20" fill="solid: ffffffff" hasStroke="0" text="AMPLITUDE ENVELOPE"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="651c 236 44 20" fill="solid: ffffffff" hasStroke="0" text="DELAY"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="618c 376 60 10" fill="solid: ff777777" hasStroke="0" text="FEEDBACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="651c 312 60 10" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="687c 376 60 10" fill="solid: ff777777" hasStroke="0" text="DRY/WET"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="651c 408 60 20" fill="solid: ffffffff" hasStroke="0" text="VOLUME"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="651c 4 92 20" fill="solid: ffffffff" hasStroke="0" text="ARTICULATION"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="618c 88 60 10" fill="solid: ff777777" hasStroke="0" text="POLYPHONY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="618c 160 46 10" fill="solid: ff777777" hasStroke="0" text="PORTA"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="684c 214 60 10" fill="solid: ff777777" hasStroke="0" text="LEGATO"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="618c 214 60 10" fill="solid: ff777777" hasStroke="0" text="PORT TYPE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="684c 88 60 10" fill="solid: ff777777" hasStroke="0" text="PITCH BEND"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="4 622c 708 44" fill="solid: ff424242" hasStroke="0"/>
    <TEXT pos="58 622c 80 12" fill="solid: ff777777" hasStroke="0" text="PITCH WHEEL"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="211 622c 60 12" fill="solid: ff777777" hasStroke="0" text="MOD WHEEL"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="380 622c 40 12" fill="solid: ff777777" hasStroke="0" text="NOTE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="510 622c 60 12" fill="solid: ff777777" hasStroke="0" text="VELOCITY"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="640 622c 70 12" fill="solid: ff777777" hasStroke="0" text="AFTERTOUCH"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="404c 450 60 10" fill="solid: ff777777" hasStroke="0" text="STEPS"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="468c 450 60 12" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="684c 160 60 10" fill="solid: ff777777" hasStroke="0" text="VEL TRACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="287c 448 60 10" fill="solid: ff777777" hasStroke="0" text="KEY TRACK"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="39c 192 70 10" fill="solid: ff777777" hasStroke="0" text="TRANSPOSE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="101c 192 40 10" fill="solid: ff777777" hasStroke="0" text="TUNE"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="287c 384 56 10" fill="solid: ff777777" hasStroke="0" text="ENV DEPTH"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="538c 586 30 10" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="234c 560 40 10" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="298c 560 50 10" fill="solid: ff777777" hasStroke="0" text="RESAMPLE "
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="164c 232r 320 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="164c 212 84 20" fill="solid: ffffffff" hasStroke="0" text="FEEDBACK"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="456c 24r 256 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="456c 14c 140 20" fill="solid: ffffffff" hasStroke="0" text="FILTER ENVELOPE"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="456c 328r 256 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="456c 307 110 20" fill="solid: ffffffff" hasStroke="0" text="STEP SEQUENCER"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="521c 490r 126 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="521c 470 84 20" fill="solid: ffffffff" hasStroke="0" text="MONO LFO 2"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ffb14a06" nonZeroWinding="1">s 194 44 l 186 44 l 178 60</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ffb14a06" nonZeroWinding="1">s 134 44 l 142 44 l 150 60</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ffb14a06" nonZeroWinding="1">s 194 116 l 186 116 l 178 100</PATH>
    <PATH pos="0 0 100 100" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ffb14a06" nonZeroWinding="1">s 134 116 l 142 116 l 150 100</PATH>
    <RECT pos="104c 490r 200 20" fill="solid: ff009688" hasStroke="0"/>
    <RECT pos="266c 490r 116 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="266c 470 84 20" fill="solid: ffffffff" hasStroke="0" text="STUTTER"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="104c 470 84 20" fill="solid: ffffffff" hasStroke="0" text="FORMANT"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="540c 450 60 12" fill="solid: ff777777" hasStroke="0" text="SMOOTHING"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="668 511c 24 1" fill="solid: ffcccccc" hasStroke="0"/>
    <TEXT pos="164c 142 50 10" fill="solid: ff777777" hasStroke="0" text="MIX"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="164c 328r 320 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="164c 308 84 20" fill="solid: ffffffff" hasStroke="0" text="FILTER"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="649c 470 126 126" fill="solid: ff424242" hasStroke="0"/>
    <TEXT pos="668c 586 30 10" fill="solid: ff777777" hasStroke="0" text="FREQ"
          fontname="Default sans-serif font" fontsize="11.400000000000000355"
          bold="0" italic="0" justification="36"/>
    <RECT pos="651c 490r 126 20" fill="solid: ff009688" hasStroke="0"/>
    <TEXT pos="651c 470 84 20" fill="solid: ffffffff" hasStroke="0" text="POLY LFO"
          fontname="Default sans-serif font" fontsize="14.400000000000000355"
          bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <JUCERCOMP name="step_sequencer" id="83a23936a8f464b5" memberName="step_sequencer_"
             virtualName="GraphicalStepSequencer" explicitFocusOrder="0" pos="456c 328 256 80"
             sourceFile="graphical_step_sequencer.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="b4880edb8b39ec9d" memberName="amplitude_envelope_"
             virtualName="GraphicalEnvelope" explicitFocusOrder="0" pos="456c 176 256 88"
             sourceFile="graphical_envelope.cpp" constructorParams=""/>
  <JUCERCOMP name="" id="87feb60c88df4fcc" memberName="filter_envelope_" virtualName="GraphicalEnvelope"
             explicitFocusOrder="0" pos="456c 24 256 88" sourceFile="graphical_envelope.cpp"
             constructorParams=""/>
  <JUCERCOMP name="osc_1_wave_display" id="55100715382ea344" memberName="osc_1_wave_display_"
             virtualName="WaveViewer" explicitFocusOrder="0" pos="4 40 128 80"
             sourceFile="wave_viewer.cpp" constructorParams="256"/>
  <JUCERCOMP name="osc_2_wave_display" id="c0c3e4a3ab2f045f" memberName="osc_2_wave_display_"
             virtualName="WaveViewer" explicitFocusOrder="0" pos="196 40 128 80"
             sourceFile="wave_viewer.cpp" constructorParams="256"/>
  <SLIDER name="polyphony" id="952bde38857bdba7" memberName="polyphony_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="618c 40 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="1" max="32"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="portamento" id="9de85cc1c5f64eaa" memberName="portamento_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="618c 112 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-9" max="-1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitch_bend_range" id="e53afc6d1a04c708" memberName="pitch_bend_range_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="684c 40 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="48"
          int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cross_modulation" id="1d3e4b59d6e470fb" memberName="cross_modulation_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="164c 56 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="portamento_type" id="909956998c46045e" memberName="portamento_type_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="618c 192 40 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="2" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_mix" id="211f463b59b2454f" memberName="osc_mix_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="164c 120 320 16" bkgcol="ff303030"
          trackcol="ff9765bc" textboxoutline="0" min="0" max="1" int="0"
          style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_2_transpose" id="555c8ee21acbf804" memberName="osc_2_transpose_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="227c 144 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-48"
          max="48" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_2_tune" id="a8bc3bcffe7146f" memberName="osc_2_tune_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="289c 144 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="volume" id="7cc7edfbfc537ee7" memberName="volume_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="588 428 126 38" bkgcol="ff424242"
          trackcol="ff9765bc" textboxoutline="0" min="0" max="1" int="0"
          style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay_feedback" id="c89eb62eea2ab491" memberName="delay_feedback_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="618c 328 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay_dry_wet" id="dbc9d35179b5bac7" memberName="delay_dry_wet_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="687c 328 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="velocity_track" id="113bdc65c4c0f18f" memberName="velocity_track_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="684c 112 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_attack" id="f6cda312a5619a33" memberName="amp_attack_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="384 270 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_decay" id="6de7c8b3a5a4826d" memberName="amp_decay_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="432 270 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_release" id="86b41ada65ec49c0" memberName="amp_release_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="528 270 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="amp_sustain" id="3853f65a726f763" memberName="amp_sustain_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="480 270 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_attack" id="bc169bc84bd26782" memberName="fil_attack_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="384 118 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_decay" id="f5b3d1ec7fc18e1" memberName="fil_decay_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="432 118 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_release" id="c878127a7ada93f0" memberName="fil_release_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="528 118 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="4"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_sustain" id="5b25f915f3694f34" memberName="fil_sustain_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="480 118 32 32"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="resonance" id="6c1c82a2d59d4b6e" memberName="resonance_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="244 344 10 112"
          bkgcol="ff424242" trackcol="ff9765bc" textboxbkgd="0" textboxoutline="0"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_1_waveform" id="ea97519d003b4224" memberName="osc_1_waveform_"
          virtualName="WaveSelector" explicitFocusOrder="0" pos="4 24 128 16"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="ff424242"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="osc_2_waveform" id="a38d2af584df969a" memberName="osc_2_waveform_"
          virtualName="WaveSelector" explicitFocusOrder="0" pos="196 24 128 16"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="ff424242"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cutoff" id="4ccde767164ea675" memberName="cutoff_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="4 456 240 10" bkgcol="ff424242" trackcol="ff9765bc"
          textboxbkgd="0" textboxoutline="0" min="28" max="127" int="0"
          style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="fil_env_depth" id="ac3a5967de6a1a92" memberName="fil_env_depth_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="287c 336 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-128"
          max="128" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="keytrack" id="33bbd8b71aa721c1" memberName="keytrack_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="287c 400 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_feedback_transpose" id="e747becfc7a4f3f7" memberName="osc_feedback_transpose_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="62c 240 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-24"
          max="24" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_feedback_amount" id="a94cee38c880759c" memberName="osc_feedback_amount_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="198c 240 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="osc_feedback_tune" id="90dc1d31f03abf4e" memberName="osc_feedback_tune_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="130c 240 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="mono_lfo_1" id="24d32b65108fb2a5" memberName="mono_lfo_1_wave_display_"
             virtualName="WaveViewer" explicitFocusOrder="0" pos="338 500 116 48"
             sourceFile="wave_viewer.cpp" constructorParams="32"/>
  <SLIDER name="mono_lfo_1_waveform" id="4ed06bb2c6901afe" memberName="mono_lfo_1_waveform_"
          virtualName="WaveSelector" explicitFocusOrder="0" pos="338 490 116 10"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="ff424242"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="num_steps" id="8be29885961d7617" memberName="num_steps_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="404c 410 40 40"
          bkgcol="0" rotarysliderfill="7fffffff" textboxtext="ffffffff"
          min="1" max="32" int="1" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="step_frequency" id="788574c1265fb47" memberName="step_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="468c 410 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-5" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="mono_lfo_1_frequency" id="ebc4a57528113c39" memberName="mono_lfo_1_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="410c 548 36 36"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-7" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="filter_saturation" id="b5014a266e860882" memberName="filter_saturation_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="266c 240 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-20"
          max="60" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="formant_on" id="a27029ddc5597777" memberName="formant_on_"
                virtualName="" explicitFocusOrder="0" pos="8 472 16 16" txtcol="ff777777"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="1"/>
  <SLIDER name="legato" id="5974d3f0077190f" memberName="legato_" virtualName="TwytchSlider"
          explicitFocusOrder="0" pos="684c 192 40 16" bkgcol="ff333333"
          trackcol="ff9765bc" textboxoutline="ff777777" min="0" max="1"
          int="1" style="LinearBar" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="formant_xy_pad" id="202ea6e8e33b6ac7" memberName="formant_xy_pad_"
             virtualName="XYPad" explicitFocusOrder="0" pos="4 490 190 96"
             sourceFile="xy_pad.cpp" constructorParams=""/>
  <SLIDER name="formant_x" id="d182d63c43cb241f" memberName="formant_x_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="4 586 190 10"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="777777"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="formant_y" id="f9e64695877940a6" memberName="formant_y_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="194 490 10 96"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="777777"
          min="0" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="filter_type" id="8d1283d4f2ace0ec" memberName="filter_type_"
          virtualName="FilterSelector" explicitFocusOrder="0" pos="4 328 250 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <TEXTBUTTON name="filter_env" id="b1c0e1b81ba12955" memberName="filter_envelope_mod_"
              virtualName="" explicitFocusOrder="0" pos="338 118 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="amplitude_env" id="f8f7a3ca2ba5265d" memberName="amplitude_envelope_mod_"
              virtualName="" explicitFocusOrder="0" pos="338 270 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="step_sequencer" id="4fd19f52e690cd89" memberName="step_sequencer_mod_"
              virtualName="" explicitFocusOrder="0" pos="338 416 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="mono_lfo_1" id="1ea938f771b995ba" memberName="mono_lfo_1_mod_"
              virtualName="" explicitFocusOrder="0" pos="338 556 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="pitch_wheel" id="527add472856006" memberName="pitch_wheel_mod_"
              virtualName="" explicitFocusOrder="0" pos="16 606 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="mod_wheel" id="38b300e016b7fbb" memberName="mod_wheel_mod_"
              virtualName="" explicitFocusOrder="0" pos="169 606 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="note" id="f119ef83db8634e0" memberName="note_mod_" virtualName=""
              explicitFocusOrder="0" pos="338 608 32 32" buttonText="M" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="velocity" id="fc46bb54c2093224" memberName="velocity_mod_"
              virtualName="" explicitFocusOrder="0" pos="468 608 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="aftertouch" id="5e4c99bb63fbb5c6" memberName="aftertouch_mod_"
              virtualName="" explicitFocusOrder="0" pos="598 608 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="osc_1_tune" id="19b20e4d54ff8b49" memberName="osc_1_tune_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="101c 144 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-1" max="1"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="delay_frequency" id="1119b78679f3c8ca" memberName="delay_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="652c 264 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-2" max="5"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <JUCERCOMP name="mono_lfo_2" id="e0735b9f144e0bed" memberName="mono_lfo_2_wave_display_"
             virtualName="WaveViewer" explicitFocusOrder="0" pos="468 500 116 48"
             sourceFile="wave_viewer.cpp" constructorParams="32"/>
  <SLIDER name="mono_lfo_2_waveform" id="ffccb388bb3729c" memberName="mono_lfo_2_waveform_"
          virtualName="WaveSelector" explicitFocusOrder="0" pos="468 490 116 10"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="ff424242"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="osc_1_transpose" id="48a41a977b14ab08" memberName="osc_1_transpose_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="39c 144 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-48"
          max="48" int="1" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="mono_lfo_2_frequency" id="1b17dd2b0ad5e56" memberName="mono_lfo_2_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="538c 548 36 36"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-7" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="mono_lfo_2" id="c487df9f9aa001d1" memberName="mono_lfo_2_mod_"
              virtualName="" explicitFocusOrder="0" pos="468 556 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="stutter_frequency" id="ca5e95b9738f9459" memberName="stutter_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="233c 512 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="4" max="100"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="stutter_on" id="c0e460c164340d7e" memberName="stutter_on_"
                virtualName="" explicitFocusOrder="0" pos="212 472 16 16" txtcol="ff777777"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="1"/>
  <SLIDER name="stutter_resample_frequency" id="31e8e484b922575e" memberName="stutter_resample_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="297c 512 46 46"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0.5"
          max="20" int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="step_smoothing" id="d5721bf2c0177c0e" memberName="step_smoothing_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="540c 410 40 40"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="0" max="0.5"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="mono_lfo_1_sync" id="7e78ba17de9b444b" memberName="mono_lfo_1_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="438 548 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="mono_lfo_2_sync" id="238e79c809ab43c" memberName="mono_lfo_2_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="568 548 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="delay_sync" id="7f286b1ad8378afd" memberName="delay_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="680 280 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="step_sequencer_sync" id="98c6cd34a7ea0a45" memberName="step_sequencer_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="488 424 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <JUCERCOMP name="filter_response" id="e5ebb41c4c259ce1" memberName="filter_response_"
             virtualName="FilterResponse" explicitFocusOrder="0" pos="4 344 240 112"
             sourceFile="filter_response.cpp" constructorParams="300"/>
  <JUCERCOMP name="poly_lfo" id="461d37c0e9597851" memberName="poly_lfo_wave_display_"
             virtualName="WaveViewer" explicitFocusOrder="0" pos="598 500 116 48"
             sourceFile="wave_viewer.cpp" constructorParams="32"/>
  <SLIDER name="poly_lfo_waveform" id="572ca2d8fd3f790f" memberName="poly_lfo_waveform_"
          virtualName="WaveSelector" explicitFocusOrder="0" pos="598 490 116 10"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="ff424242"
          min="0" max="11" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="poly_lfo_frequency" id="4f2b1269ff66a38c" memberName="poly_lfo_frequency_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="668c 548 36 36"
          rotarysliderfill="7fffffff" textboxtext="ffffffff" min="-7" max="6"
          int="0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="poly_lfo" id="fdc09e5d1dd367d0" memberName="poly_lfo_mod_"
              virtualName="" explicitFocusOrder="0" pos="598 556 32 32" buttonText="M"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="poly_lfo_sync" id="eac50d626ad94f4e" memberName="poly_lfo_sync_"
          virtualName="TempoSelector" explicitFocusOrder="0" pos="698 548 16 16"
          bkgcol="ff333333" trackcol="ff9765bc" textboxoutline="ff777777"
          min="0" max="6" int="1" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1"/>
  <SLIDER name="mono_lfo_1_amplitude" id="2a0a50f49c1b39f7" memberName="mono_lfo_1_amplitude_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="328 500 10 48"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="777777"
          min="-1" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="mono_lfo_2_amplitude" id="c2ddbdac098db676" memberName="mono_lfo_2_amplitude_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="458 500 10 48"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="777777"
          min="-1" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="poly_lfo_amplitude" id="aef398613be0d620" memberName="poly_lfo_amplitude_"
          virtualName="TwytchSlider" explicitFocusOrder="0" pos="588 500 10 48"
          bkgcol="ff424242" trackcol="ff9765bc" textboxoutline="777777"
          min="-1" max="1" int="0" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
