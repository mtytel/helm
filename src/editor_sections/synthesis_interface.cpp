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
#include "modulation_look_and_feel.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

SynthesisInterface::SynthesisInterface(mopo::control_map controls) : SynthSection("synthesis") {
  addSubSection(amplitude_envelope_section_ = new EnvelopeSection("AMPLITUDE ENVELOPE", "amp"));
  addSubSection(delay_section_ = new DelaySection("DELAY"));
  addSubSection(feedback_section_ = new FeedbackSection("FEEDBACK"));
  addSubSection(filter_envelope_section_ = new EnvelopeSection("FILTER ENVELOPE", "fil"));
  addSubSection(filter_section_ = new FilterSection("FILTER"));
  addSubSection(formant_section_ = new FormantSection("FORMANT"));
  addSubSection(mono_lfo_1_section_ = new LfoSection("MONO LFO 1", "mono_lfo_1"));
  addSubSection(mono_lfo_2_section_ = new LfoSection("MONO LFO 2", "mono_lfo_2"));
  addSubSection(poly_lfo_section_ = new LfoSection("POLY LFO", "poly_lfo"));
  addSubSection(reverb_section_ = new ReverbSection("REVERB"));
  addSubSection(step_sequencer_section_ = new StepSequencerSection("STEP SEQUENCER"));

  addAndMakeVisible(osc_1_wave_display_ = new WaveViewer(256));
  addAndMakeVisible(osc_2_wave_display_ = new WaveViewer(256));
  addSlider(polyphony_ = new SynthSlider("polyphony"));
  polyphony_->setRange(1, 32, 1);
  polyphony_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  polyphony_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  polyphony_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  polyphony_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  polyphony_->addListener(this);

  addSlider(portamento_ = new SynthSlider("portamento"));
  portamento_->setRange(-9, -1, 0);
  portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  portamento_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  portamento_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  portamento_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  portamento_->addListener(this);

  addSlider(pitch_bend_range_ = new SynthSlider("pitch_bend_range"));
  pitch_bend_range_->setRange(0, 48, 1);
  pitch_bend_range_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  pitch_bend_range_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  pitch_bend_range_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  pitch_bend_range_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  pitch_bend_range_->addListener(this);

  addSlider(cross_modulation_ = new SynthSlider("cross_modulation"));
  cross_modulation_->setRange(0, 0.4, 0);
  cross_modulation_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  cross_modulation_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  cross_modulation_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  cross_modulation_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  cross_modulation_->addListener(this);

  addSlider(portamento_type_ = new SynthSlider("portamento_type"));
  portamento_type_->setRange(0, 2, 1);
  portamento_type_->setSliderStyle(Slider::LinearBar);
  portamento_type_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
  portamento_type_->setColour(Slider::backgroundColourId, Colour(0xff333333));
  portamento_type_->setColour(Slider::trackColourId, Colour(0xff9765bc));
  portamento_type_->setColour(Slider::textBoxOutlineColourId, Colour(0x00bbbbbb));
  portamento_type_->addListener(this);

  addSlider(osc_mix_ = new SynthSlider("osc_mix"));
  osc_mix_->setRange(0, 1, 0);
  osc_mix_->setSliderStyle(Slider::LinearBar);
  osc_mix_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_mix_->setColour(Slider::backgroundColourId, Colour(0xff303030));
  osc_mix_->setColour(Slider::trackColourId, Colour(0xff9765bc));
  osc_mix_->setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
  osc_mix_->addListener(this);

  addSlider(osc_2_transpose_ = new SynthSlider("osc_2_transpose"));
  osc_2_transpose_->setRange(-48, 48, 1);
  osc_2_transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_2_transpose_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_2_transpose_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_2_transpose_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_2_transpose_->addListener(this);

  addSlider(osc_2_tune_ = new SynthSlider("osc_2_tune"));
  osc_2_tune_->setRange(-1, 1, 0);
  osc_2_tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_2_tune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_2_tune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_2_tune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_2_tune_->addListener(this);

  addSlider(volume_ = new SynthSlider("volume"));
  volume_->setRange(0, 1, 0);
  volume_->setSliderStyle(Slider::LinearBar);
  volume_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
  volume_->setColour(Slider::backgroundColourId, Colour(0xff303030));
  volume_->setColour(Slider::trackColourId, Colour(0xff9765bc));
  volume_->setColour(Slider::textBoxOutlineColourId, Colour(0x00000000));
  volume_->addListener(this);

  addSlider(velocity_track_ = new SynthSlider("velocity_track"));
  velocity_track_->setRange(-1, 1, 0);
  velocity_track_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  velocity_track_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  velocity_track_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  velocity_track_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  velocity_track_->addListener(this);

  addSlider(osc_1_waveform_ = new WaveSelector("osc_1_waveform"));
  osc_1_waveform_->setRange(0, 11, 1);
  osc_1_waveform_->setSliderStyle(Slider::LinearBar);
  osc_1_waveform_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  osc_1_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
  osc_1_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
  osc_1_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
  osc_1_waveform_->addListener(this);

  addSlider(osc_2_waveform_ = new WaveSelector("osc_2_waveform"));
  osc_2_waveform_->setRange(0, 11, 1);
  osc_2_waveform_->setSliderStyle(Slider::LinearBar);
  osc_2_waveform_->setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
  osc_2_waveform_->setColour(Slider::backgroundColourId, Colour(0xff303030));
  osc_2_waveform_->setColour(Slider::trackColourId, Colour(0xff9765bc));
  osc_2_waveform_->setColour(Slider::textBoxOutlineColourId, Colour(0xff303030));
  osc_2_waveform_->addListener(this);

  addSlider(osc_1_tune_ = new SynthSlider("osc_1_tune"));
  osc_1_tune_->setRange(-1, 1, 0);
  osc_1_tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_1_tune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_1_tune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_1_tune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_1_tune_->addListener(this);

  addSlider(osc_1_transpose_ = new SynthSlider("osc_1_transpose"));
  osc_1_transpose_->setRange(-48, 48, 1);
  osc_1_transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_1_transpose_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_1_transpose_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_1_transpose_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_1_transpose_->addListener(this);

  addSlider(stutter_frequency_ = new SynthSlider("stutter_frequency"));
  stutter_frequency_->setRange(4, 100, 0);
  stutter_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  stutter_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  stutter_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  stutter_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  stutter_frequency_->addListener(this);

  addButton(stutter_on_ = new ToggleButton("stutter_on"));
  stutter_on_->setButtonText(String::empty);
  stutter_on_->addListener(this);
  stutter_on_->setToggleState(true, dontSendNotification);
  stutter_on_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

  addSlider(stutter_resample_frequency_ = new SynthSlider("stutter_resample_frequency"));
  stutter_resample_frequency_->setRange(0.5, 20, 0);
  stutter_resample_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  stutter_resample_frequency_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  stutter_resample_frequency_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  stutter_resample_frequency_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  stutter_resample_frequency_->addListener(this);

  addSlider(osc_1_unison_detune_ = new SynthSlider("osc_1_unison_detune"));
  osc_1_unison_detune_->setRange(0, 200, 0);
  osc_1_unison_detune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_1_unison_detune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_1_unison_detune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_1_unison_detune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_1_unison_detune_->addListener(this);

  addSlider(osc_2_unison_detune_ = new SynthSlider("osc_2_unison_detune"));
  osc_2_unison_detune_->setRange(0, 200, 0);
  osc_2_unison_detune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_2_unison_detune_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_2_unison_detune_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_2_unison_detune_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_2_unison_detune_->addListener(this);

  addSlider(osc_1_unison_voices_ = new SynthSlider("osc_1_unison_voices"));
  osc_1_unison_voices_->setRange(1, 15, 2);
  osc_1_unison_voices_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_1_unison_voices_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_1_unison_voices_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_1_unison_voices_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_1_unison_voices_->addListener(this);

  addSlider(osc_2_unison_voices_ = new SynthSlider("osc_2_unison_voices"));
  osc_2_unison_voices_->setRange(1, 15, 2);
  osc_2_unison_voices_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  osc_2_unison_voices_->setTextBoxStyle(Slider::NoTextBox, true, 80, 20);
  osc_2_unison_voices_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  osc_2_unison_voices_->setColour(Slider::textBoxTextColourId, Colour(0xff999999));
  osc_2_unison_voices_->addListener(this);

  addButton(filter_envelope_mod_ = new ModulationButton("filter_env"));
  filter_envelope_mod_->setButtonText(String::empty);
  filter_envelope_mod_->addListener(this);

  addButton(amplitude_envelope_mod_ = new ModulationButton("amplitude_env"));
  amplitude_envelope_mod_->setButtonText(String::empty);
  amplitude_envelope_mod_->addListener(this);

  addButton(step_sequencer_mod_ = new ModulationButton("step_sequencer"));
  step_sequencer_mod_->setButtonText(String::empty);
  step_sequencer_mod_->addListener(this);

  addButton(mono_lfo_1_mod_ = new ModulationButton("mono_lfo_1"));
  mono_lfo_1_mod_->setButtonText(String::empty);
  mono_lfo_1_mod_->addListener(this);

  addButton(mono_lfo_2_mod_ = new ModulationButton("mono_lfo_2"));
  mono_lfo_2_mod_->setButtonText(String::empty);
  mono_lfo_2_mod_->addListener(this);

  addButton(poly_lfo_mod_ = new ModulationButton("poly_lfo"));
  poly_lfo_mod_->setButtonText(String::empty);
  poly_lfo_mod_->addListener(this);

  addButton(aftertouch_mod_ = new ModulationButton("aftertouch"));
  aftertouch_mod_->setButtonText(String::empty);
  aftertouch_mod_->addListener(this);

  addButton(note_mod_ = new ModulationButton("note"));
  note_mod_->setButtonText(String::empty);
  note_mod_->addListener(this);

  addButton(velocity_mod_ = new ModulationButton("velocity"));
  velocity_mod_->setButtonText(String::empty);
  velocity_mod_->addListener(this);

  addButton(mod_wheel_mod_ = new ModulationButton("mod_wheel"));
  mod_wheel_mod_->setButtonText(String::empty);
  mod_wheel_mod_->addListener(this);

  addButton(pitch_wheel_mod_ = new ModulationButton("pitch_wheel"));
  pitch_wheel_mod_->setButtonText(String::empty);
  pitch_wheel_mod_->addListener(this);

  addButton(legato_ = new ToggleButton("legato"));
  legato_->setButtonText(String::empty);
  legato_->addListener(this);
  legato_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

  addButton(unison_1_harmonize_ = new ToggleButton("unison_1_harmonize"));
  unison_1_harmonize_->setButtonText(String::empty);
  unison_1_harmonize_->addListener(this);
  unison_1_harmonize_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

  addButton(unison_2_harmonize_ = new ToggleButton("unison_2_harmonize"));
  unison_2_harmonize_->setButtonText(String::empty);
  unison_2_harmonize_->addListener(this);
  unison_2_harmonize_->setColour(ToggleButton::textColourId, Colour(0xffbbbbbb));

  setSliderUnits();
  markBipolarSliders();
  setStyles();

  osc_1_wave_display_->setWaveSlider(osc_1_waveform_);
  osc_2_wave_display_->setWaveSlider(osc_2_waveform_);

  setDefaultDoubleClickValues();
  setAllValues(controls);
  setOpaque(true);
}

