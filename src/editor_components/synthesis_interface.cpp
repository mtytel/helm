/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "synthesis_interface.h"

#include <iomanip>
#include "full_interface.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

SynthesisInterface::SynthesisInterface(mopo::control_map controls) {
    addAndMakeVisible(step_sequencer_ = new GraphicalStepSequencer());
    addAndMakeVisible(amplitude_envelope_ = new GraphicalEnvelope());
    addAndMakeVisible(filter_envelope_ = new GraphicalEnvelope());
    addAndMakeVisible(osc_1_wave_display_ = new WaveViewer(256));
    addAndMakeVisible(osc_2_wave_display_ = new WaveViewer(256));
    addAndMakeVisible(polyphony_ = new SynthSlider("polyphony"));
    polyphony_->setRange(1, 32, 1);
    polyphony_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    polyphony_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    polyphony_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    polyphony_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    polyphony_->addListener(this);

    addAndMakeVisible(portamento_ = new SynthSlider("portamento"));
    portamento_->setRange(-9, -1, 0);
    portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    portamento_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    portamento_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    portamento_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    portamento_->addListener(this);

    addAndMakeVisible(pitch_bend_range_ = new SynthSlider("pitch_bend_range"));
    pitch_bend_range_->setRange(0, 48, 1);
    pitch_bend_range_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    pitch_bend_range_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    pitch_bend_range_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    pitch_bend_range_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    pitch_bend_range_->addListener(this);

    addAndMakeVisible(cross_modulation_ = new SynthSlider("cross_modulation"));
    cross_modulation_->setRange(0, 0.4, 0);
    cross_modulation_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    cross_modulation_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    cross_modulation_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    cross_modulation_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    cross_modulation_->addListener(this);

    addAndMakeVisible(portamento_type_ = new SynthSlider("portamento_type"));
    portamento_type_->setRange(0, 2, 1);
    portamento_type_->setSliderStyle(Slider::LinearBar);
    portamento_type_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    portamento_type_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    portamento_type_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    portamento_type_->setColour(Slider::textBoxOutlineColourId, Colour(0x00bbbbbb));
    portamento_type_->addListener(this);

    addAndMakeVisible(osc_mix_ = new SynthSlider("osc_mix"));
    osc_mix_->setRange(0, 1, 0);
    osc_mix_->setSliderStyle(Slider::LinearBar);
    osc_mix_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_mix_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    osc_mix_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    osc_mix_->setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
    osc_mix_->addListener(this);

    addAndMakeVisible(osc_2_transpose_ = new SynthSlider("osc_2_transpose"));
    osc_2_transpose_->setRange(-48, 48, 1);
    osc_2_transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_2_transpose_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_2_transpose_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_2_transpose_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_2_transpose_->addListener(this);

    addAndMakeVisible(osc_2_tune_ = new SynthSlider("osc_2_tune"));
    osc_2_tune_->setRange(-1, 1, 0);
    osc_2_tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_2_tune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_2_tune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_2_tune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_2_tune_->addListener(this);

    addAndMakeVisible(volume_ = new SynthSlider("volume"));
    volume_->setRange(0, 1, 0);
    volume_->setSliderStyle(Slider::LinearBar);
    volume_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    volume_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    volume_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    volume_->setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
    volume_->addListener(this);

    addAndMakeVisible(delay_feedback_ = new SynthSlider("delay_feedback"));
    delay_feedback_->setRange(-1, 1, 0);
    delay_feedback_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delay_feedback_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    delay_feedback_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    delay_feedback_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    delay_feedback_->addListener(this);

    addAndMakeVisible(delay_dry_wet_ = new SynthSlider("delay_dry_wet"));
    delay_dry_wet_->setRange(0, 1, 0);
    delay_dry_wet_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delay_dry_wet_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    delay_dry_wet_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    delay_dry_wet_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    delay_dry_wet_->addListener(this);

    addAndMakeVisible(velocity_track_ = new SynthSlider("velocity_track"));
    velocity_track_->setRange(-1, 1, 0);
    velocity_track_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    velocity_track_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    velocity_track_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    velocity_track_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    velocity_track_->addListener(this);

    addAndMakeVisible(amp_attack_ = new SynthSlider("amp_attack"));
    amp_attack_->setRange(0, 4, 0);
    amp_attack_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    amp_attack_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    amp_attack_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    amp_attack_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    amp_attack_->addListener(this);

    addAndMakeVisible(amp_decay_ = new SynthSlider("amp_decay"));
    amp_decay_->setRange(0, 4, 0);
    amp_decay_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    amp_decay_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    amp_decay_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    amp_decay_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    amp_decay_->addListener(this);

    addAndMakeVisible(amp_release_ = new SynthSlider("amp_release"));
    amp_release_->setRange(0, 4, 0);
    amp_release_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    amp_release_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    amp_release_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    amp_release_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    amp_release_->addListener(this);

    addAndMakeVisible(amp_sustain_ = new SynthSlider("amp_sustain"));
    amp_sustain_->setRange(0, 1, 0);
    amp_sustain_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    amp_sustain_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    amp_sustain_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    amp_sustain_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    amp_sustain_->addListener(this);

    addAndMakeVisible(fil_attack_ = new SynthSlider("fil_attack"));
    fil_attack_->setRange(0, 4, 0);
    fil_attack_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fil_attack_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    fil_attack_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    fil_attack_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    fil_attack_->addListener(this);

    addAndMakeVisible(fil_decay_ = new SynthSlider("fil_decay"));
    fil_decay_->setRange(0, 4, 0);
    fil_decay_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fil_decay_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    fil_decay_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    fil_decay_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    fil_decay_->addListener(this);

    addAndMakeVisible(fil_release_ = new SynthSlider("fil_release"));
    fil_release_->setRange(0, 4, 0);
    fil_release_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fil_release_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    fil_release_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    fil_release_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    fil_release_->addListener(this);

    addAndMakeVisible(fil_sustain_ = new SynthSlider("fil_sustain"));
    fil_sustain_->setRange(0, 1, 0);
    fil_sustain_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fil_sustain_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    fil_sustain_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    fil_sustain_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    fil_sustain_->addListener(this);

    addAndMakeVisible(resonance_ = new SynthSlider("resonance"));
    resonance_->setRange(0, 1, 0);
    resonance_->setSliderStyle(Slider::LinearBar);
    resonance_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    resonance_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    resonance_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    resonance_->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
    resonance_->setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
    resonance_->addListener(this);

    addAndMakeVisible(osc_1_waveform_ = new WaveSelector("osc_1_waveform"));
    osc_1_waveform_->setRange(0, 11, 1);
    osc_1_waveform_->setSliderStyle(Slider::LinearBar);
    osc_1_waveform_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    osc_1_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    osc_1_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    osc_1_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
    osc_1_waveform_->addListener(this);

    addAndMakeVisible(osc_2_waveform_ = new WaveSelector("osc_2_waveform"));
    osc_2_waveform_->setRange(0, 11, 1);
    osc_2_waveform_->setSliderStyle(Slider::LinearBar);
    osc_2_waveform_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    osc_2_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    osc_2_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    osc_2_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
    osc_2_waveform_->addListener(this);

    addAndMakeVisible(cutoff_ = new SynthSlider("cutoff"));
    cutoff_->setRange(28, 127, 0);
    cutoff_->setSliderStyle(Slider::LinearBar);
    cutoff_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    cutoff_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    cutoff_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    cutoff_->setColour(Slider::textBoxBackgroundColourId, Colour(0x00000000));
    cutoff_->setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
    cutoff_->addListener(this);

    addAndMakeVisible(fil_env_depth_ = new SynthSlider("fil_env_depth"));
    fil_env_depth_->setRange(-128, 128, 0);
    fil_env_depth_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    fil_env_depth_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    fil_env_depth_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    fil_env_depth_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    fil_env_depth_->addListener(this);

    addAndMakeVisible(keytrack_ = new SynthSlider("keytrack"));
    keytrack_->setRange(-1, 1, 0);
    keytrack_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    keytrack_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    keytrack_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    keytrack_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    keytrack_->addListener(this);

    addAndMakeVisible(osc_feedback_transpose_ = new SynthSlider("osc_feedback_transpose"));
    osc_feedback_transpose_->setRange(-24, 24, 1);
    osc_feedback_transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_transpose_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_feedback_transpose_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_feedback_transpose_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_feedback_transpose_->addListener(this);

    addAndMakeVisible(osc_feedback_amount_ = new SynthSlider("osc_feedback_amount"));
    osc_feedback_amount_->setRange(-1, 1, 0);
    osc_feedback_amount_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_amount_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_feedback_amount_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_feedback_amount_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_feedback_amount_->addListener(this);

    addAndMakeVisible(osc_feedback_tune_ = new SynthSlider("osc_feedback_tune"));
    osc_feedback_tune_->setRange(-1, 1, 0);
    osc_feedback_tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_feedback_tune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_feedback_tune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_feedback_tune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_feedback_tune_->addListener(this);

    addAndMakeVisible(mono_lfo_1_wave_display_ = new WaveViewer(32));
    addAndMakeVisible(mono_lfo_1_waveform_ = new WaveSelector("mono_lfo_1_waveform"));
    mono_lfo_1_waveform_->setRange(0, 11, 1);
    mono_lfo_1_waveform_->setSliderStyle(Slider::LinearBar);
    mono_lfo_1_waveform_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    mono_lfo_1_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    mono_lfo_1_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
    mono_lfo_1_waveform_->addListener(this);

    addAndMakeVisible(num_steps_ = new SynthSlider("num_steps"));
    num_steps_->setRange(1, 32, 1);
    num_steps_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    num_steps_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    num_steps_->setColour(Slider::backgroundColourId, Colour(0x00000000));
    num_steps_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    num_steps_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    num_steps_->addListener(this);

    addAndMakeVisible(step_frequency_ = new SynthSlider("step_frequency"));
    step_frequency_->setRange(-5, 6, 0);
    step_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    step_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    step_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    step_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    step_frequency_->addListener(this);

    addAndMakeVisible(mono_lfo_1_frequency_ = new SynthSlider("mono_lfo_1_frequency"));
    mono_lfo_1_frequency_->setRange(-7, 6, 0);
    mono_lfo_1_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_1_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    mono_lfo_1_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_1_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    mono_lfo_1_frequency_->addListener(this);

    addAndMakeVisible(filter_saturation_ = new SynthSlider("filter_saturation"));
    filter_saturation_->setRange(-20, 60, 0);
    filter_saturation_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filter_saturation_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    filter_saturation_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    filter_saturation_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    filter_saturation_->addListener(this);

    addAndMakeVisible(formant_on_ = new ToggleButton("formant_on"));
    formant_on_->setButtonText(String::empty);
    formant_on_->addListener(this);
    formant_on_->setToggleState(true, dontSendNotification);
    formant_on_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

    addAndMakeVisible(formant_xy_pad_ = new XYPad());
    addAndMakeVisible(formant_x_ = new SynthSlider("formant_x"));
    formant_x_->setRange(0, 1, 0);
    formant_x_->setSliderStyle(Slider::LinearBar);
    formant_x_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    formant_x_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    formant_x_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    formant_x_->setColour(Slider::textBoxOutlineColourId, Colour(0x00777777));
    formant_x_->addListener(this);

    addAndMakeVisible(formant_y_ = new SynthSlider("formant_y"));
    formant_y_->setRange(0, 1, 0);
    formant_y_->setSliderStyle(Slider::LinearBar);
    formant_y_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    formant_y_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    formant_y_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    formant_y_->setColour(Slider::textBoxOutlineColourId, Colour(0x00777777));
    formant_y_->addListener(this);

    addAndMakeVisible(filter_type_ = new FilterSelector("filter_type"));
    filter_type_->setRange(0, 6, 1);
    filter_type_->setSliderStyle(Slider::LinearBar);
    filter_type_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    filter_type_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    filter_type_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    filter_type_->setColour(Slider::textBoxOutlineColourId, Colour(0xffbbbbbb));
    filter_type_->addListener(this);

    addAndMakeVisible(osc_1_tune_ = new SynthSlider("osc_1_tune"));
    osc_1_tune_->setRange(-1, 1, 0);
    osc_1_tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_1_tune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_1_tune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_1_tune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_1_tune_->addListener(this);

    addAndMakeVisible(delay_frequency_ = new SynthSlider("delay_frequency"));
    delay_frequency_->setRange(-2, 5, 0);
    delay_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delay_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    delay_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    delay_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    delay_frequency_->addListener(this);

    addAndMakeVisible(mono_lfo_2_wave_display_ = new WaveViewer(32));
    addAndMakeVisible(mono_lfo_2_waveform_ = new WaveSelector("mono_lfo_2_waveform"));
    mono_lfo_2_waveform_->setRange(0, 11, 1);
    mono_lfo_2_waveform_->setSliderStyle(Slider::LinearBar);
    mono_lfo_2_waveform_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    mono_lfo_2_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    mono_lfo_2_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
    mono_lfo_2_waveform_->addListener(this);

    addAndMakeVisible(osc_1_transpose_ = new SynthSlider("osc_1_transpose"));
    osc_1_transpose_->setRange(-48, 48, 1);
    osc_1_transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_1_transpose_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_1_transpose_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_1_transpose_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_1_transpose_->addListener(this);

    addAndMakeVisible(mono_lfo_2_frequency_ = new SynthSlider("mono_lfo_2_frequency"));
    mono_lfo_2_frequency_->setRange(-7, 6, 0);
    mono_lfo_2_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_2_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    mono_lfo_2_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_2_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    mono_lfo_2_frequency_->addListener(this);

    addAndMakeVisible(stutter_frequency_ = new SynthSlider("stutter_frequency"));
    stutter_frequency_->setRange(4, 100, 0);
    stutter_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    stutter_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    stutter_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    stutter_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    stutter_frequency_->addListener(this);

    addAndMakeVisible(stutter_on_ = new ToggleButton("stutter_on"));
    stutter_on_->setButtonText(String::empty);
    stutter_on_->addListener(this);
    stutter_on_->setToggleState(true, dontSendNotification);
    stutter_on_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

    addAndMakeVisible(stutter_resample_frequency_ = new SynthSlider("stutter_resample_frequency"));
    stutter_resample_frequency_->setRange(0.5, 20, 0);
    stutter_resample_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    stutter_resample_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    stutter_resample_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    stutter_resample_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    stutter_resample_frequency_->addListener(this);

    addAndMakeVisible(step_smoothing_ = new SynthSlider("step_smoothing"));
    step_smoothing_->setRange(0, 0.5, 0);
    step_smoothing_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    step_smoothing_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    step_smoothing_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    step_smoothing_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    step_smoothing_->addListener(this);

    addAndMakeVisible(mono_lfo_1_sync_ = new TempoSelector("mono_lfo_1_sync"));
    mono_lfo_1_sync_->setRange(0, 6, 1);
    mono_lfo_1_sync_->setSliderStyle(Slider::LinearBar);
    mono_lfo_1_sync_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_sync_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    mono_lfo_1_sync_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    mono_lfo_1_sync_->setColour(Slider::textBoxOutlineColourId, Colour(0xffbbbbbb));
    mono_lfo_1_sync_->addListener(this);

    addAndMakeVisible(mono_lfo_2_sync_ = new TempoSelector("mono_lfo_2_sync"));
    mono_lfo_2_sync_->setRange(0, 6, 1);
    mono_lfo_2_sync_->setSliderStyle(Slider::LinearBar);
    mono_lfo_2_sync_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_sync_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    mono_lfo_2_sync_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    mono_lfo_2_sync_->setColour(Slider::textBoxOutlineColourId, Colour(0xffbbbbbb));
    mono_lfo_2_sync_->addListener(this);

    addAndMakeVisible(delay_sync_ = new TempoSelector("delay_sync"));
    delay_sync_->setRange(0, 6, 1);
    delay_sync_->setSliderStyle(Slider::LinearBar);
    delay_sync_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    delay_sync_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    delay_sync_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    delay_sync_->setColour(Slider::textBoxOutlineColourId, Colour(0xffbbbbbb));
    delay_sync_->addListener(this);

    addAndMakeVisible(step_sequencer_sync_ = new TempoSelector("step_sequencer_sync"));
    step_sequencer_sync_->setRange(0, 6, 1);
    step_sequencer_sync_->setSliderStyle(Slider::LinearBar);
    step_sequencer_sync_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    step_sequencer_sync_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    step_sequencer_sync_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    step_sequencer_sync_->setColour(Slider::textBoxOutlineColourId, Colour(0xffbbbbbb));
    step_sequencer_sync_->addListener(this);

    addAndMakeVisible(filter_response_ = new FilterResponse(300));
    addAndMakeVisible(poly_lfo_wave_display_ = new WaveViewer(32));
    addAndMakeVisible(poly_lfo_waveform_ = new WaveSelector("poly_lfo_waveform"));
    poly_lfo_waveform_->setRange(0, 11, 1);
    poly_lfo_waveform_->setSliderStyle(Slider::LinearBar);
    poly_lfo_waveform_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    poly_lfo_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    poly_lfo_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    poly_lfo_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
    poly_lfo_waveform_->addListener(this);

    addAndMakeVisible(poly_lfo_frequency_ = new SynthSlider("poly_lfo_frequency"));
    poly_lfo_frequency_->setRange(-7, 6, 0);
    poly_lfo_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    poly_lfo_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    poly_lfo_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    poly_lfo_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    poly_lfo_frequency_->addListener(this);

    addAndMakeVisible(poly_lfo_sync_ = new TempoSelector("poly_lfo_sync"));
    poly_lfo_sync_->setRange(0, 6, 1);
    poly_lfo_sync_->setSliderStyle(Slider::LinearBar);
    poly_lfo_sync_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    poly_lfo_sync_->setColour(Slider::backgroundColourId, Colour(0xff333333));
    poly_lfo_sync_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    poly_lfo_sync_->setColour(Slider::textBoxOutlineColourId, Colour(0xffbbbbbb));
    poly_lfo_sync_->addListener(this);

    addAndMakeVisible(mono_lfo_1_amplitude_ = new SynthSlider("mono_lfo_1_amplitude"));
    mono_lfo_1_amplitude_->setRange(-1, 1, 0);
    mono_lfo_1_amplitude_->setSliderStyle(Slider::LinearBar);
    mono_lfo_1_amplitude_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    mono_lfo_1_amplitude_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    mono_lfo_1_amplitude_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    mono_lfo_1_amplitude_->setColour(Slider::textBoxOutlineColourId, Colour(0x00777777));
    mono_lfo_1_amplitude_->addListener(this);

    addAndMakeVisible(mono_lfo_2_amplitude_ = new SynthSlider("mono_lfo_2_amplitude"));
    mono_lfo_2_amplitude_->setRange(-1, 1, 0);
    mono_lfo_2_amplitude_->setSliderStyle(Slider::LinearBar);
    mono_lfo_2_amplitude_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    mono_lfo_2_amplitude_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    mono_lfo_2_amplitude_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    mono_lfo_2_amplitude_->setColour(Slider::textBoxOutlineColourId, Colour(0x00777777));
    mono_lfo_2_amplitude_->addListener(this);

    addAndMakeVisible(poly_lfo_amplitude_ = new SynthSlider("poly_lfo_amplitude"));
    poly_lfo_amplitude_->setRange(-1, 1, 0);
    poly_lfo_amplitude_->setSliderStyle(Slider::LinearBar);
    poly_lfo_amplitude_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    poly_lfo_amplitude_->setColour(Slider::backgroundColourId, Colour(0xff303030));
    poly_lfo_amplitude_->setColour(Slider::trackColourId, Colour(0xff9765bc));
    poly_lfo_amplitude_->setColour(Slider::textBoxOutlineColourId, Colour(0x00777777));
    poly_lfo_amplitude_->addListener(this);

    addAndMakeVisible(osc_1_unison_detune_ = new SynthSlider("osc_1_unison_detune"));
    osc_1_unison_detune_->setRange(0, 200, 0);
    osc_1_unison_detune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_1_unison_detune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_1_unison_detune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_1_unison_detune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_1_unison_detune_->addListener(this);

    addAndMakeVisible(osc_2_unison_detune_ = new SynthSlider("osc_2_unison_detune"));
    osc_2_unison_detune_->setRange(0, 200, 0);
    osc_2_unison_detune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_2_unison_detune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_2_unison_detune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_2_unison_detune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_2_unison_detune_->addListener(this);

    addAndMakeVisible(osc_1_unison_voices_ = new SynthSlider("osc_1_unison_voices"));
    osc_1_unison_voices_->setRange(1, 15, 2);
    osc_1_unison_voices_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_1_unison_voices_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_1_unison_voices_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_1_unison_voices_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_1_unison_voices_->addListener(this);

    addAndMakeVisible(osc_2_unison_voices_ = new SynthSlider("osc_2_unison_voices"));
    osc_2_unison_voices_->setRange(1, 15, 2);
    osc_2_unison_voices_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    osc_2_unison_voices_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    osc_2_unison_voices_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    osc_2_unison_voices_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    osc_2_unison_voices_->addListener(this);

    addAndMakeVisible(filter_envelope_mod_ = new ModulationButton("filter_env"));
    filter_envelope_mod_->setButtonText(String::empty);
    filter_envelope_mod_->addListener(this);

    addAndMakeVisible(amplitude_envelope_mod_ = new ModulationButton("amplitude_env"));
    amplitude_envelope_mod_->setButtonText(String::empty);
    amplitude_envelope_mod_->addListener(this);

    addAndMakeVisible(step_sequencer_mod_ = new ModulationButton("step_sequencer"));
    step_sequencer_mod_->setButtonText(String::empty);
    step_sequencer_mod_->addListener(this);

    addAndMakeVisible(mono_lfo_1_mod_ = new ModulationButton("mono_lfo_1"));
    mono_lfo_1_mod_->setButtonText(String::empty);
    mono_lfo_1_mod_->addListener(this);

    addAndMakeVisible(mono_lfo_2_mod_ = new ModulationButton("mono_lfo_2"));
    mono_lfo_2_mod_->setButtonText(String::empty);
    mono_lfo_2_mod_->addListener(this);

    addAndMakeVisible(poly_lfo_mod_ = new ModulationButton("poly_lfo"));
    poly_lfo_mod_->setButtonText(String::empty);
    poly_lfo_mod_->addListener(this);

    addAndMakeVisible(aftertouch_mod_ = new ModulationButton("aftertouch"));
    aftertouch_mod_->setButtonText(String::empty);
    aftertouch_mod_->addListener(this);

    addAndMakeVisible(note_mod_ = new ModulationButton("note"));
    note_mod_->setButtonText(String::empty);
    note_mod_->addListener(this);

    addAndMakeVisible(velocity_mod_ = new ModulationButton("velocity"));
    velocity_mod_->setButtonText(String::empty);
    velocity_mod_->addListener(this);

    addAndMakeVisible(mod_wheel_mod_ = new ModulationButton("mod_wheel"));
    mod_wheel_mod_->setButtonText(String::empty);
    mod_wheel_mod_->addListener(this);

    addAndMakeVisible(pitch_wheel_mod_ = new ModulationButton("pitch_wheel"));
    pitch_wheel_mod_->setButtonText(String::empty);
    pitch_wheel_mod_->addListener(this);

    addAndMakeVisible(legato_ = new ToggleButton("legato"));
    legato_->setButtonText(String::empty);
    legato_->addListener(this);
    legato_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

    addAndMakeVisible(unison_1_harmonize_ = new ToggleButton("unison_1_harmonize"));
    unison_1_harmonize_->setButtonText(String::empty);
    unison_1_harmonize_->addListener(this);
    unison_1_harmonize_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

    addAndMakeVisible(unison_2_harmonize_ = new ToggleButton("unison_2_harmonize"));
    unison_2_harmonize_->setButtonText(String::empty);
    unison_2_harmonize_->addListener(this);
    unison_2_harmonize_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

    addAndMakeVisible(reverb_feedback_ = new SynthSlider("reverb_feedback"));
    reverb_feedback_->setRange(-1, 1, 0);
    reverb_feedback_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    reverb_feedback_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    reverb_feedback_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    reverb_feedback_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    reverb_feedback_->addListener(this);

    addAndMakeVisible(reverb_dry_wet_ = new SynthSlider("reverb_dry_wet"));
    reverb_dry_wet_->setRange(0, 1, 0);
    reverb_dry_wet_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    reverb_dry_wet_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    reverb_dry_wet_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    reverb_dry_wet_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    reverb_dry_wet_->addListener(this);

    addAndMakeVisible(reverb_damping_ = new SynthSlider("reverb_damping"));
    reverb_damping_->setRange(0, 1, 0);
    reverb_damping_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    reverb_damping_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
    reverb_damping_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    reverb_damping_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    reverb_damping_->addListener(this);

    createTempoSliders();
    setSliderUnits();
    markBipolarSliders();
    createStepSequencerSliders();
    setStyles();

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
        SynthSlider* slider = dynamic_cast<SynthSlider*>(getChildComponent(i));
        if (slider)
            slider_lookup_[slider->getName().toStdString()] = slider;

        Button* button = dynamic_cast<Button*>(getChildComponent(i));
        if (button)
            button_lookup_[button->getName().toStdString()] = button;
    }

    setDefaultDoubleClickValues();
    setAllValues(controls);
    setOpaque(true);
}