SynthesisInterface::~SynthesisInterface() {
  amplitude_envelope_section_ = nullptr;
  delay_section_ = nullptr;
  feedback_section_ = nullptr;
  filter_envelope_section_ = nullptr;
  filter_section_ = nullptr;
  formant_section_ = nullptr;
  mono_lfo_1_section_ = nullptr;
  mono_lfo_2_section_ = nullptr;
  poly_lfo_section_ = nullptr;
  reverb_section_ = nullptr;
  step_sequencer_section_ = nullptr;

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
  velocity_track_ = nullptr;
  osc_1_waveform_ = nullptr;
  osc_2_waveform_ = nullptr;
  osc_1_tune_ = nullptr;
  osc_1_transpose_ = nullptr;
  stutter_frequency_ = nullptr;
  stutter_on_ = nullptr;
  stutter_resample_frequency_ = nullptr;
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
}

void SynthesisInterface::paint(Graphics& g) {
  static const DropShadow section_shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));

  g.setColour(Colour(0xff212121));
  g.fillRect(getLocalBounds());

  section_shadow.drawForRectangle(g, amplitude_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, delay_section_->getBounds());
  section_shadow.drawForRectangle(g, feedback_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_section_->getBounds());
  section_shadow.drawForRectangle(g, formant_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_1_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_2_section_->getBounds());
  section_shadow.drawForRectangle(g, poly_lfo_section_->getBounds());
  section_shadow.drawForRectangle(g, reverb_section_->getBounds());
  section_shadow.drawForRectangle(g, step_sequencer_section_->getBounds());

  section_shadow.drawForRectangle(g, Rectangle<int>(8, 630 - (44 / 2), 722, 44));
  section_shadow.drawForRectangle(g, Rectangle<int>(270 - (116 / 2), 482, 116, 118));
  section_shadow.drawForRectangle(g, Rectangle<int>(604, 416, 126, 58));
  section_shadow.drawForRectangle(g, Rectangle<int>(604, 4, 126, 220));
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
  g.fillRoundedRectangle(604.0f, 4.0f, 126.0f, 220.0f, 3.000f);

  g.setGradientFill(ColourGradient(Colour(0x00000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 22.0f,
                                   Colour(0x77000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 26.0f,
                                   false));
  g.fillRect(604, 4, 126, 20);

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
  g.drawText(TRANS("VEL TRACK"),
             700 - (60 / 2), 154, 60, 10,
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

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("MIX"),
             168 - (50 / 2), 140, 50, 10,
             Justification::centred, true);

  g.setGradientFill(ColourGradient(Colour(0x00000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 500.0f,
                                   Colour(0x77000000),
                                   static_cast<float>(proportionOfWidth(0.0000f)), 504.0f,
                                   false));
  g.fillRect(212, 482, 116, 20);

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
  g.setFont(roboto_reg.withPointHeight(8.0f));
  g.drawText(TRANS("OFF"),
             614 - (28 / 2), 172, 28, 12,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(8.0f));
  g.drawText(TRANS("AUTO"),
             634 - (28 / 2), 172, 28, 12,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(8.0f));
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

  component_shadow.drawForRectangle(g, osc_1_wave_display_->getBounds());
  component_shadow.drawForRectangle(g, osc_2_wave_display_->getBounds());

  for (auto slider : slider_lookup_)
    slider.second->drawShadow(g);
}

void SynthesisInterface::resized() {
  amplitude_envelope_section_->setBounds(336.0f, 160.0f, 260.0f, 148.0f);
  delay_section_->setBounds(604.0f, 232.0f, 126.0f, 84.0f);
  feedback_section_->setBounds(8.0f, 216.0f, 320.0f, 92.0f);
  filter_envelope_section_->setBounds(336.0f, 4.0f, 260.0f, 148.0f);
  filter_section_->setBounds(8.0f, 316.0f, 320.0f, 158.0f);
  formant_section_->setBounds(8.0f, 482.0f, 196.0f, 118.0f);
  mono_lfo_1_section_->setBounds(336.0f, 482.0f, 126.0f, 118.0f);
  mono_lfo_2_section_->setBounds(470.0f, 482.0f, 126.0f, 118.0f);
  poly_lfo_section_->setBounds(604.0f, 482.0f, 126.0f, 118.0f);
  reverb_section_->setBounds(604.0f, 324.0f, 126.0f, 84.0f);
  step_sequencer_section_->setBounds(336.0f, 316.0f, 260.0f, 158.0f);

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
  velocity_track_->setBounds(700 - (40 / 2), 110, 40, 40);
  osc_1_waveform_->setBounds(8, 24, 128, 16);
  osc_2_waveform_->setBounds(200, 24, 128, 16);
  osc_1_tune_->setBounds(36 - (32 / 2), 156, 32, 32);
  osc_1_transpose_->setBounds(76 - (40 / 2), 148, 40, 40);
  stutter_frequency_->setBounds(241 - (40 / 2), 524, 40, 40);
  stutter_on_->setBounds(214, 482, 20, 20);
  stutter_resample_frequency_->setBounds(297 - (40 / 2), 524, 40, 40);
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

  if (buttonThatWasClicked == stutter_on_)
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

  portamento_->setUnits("secs/semitone");
  portamento_->setScalingType(mopo::ValueDetails::kExponential);
  pitch_bend_range_->setUnits("semitones");

  polyphony_->setUnits("voices");

  velocity_track_->setUnits("%");
  velocity_track_->setPostMultiply(100.0);
  portamento_type_->setStringLookup(mopo::strings::off_auto_on);

  osc_1_waveform_->setStringLookup(mopo::strings::waveforms);
  osc_2_waveform_->setStringLookup(mopo::strings::waveforms);
}

void SynthesisInterface::markBipolarSliders() {
  osc_1_transpose_->setBipolar();
  osc_1_tune_->setBipolar();
  osc_2_transpose_->setBipolar();
  osc_2_tune_->setBipolar();
  osc_mix_->setBipolar();
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

  portamento_->setDoubleClickReturnValue(true, -7.0f);
  pitch_bend_range_->setDoubleClickReturnValue(true, 2.0f);

  velocity_track_->setDoubleClickReturnValue(true, 0.0f);
}

void SynthesisInterface::setStyles() {
  static const int UNISON_DRAG_SENSITIVITY = 100;

  osc_1_unison_voices_->setLookAndFeel(TextLookAndFeel::instance());
  osc_1_unison_voices_->setMouseDragSensitivity(UNISON_DRAG_SENSITIVITY);
  osc_2_unison_voices_->setLookAndFeel(TextLookAndFeel::instance());
  osc_1_unison_voices_->setMouseDragSensitivity(UNISON_DRAG_SENSITIVITY);

  legato_->setLookAndFeel(TextLookAndFeel::instance());
  unison_1_harmonize_->setLookAndFeel(TextLookAndFeel::instance());
  unison_2_harmonize_->setLookAndFeel(TextLookAndFeel::instance());

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
}

void SynthesisInterface::setValue(std::string name, mopo::mopo_float value,
                                  NotificationType notification) {
  if (slider_lookup_.count(name))
    slider_lookup_[name]->setValue(value, notification);
}

void SynthesisInterface::modulationChanged(std::string source) {
  button_lookup_[source]->repaint();
}