SynthesisInterface::~SynthesisInterface() {
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
    formant_xy_pad_ = nullptr;
    formant_x_ = nullptr;
    formant_y_ = nullptr;
    filter_type_ = nullptr;
    osc_1_tune_ = nullptr;
    delay_frequency_ = nullptr;
    mono_lfo_2_wave_display_ = nullptr;
    mono_lfo_2_waveform_ = nullptr;
    osc_1_transpose_ = nullptr;
    mono_lfo_2_frequency_ = nullptr;
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
    poly_lfo_sync_ = nullptr;
    mono_lfo_1_amplitude_ = nullptr;
    mono_lfo_2_amplitude_ = nullptr;
    poly_lfo_amplitude_ = nullptr;
    osc_1_unison_detune_ = nullptr;
    osc_2_unison_detune_ = nullptr;
    osc_1_unison_voices_ = nullptr;
    osc_2_unison_voices_ = nullptr;
    filter_envelope_mod_ = nullptr;
    amplitude_envelope_mod_ = nullptr;
    step_sequencer_mod_ = nullptr;
    mono_lfo_1_mod_ = nullptr;
    mono_lfo_2_mod_ = nullptr;
    poly_lfo_mod_ = nullptr;
    aftertouch_mod_ = nullptr;
    note_mod_ = nullptr;
    velocity_mod_ = nullptr;
    mod_wheel_mod_ = nullptr;
    pitch_wheel_mod_ = nullptr;
    legato_ = nullptr;
    unison_1_harmonize_ = nullptr;
    unison_2_harmonize_ = nullptr;
    reverb_feedback_ = nullptr;
    reverb_dry_wet_ = nullptr;
    reverb_damping_ = nullptr;

    for (Slider* slider : step_sequencer_sliders_)
      delete slider;
}

void SynthesisInterface::paint(Graphics& g) {
    static const DropShadow section_shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
    static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
    static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                             BinaryData::RobotoRegular_ttfSize));
    static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                               BinaryData::RobotoLight_ttfSize));

    step_sequencer_->showRealtimeFeedback();
    mono_lfo_1_wave_display_->showRealtimeFeedback();
    mono_lfo_2_wave_display_->showRealtimeFeedback();

    g.setColour(Colour(0xff212121));
    g.fillRect(getLocalBounds());

    section_shadow.drawForRectangle(g, Rectangle<int>(8, 630 - (44 / 2), 722, 44));
    section_shadow.drawForRectangle(g, Rectangle<int>(8, 482, 196, 118));
    section_shadow.drawForRectangle(g, Rectangle<int>(270 - (116 / 2), 482, 116, 118));
    section_shadow.drawForRectangle(g, Rectangle<int>(336, 482, 126, 118));
    section_shadow.drawForRectangle(g, Rectangle<int>(533 - (126 / 2), 482, 126, 118));
    section_shadow.drawForRectangle(g, Rectangle<int>(604, 416, 126, 58));
    section_shadow.drawForRectangle(g, Rectangle<int>(604, 232, 126, 84));
    section_shadow.drawForRectangle(g, Rectangle<int>(604, 4, 126, 220));
    section_shadow.drawForRectangle(g, Rectangle<int>(604, 482, 126, 118));
    section_shadow.drawForRectangle(g, Rectangle<int>(604, 324, 126, 20));
    section_shadow.drawForRectangle(g, Rectangle<int>(336, 316, 260, 158));
    section_shadow.drawForRectangle(g, Rectangle<int>(328 - 320, 316, 320, 158));
    section_shadow.drawForRectangle(g, Rectangle<int>(168 - (320 / 2), 216, 320, 92));
    section_shadow.drawForRectangle(g, Rectangle<int>(336, 160, 260, 148));
    section_shadow.drawForRectangle(g, Rectangle<int>(336, 4, 260, 148));
    section_shadow.drawForRectangle(g, Rectangle<int>(168 - (320 / 2), 4, 320, 200));

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(static_cast<float>(168 - (320 / 2)), 4.0f, 320.0f, 204.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 22.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 26.0f,
                                       false));
    g.fillRect(168 - (320 / 2), 4, 320, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(336.0f, 316.0f, 260.0f, 158.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 334.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 338.0f,
                                       false));
    g.fillRect(466 - (260 / 2), 316, 260, 20);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("STEP SEQUENCER"),
                466 - (260 / 2), 315, 260, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(336.0f, 160.0f, 260.0f, 148.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 178.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 182.0f,
                                       false));
    g.fillRect(336, 160, 260, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(336.0f, 4.0f, 260.0f, 148.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 22.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 26.0f,
                                       false));
    g.fillRect(336, 4, 260, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(604.0f, 4.0f, 126.0f, 220.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 22.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 26.0f,
                                       false));
    g.fillRect(604, 4, 126, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(604.0f, 232.0f, 126.0f, 84.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 250.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 254.0f,
                                       false));
    g.fillRect(604, 232, 126, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(604.0f, 324.0f, 126.0f, 84.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 342.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 346.0f,
                                       false));
    g.fillRect(604, 324, 126, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(604.0f, 482.0f, 126.0f, 118.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                       false));
    g.fillRect(604, 482, 126, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(470.0f, 482.0f, 126.0f, 118.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                       false));
    g.fillRect(470, 482, 126, 20);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(336.0f, 482.0f, 126.0f, 118.0f, 3.000f);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                       false));
    g.fillRect(336, 482, 126, 20);

    g.setColour(Colour(0xff363636));
    g.fillEllipse(static_cast<float>(36 - (40 / 2)), static_cast<float>(172 - (40 / 2)), 40.0f, 40.0f);

    g.setColour(Colour(0xff424242));
    g.fillRect(8, 120, 320, 16);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(8.0f, static_cast<float>(630 - (44 / 2)), 722.0f, 44.0f, 3.000f);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(604.0f, 416.0f, 126.0f, 58.0f, 3.000f);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(static_cast<float>(270 - (116 / 2)), 482.0f, 116.0f, 118.0f, 3.000f);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(8.0f, 482.0f, 196.0f, 118.0f, 3.000f);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(static_cast<float>(168 - (320 / 2)), 316.0f, 320.0f, 158.0f, 3.000f);

    g.setColour(Colour(0xff303030));
    g.fillRoundedRectangle(static_cast<float>(168 - (320 / 2)), 216.0f, 320.0f, 92.0f, 3.000f);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("MOD"),
                168 - (50 / 2), 106, 50, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("OSCILLATORS"),
                168 - (320 / 2), 14 - (20 / 2), 320, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("TRANSPOSE"),
                66 - (80 / 2), 290, 80, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("TUNE"),
                134 - (50 / 2), 290, 50, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("AMOUNT"),
                202 - (60 / 2), 290, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("SATURATION"),
                270 - (92 / 2), 290, 92, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("MONO LFO 1"),
                399 - (126 / 2), 482, 126, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FREQUENCY"),
                423 - (60 / 2), 584, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("AMPLITUDE ENVELOPE"),
                466 - (260 / 2), 160, 260, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("DELAY"),
                667 - (126 / 2), 232, 126, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FEEDB"),
                626 - (40 / 2), 300, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FREQUENCY"),
                667 - (60 / 2), 276, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("WET"),
                708 - (30 / 2), 300, 30, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("VOLUME"),
                667 - (60 / 2), 416, 60, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("ARTICULATION"),
                667 - (126 / 2), 4, 126, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("POLYPHONY"),
                634 - (60 / 2), 86, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("PORTA"),
                634 - (46 / 2), 154, 46, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("LEGATO"),
                700 - (60 / 2), 204, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("PORT TYPE"),
                634 - (60 / 2), 204, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("PITCH BEND"),
                700 - (60 / 2), 86, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("PITCH WHEEL"),
                62, 630 - (12 / 2), 80, 12,
                Justification::centredLeft, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("MOD WHEEL"),
                215, 630 - (12 / 2), 60, 12,
                Justification::centredLeft, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("NOTE"),
                388, 630 - (12 / 2), 40, 12,
                Justification::centredLeft, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("VELOCITY"),
                522, 630 - (12 / 2), 60, 12,
                Justification::centredLeft, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("AFTERTOUCH"),
                656, 630 - (12 / 2), 70, 12,
                Justification::centredLeft, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("STEPS"),
                424 - (42 / 2), 460, 42, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FREQUENCY"),
                492 - (60 / 2), 460, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("VEL TRACK"),
                700 - (60 / 2), 154, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("KEY TRACK"),
                293 - (60 / 2), 458, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("ENV DEPTH"),
                293 - (56 / 2), 394, 56, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FREQUENCY"),
                556 - (60 / 2), 584, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FREQ"),
                241 - (40 / 2), 570, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("RESAMPLE "),
                297 - (50 / 2), 570, 50, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("FEEDBACK"),
                168 - (84 / 2), 216, 84, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("FILTER ENVELOPE"),
                466 - (260 / 2), 14 - (20 / 2), 260, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("MONO LFO 2"),
                533 - (126 / 2), 482, 126, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff4fc3f7));
    g.strokePath(internalPath1, PathStrokeType(1.000f));

    g.setColour(Colour(0xff4fc3f7));
    g.strokePath(internalPath2, PathStrokeType(1.000f));

    g.setColour(Colour(0xff4fc3f7));
    g.strokePath(internalPath3, PathStrokeType(1.000f));

    g.setColour(Colour(0xff4fc3f7));
    g.strokePath(internalPath4, PathStrokeType(1.000f));

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("STUTTER"),
                270 - (84 / 2), 482, 84, 20,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("FORMANT"),
                106 - (84 / 2), 482, 84, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("MIX"),
                168 - (50 / 2), 140, 50, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("FILTER"),
                168 - (84 / 2), 316, 84, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FREQUENCY"),
                690 - (60 / 2), 584, 60, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("POLY LFO"),
                667 - (126 / 2), 482, 126, 20,
                Justification::centred, true);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 234.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 238.0f,
                                       false));
    g.fillRect(168 - (320 / 2), 216, 320, 20);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 334.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 338.0f,
                                       false));
    g.fillRect(168 - (320 / 2), 316, 320, 20);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                       false));
    g.fillRect(212, 482, 116, 20);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                       false));
    g.fillRect(106 - (196 / 2), 482, 196, 20);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 434.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 438.0f,
                                       false));
    g.fillRect(604, 416, 126, 20);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 134.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 138.0f,
                                       false));
    g.fillRect(168 - (320 / 2), 120, 320, 16);

    g.setGradientFill(ColourGradient(Colour(0x00000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 122.0f,
                                       Colour(0x77000000),
                                       static_cast<float>(proportionOfWidth(0.0000f)), 118.0f,
                                       false));
    g.fillRect(168 - (320 / 2), 120, 320, 16);

    g.setColour(Colour(0xff999999));
    g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
    g.drawText(TRANS("REVERB"),
                667 - (126 / 2), 324, 126, 20,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("FEEDB"),
                626 - (40 / 2), 392, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("DAMP"),
                667 - (40 / 2), 392, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("WET"),
                708 - (30 / 2), 392, 30, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("UNISON"),
                130 - (40 / 2), 176, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("UNISON"),
                206 - (40 / 2), 176, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(Font(Font::getDefaultSansSerifFontName(), 9.00f, Font::plain));
    g.drawText(TRANS("OFF"),
                614 - (28 / 2), 172, 28, 12,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(Font(Font::getDefaultSansSerifFontName(), 9.00f, Font::plain));
    g.drawText(TRANS("AUTO"),
                634 - (28 / 2), 172, 28, 12,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(Font(Font::getDefaultSansSerifFontName(), 9.00f, Font::plain));
    g.drawText(TRANS("ON"),
                654 - (28 / 2), 172, 28, 12,
                Justification::centred, true);

    g.setColour(Colour(0xff363636));
    g.fillEllipse(static_cast<float>(260 - (50 / 2)), static_cast<float>(168 - (50 / 2)), 50.0f, 50.0f);

    g.setColour(Colour(0xff363636));
    g.fillEllipse(static_cast<float>(300 - (40 / 2)), static_cast<float>(172 - (40 / 2)), 40.0f, 40.0f);

    g.setColour(Colour(0xff303030));
    g.fillRect(224, 188, 100, 16);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("TRANS"),
                260 - (40 / 2), 192, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("TUNE"),
                300 - (40 / 2), 192, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xff363636));
    g.fillEllipse(static_cast<float>(76 - (50 / 2)), static_cast<float>(168 - (50 / 2)), 50.0f, 50.0f);

    g.setColour(Colour(0xff303030));
    g.fillRect(16, 188, 100, 16);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("TUNE"),
                36 - (40 / 2), 192, 40, 10,
                Justification::centred, true);

    g.setColour(Colour(0xffbbbbbb));
    g.setFont(roboto_reg.withPointHeight(10.0f));
    g.drawText(TRANS("TRANS"),
                76 - (40 / 2), 192, 40, 10,
                Justification::centred, true);

    component_shadow.drawForRectangle(g, filter_response_->getBounds());
    component_shadow.drawForRectangle(g, filter_envelope_->getBounds());
    component_shadow.drawForRectangle(g, amplitude_envelope_->getBounds());
    component_shadow.drawForRectangle(g, step_sequencer_->getBounds());
    component_shadow.drawForRectangle(g, formant_xy_pad_->getBounds());
    component_shadow.drawForRectangle(g, mono_lfo_1_wave_display_->getBounds());
    component_shadow.drawForRectangle(g, mono_lfo_2_wave_display_->getBounds());
    component_shadow.drawForRectangle(g, poly_lfo_wave_display_->getBounds());
    component_shadow.drawForRectangle(g, osc_1_wave_display_->getBounds());
    component_shadow.drawForRectangle(g, osc_2_wave_display_->getBounds());

    for (auto slider : slider_lookup_)
        slider.second->drawShadow(g);
}

void SynthesisInterface::resized() {
    step_sequencer_->setBounds(336, 336, 260, 96);
    amplitude_envelope_->setBounds(336, 180, 260, 88);
    filter_envelope_->setBounds(336, 24, 260, 88);
    osc_1_wave_display_->setBounds(8, 40, 128, 80);
    osc_2_wave_display_->setBounds(200, 40, 128, 80);
    polyphony_->setBounds(634 - (40 / 2), 42, 40, 40);
    portamento_->setBounds(634 - (40 / 2), 110, 40, 40);
    pitch_bend_range_->setBounds(700 - (40 / 2), 42, 40, 40);
    cross_modulation_->setBounds(168 - (40 / 2), 60, 40, 40);
    portamento_type_->setBounds(634 - (45 / 2), 186, 45, 16);
    osc_mix_->setBounds(168 - (320 / 2), 120, 320, 16);
    osc_2_transpose_->setBounds(260 - (40 / 2), 148, 40, 40);
    osc_2_tune_->setBounds(300 - (32 / 2), 156, 32, 32);
    volume_->setBounds(604, 436, 126, 38);
    delay_feedback_->setBounds(626 - (32 / 2), 270, 32, 32);
    delay_dry_wet_->setBounds(708 - (32 / 2), 270, 32, 32);
    velocity_track_->setBounds(700 - (40 / 2), 110, 40, 40);
    amp_attack_->setBounds(392, 272, 32, 32);
    amp_decay_->setBounds(440, 272, 32, 32);
    amp_release_->setBounds(536, 272, 32, 32);
    amp_sustain_->setBounds(488, 272, 32, 32);
    fil_attack_->setBounds(392, 116, 32, 32);
    fil_decay_->setBounds(440, 116, 32, 32);
    fil_release_->setBounds(536, 116, 32, 32);
    fil_sustain_->setBounds(488, 116, 32, 32);
    resonance_->setBounds(258 - 10, 352, 10, 112);
    osc_1_waveform_->setBounds(8, 24, 128, 16);
    osc_2_waveform_->setBounds(200, 24, 128, 16);
    cutoff_->setBounds(8, 464, 240, 10);
    fil_env_depth_->setBounds(293 - (40 / 2), 348, 40, 40);
    keytrack_->setBounds(293 - (40 / 2), 452 - 40, 40, 40);
    osc_feedback_transpose_->setBounds(66 - (40 / 2), 244, 40, 40);
    osc_feedback_amount_->setBounds(202 - (40 / 2), 244, 40, 40);
    osc_feedback_tune_->setBounds(134 - (40 / 2), 244, 40, 40);
    mono_lfo_1_wave_display_->setBounds(346, 512, 116, 48);
    mono_lfo_1_waveform_->setBounds(346, 502, 116, 10);
    num_steps_->setBounds(424 - (42 / 2), 440, 42, 16);
    step_frequency_->setBounds(463, 440, 42, 16);
    mono_lfo_1_frequency_->setBounds(394, 564, 42, 16);
    filter_saturation_->setBounds(270 - (40 / 2), 244, 40, 40);
    formant_on_->setBounds(12, 484, 16, 16);
    formant_xy_pad_->setBounds(8, 502, 186, 88);
    formant_x_->setBounds(8, 590, 186, 10);
    formant_y_->setBounds(194, 502, 10, 88);
    filter_type_->setBounds(8, 336, 250, 16);
    osc_1_tune_->setBounds(36 - (32 / 2), 156, 32, 32);
    delay_frequency_->setBounds(659 - (42 / 2), 256, 42, 16);
    mono_lfo_2_wave_display_->setBounds(480, 512, 116, 48);
    mono_lfo_2_waveform_->setBounds(478, 502, 116, 10);
    osc_1_transpose_->setBounds(76 - (40 / 2), 148, 40, 40);
    mono_lfo_2_frequency_->setBounds(527, 564, 42, 16);
    stutter_frequency_->setBounds(241 - (40 / 2), 524, 40, 40);
    stutter_on_->setBounds(216, 484, 16, 16);
    stutter_resample_frequency_->setBounds(297 - (40 / 2), 524, 40, 40);
    step_smoothing_->setBounds(560 - (32 / 2), 436, 32, 32);
    mono_lfo_1_sync_->setBounds(436, 564, 16, 16);
    mono_lfo_2_sync_->setBounds(570, 564, 16, 16);
    delay_sync_->setBounds(680, 256, 16, 16);
    step_sequencer_sync_->setBounds(505, 440, 16, 16);
    filter_response_->setBounds(8, 352, 240, 112);
    poly_lfo_wave_display_->setBounds(614, 514, 116, 48);
    poly_lfo_waveform_->setBounds(614, 504, 116, 10);
    poly_lfo_frequency_->setBounds(661, 564, 42, 16);
    poly_lfo_sync_->setBounds(704, 564, 16, 16);
    mono_lfo_1_amplitude_->setBounds(336, 512, 10, 48);
    mono_lfo_2_amplitude_->setBounds(470, 512, 10, 48);
    poly_lfo_amplitude_->setBounds(604, 514, 10, 48);
    osc_1_unison_detune_->setBounds(130 - (36 / 2), 144, 36, 36);
    osc_2_unison_detune_->setBounds(206 - (36 / 2), 144, 36, 36);
    osc_1_unison_voices_->setBounds(122 - (36 / 2), 188, 36, 16);
    osc_2_unison_voices_->setBounds(214 - (36 / 2), 188, 36, 16);
    filter_envelope_mod_->setBounds(346, 116, 32, 32);
    amplitude_envelope_mod_->setBounds(346, 272, 32, 32);
    step_sequencer_mod_->setBounds(346, 438, 32, 32);
    mono_lfo_1_mod_->setBounds(346, 564, 32, 32);
    mono_lfo_2_mod_->setBounds(480, 568, 32, 32);
    poly_lfo_mod_->setBounds(614, 568, 32, 32);
    aftertouch_mod_->setBounds(614, 614, 32, 32);
    note_mod_->setBounds(346, 614, 32, 32);
    velocity_mod_->setBounds(480, 614, 32, 32);
    mod_wheel_mod_->setBounds(173, 614, 32, 32);
    pitch_wheel_mod_->setBounds(20, 614, 32, 32);
    legato_->setBounds(684, 186, 32, 16);
    unison_1_harmonize_->setBounds(140, 188, 16, 16);
    unison_2_harmonize_->setBounds(180, 188, 16, 16);
    reverb_feedback_->setBounds(626 - (32 / 2), 358, 32, 32);
    reverb_dry_wet_->setBounds(708 - (32 / 2), 358, 32, 32);
    reverb_damping_->setBounds(667 - (32 / 2), 356, 32, 32);
    internalPath1.clear();
    internalPath1.startNewSubPath(198.0f, 44.0f);
    internalPath1.lineTo(190.0f, 44.0f);
    internalPath1.lineTo(182.0f, 60.0f);

    internalPath2.clear();
    internalPath2.startNewSubPath(138.0f, 44.0f);
    internalPath2.lineTo(146.0f, 44.0f);
    internalPath2.lineTo(154.0f, 60.0f);

    internalPath3.clear();
    internalPath3.startNewSubPath(198.0f, 116.0f);
    internalPath3.lineTo(190.0f, 116.0f);
    internalPath3.lineTo(182.0f, 100.0f);

    internalPath4.clear();
    internalPath4.startNewSubPath(138.0f, 116.0f);
    internalPath4.lineTo(146.0f, 116.0f);
    internalPath4.lineTo(154.0f, 100.0f);

    step_sequencer_tempo_->setBounds(step_frequency_->getBounds());
    mono_lfo_1_tempo_->setBounds(mono_lfo_1_frequency_->getBounds());
    mono_lfo_2_tempo_->setBounds(mono_lfo_2_frequency_->getBounds());
    poly_lfo_tempo_->setBounds(poly_lfo_frequency_->getBounds());
    delay_tempo_->setBounds(delay_frequency_->getBounds());
}

void SynthesisInterface::sliderValueChanged(Slider* sliderThatWasMoved) {
    std::string name = sliderThatWasMoved->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->valueChanged(name, sliderThatWasMoved->getValue());
}

void SynthesisInterface::buttonClicked(Button* buttonThatWasClicked) {
    std::string name = buttonThatWasClicked->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent == nullptr)
        return;

    if (buttonThatWasClicked == formant_on_)
        parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    else if (buttonThatWasClicked == stutter_on_)
        parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    else if (buttonThatWasClicked == legato_)
        parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    else if (buttonThatWasClicked == unison_1_harmonize_)
        parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    else if (buttonThatWasClicked == unison_2_harmonize_)
        parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
    else {
        std::string name = buttonThatWasClicked->getName().toStdString();
        FullInterface* full_parent = findParentComponentOfClass<FullInterface>();
        if (full_parent) {
            if (buttonThatWasClicked->getToggleState()) {
                std::string current_modulator = full_parent->getCurrentModulator();
                if (current_modulator != "") {
                    Button* modulator = button_lookup_[current_modulator];
                    modulator->setToggleState(false, NotificationType::dontSendNotification);
                }
                full_parent->changeModulator(name);
            }
            else
                full_parent->forgetModulator();
        }
    }
}

void SynthesisInterface::createTempoSliders() {
    step_sequencer_tempo_ = new SynthSlider("step_sequencer_tempo");
    addAndMakeVisible(step_sequencer_tempo_);
    int num_tempos = sizeof(mopo::synced_freq_ratios) / sizeof(mopo::Value);
    step_sequencer_tempo_->setRange(0, num_tempos - 1, 1);
    step_sequencer_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    step_sequencer_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    step_sequencer_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    step_sequencer_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    step_sequencer_tempo_->addListener(this);

    mono_lfo_1_tempo_ = new SynthSlider("mono_lfo_1_tempo");
    addAndMakeVisible(mono_lfo_1_tempo_);
    mono_lfo_1_tempo_->setRange(0, num_tempos - 1, 1);
    mono_lfo_1_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_1_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_1_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_1_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    mono_lfo_1_tempo_->addListener(this);

    mono_lfo_2_tempo_ = new SynthSlider("mono_lfo_2_tempo");
    addAndMakeVisible(mono_lfo_2_tempo_);
    mono_lfo_2_tempo_->setRange(0, num_tempos - 1, 1);
    mono_lfo_2_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    mono_lfo_2_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mono_lfo_2_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    mono_lfo_2_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    mono_lfo_2_tempo_->addListener(this);

    poly_lfo_tempo_ = new SynthSlider("poly_lfo_tempo");
    addAndMakeVisible(poly_lfo_tempo_);
    poly_lfo_tempo_->setRange(0, num_tempos - 1, 1);
    poly_lfo_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    poly_lfo_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    poly_lfo_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    poly_lfo_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    poly_lfo_tempo_->addListener(this);

    delay_tempo_ = new SynthSlider("delay_tempo");
    addAndMakeVisible(delay_tempo_);
    delay_tempo_->setRange(0, num_tempos - 1, 1);
    delay_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    delay_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    delay_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
    delay_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
    delay_tempo_->addListener(this);
}

void SynthesisInterface::setSliderUnits() {
    osc_1_transpose_->setUnits("semitones");
    osc_2_transpose_->setUnits("semitones");
    osc_1_tune_->setPostMultiply(100.0);
    osc_1_tune_->setUnits("cents");
    osc_2_tune_->setPostMultiply(100.0);
    osc_2_tune_->setUnits("cents");

    osc_1_unison_voices_->setUnits("v");
    osc_2_unison_voices_->setUnits("v");
    osc_1_unison_detune_->setUnits("cents");
    osc_2_unison_detune_->setUnits("cents");
    osc_feedback_transpose_->setUnits("semitones");
    osc_feedback_tune_->setUnits("cents");
    osc_feedback_tune_->setPostMultiply(100.0);
    filter_saturation_->setUnits("dB");

    fil_env_depth_->setUnits("semitones");
    portamento_->setUnits("secs/semitone");
    portamento_->setScalingType(SynthSlider::kExponential);
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
    mono_lfo_1_frequency_->setScalingType(SynthSlider::kExponential);
    mono_lfo_2_frequency_->setScalingType(SynthSlider::kExponential);
    poly_lfo_frequency_->setScalingType(SynthSlider::kExponential);
    step_frequency_->setScalingType(SynthSlider::kExponential);
    delay_frequency_->setScalingType(SynthSlider::kExponential);

    amp_attack_->setUnits("secs");
    amp_decay_->setUnits("secs");
    amp_release_->setUnits("secs");
    fil_attack_->setUnits("secs");
    fil_decay_->setUnits("secs");
    fil_release_->setUnits("secs");
    amp_attack_->setScalingType(SynthSlider::kPolynomial);
    amp_decay_->setScalingType(SynthSlider::kPolynomial);
    amp_release_->setScalingType(SynthSlider::kPolynomial);
    fil_attack_->setScalingType(SynthSlider::kPolynomial);
    fil_decay_->setScalingType(SynthSlider::kPolynomial);
    fil_release_->setScalingType(SynthSlider::kPolynomial);

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

void SynthesisInterface::setStyles() {
    static const int UNISON_DRAG_SENSITIVITY = 100;
    static const int TEMPO_DRAG_SENSITIVITY = 150;

    osc_1_unison_voices_->setLookAndFeel(TextLookAndFeel::instance());
    osc_1_unison_voices_->setMouseDragSensitivity(UNISON_DRAG_SENSITIVITY);
    osc_2_unison_voices_->setLookAndFeel(TextLookAndFeel::instance());
    osc_1_unison_voices_->setMouseDragSensitivity(UNISON_DRAG_SENSITIVITY);

    mono_lfo_1_frequency_->setLookAndFeel(TextLookAndFeel::instance());
    mono_lfo_2_frequency_->setLookAndFeel(TextLookAndFeel::instance());
    poly_lfo_frequency_->setLookAndFeel(TextLookAndFeel::instance());
    step_frequency_->setLookAndFeel(TextLookAndFeel::instance());

    mono_lfo_1_tempo_->setLookAndFeel(TextLookAndFeel::instance());
    mono_lfo_1_tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);
    mono_lfo_2_tempo_->setLookAndFeel(TextLookAndFeel::instance());
    mono_lfo_2_tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);
    poly_lfo_tempo_->setLookAndFeel(TextLookAndFeel::instance());
    poly_lfo_tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);
    step_sequencer_tempo_->setLookAndFeel(TextLookAndFeel::instance());
    step_sequencer_tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);

    num_steps_->setLookAndFeel(TextLookAndFeel::instance());
    legato_->setLookAndFeel(TextLookAndFeel::instance());
    unison_1_harmonize_->setLookAndFeel(TextLookAndFeel::instance());
    unison_2_harmonize_->setLookAndFeel(TextLookAndFeel::instance());
    delay_frequency_->setLookAndFeel(TextLookAndFeel::instance());
    delay_tempo_->setLookAndFeel(TextLookAndFeel::instance());

    filter_envelope_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    amplitude_envelope_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    step_sequencer_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    mono_lfo_1_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    mono_lfo_2_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    poly_lfo_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    aftertouch_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    note_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    velocity_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    mod_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
    pitch_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

    resonance_->setSliderStyle(Slider::LinearBarVertical);
    formant_y_->setSliderStyle(Slider::LinearBarVertical);
    mono_lfo_1_amplitude_->setSliderStyle(Slider::LinearBarVertical);
    mono_lfo_2_amplitude_->setSliderStyle(Slider::LinearBarVertical);
    poly_lfo_amplitude_->setSliderStyle(Slider::LinearBarVertical);
}

void SynthesisInterface::createStepSequencerSliders() {
    for (int i = 0; i < num_steps_->getMaximum(); ++i) {
        String num(i);
        if (num.length() == 1)
            num = String("0") + num;

        SynthSlider* step = new SynthSlider(String("step_seq_") + num);
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
        button.second->repaint();
    }
    repaint();
}

void SynthesisInterface::setValue(std::string name, mopo::mopo_float value,
                                  NotificationType notification) {
    if (slider_lookup_.count(name))
        slider_lookup_[name]->setValue(value, notification);
}

void SynthesisInterface::modulationChanged(std::string source) {
    button_lookup_[source]->repaint();
}

SynthSlider* SynthesisInterface::getSlider(std::string name) {
    if (slider_lookup_.count(name))
        return slider_lookup_[name];
    return nullptr;
}
