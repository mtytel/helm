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
#include "modulation_look_and_feel.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

#define COLUMN_WIDTH_1 320.0f
#define COLUMN_WIDTH_2 260.0f
#define COLUMN_WIDTH_3 126.0f
#define CELL_PADDING 8.0f

SynthesisInterface::SynthesisInterface(mopo::control_map controls) : SynthSection("synthesis") {
  addSubSection(amplitude_envelope_section_ = new EnvelopeSection("AMPLITUDE ENVELOPE", "amp"));
  addSubSection(articulation_section_ = new ArticulationSection("ARTICULATION"));
  addSubSection(delay_section_ = new DelaySection("DELAY"));
  addSubSection(feedback_section_ = new FeedbackSection("FEEDBACK"));
  addSubSection(filter_envelope_section_ = new EnvelopeSection("FILTER ENVELOPE", "fil"));
  addSubSection(filter_section_ = new FilterSection("FILTER"));
  addSubSection(formant_section_ = new FormantSection("FORMANT"));
  addSubSection(mono_lfo_1_section_ = new LfoSection("MONO LFO 1", "mono_lfo_1"));
  addSubSection(mono_lfo_2_section_ = new LfoSection("MONO LFO 2", "mono_lfo_2"));
  addSubSection(oscillator_section_ = new OscillatorSection("OSCILLATORS"));
  addSubSection(poly_lfo_section_ = new LfoSection("POLY LFO", "poly_lfo"));
  addSubSection(reverb_section_ = new ReverbSection("REVERB"));
  addSubSection(step_sequencer_section_ = new StepSequencerSection("STEP SEQUENCER"));
  addSubSection(stutter_section_ = new StutterSection("STUTTER"));
  addSubSection(volume_section_ = new VolumeSection("VOLUME"));

  addModulationButton(aftertouch_mod_ = new ModulationButton("aftertouch"));
  aftertouch_mod_->setButtonText(String::empty);
  aftertouch_mod_->addListener(this);
  aftertouch_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(note_mod_ = new ModulationButton("note"));
  note_mod_->setButtonText(String::empty);
  note_mod_->addListener(this);
  note_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(velocity_mod_ = new ModulationButton("velocity"));
  velocity_mod_->setButtonText(String::empty);
  velocity_mod_->addListener(this);
  velocity_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(mod_wheel_mod_ = new ModulationButton("mod_wheel"));
  mod_wheel_mod_->setButtonText(String::empty);
  mod_wheel_mod_->addListener(this);
  mod_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(pitch_wheel_mod_ = new ModulationButton("pitch_wheel"));
  pitch_wheel_mod_->setButtonText(String::empty);
  pitch_wheel_mod_->addListener(this);
  pitch_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  setAllValues(controls);
  setOpaque(true);
}

SynthesisInterface::~SynthesisInterface() {
  amplitude_envelope_section_ = nullptr;
  articulation_section_ = nullptr;
  delay_section_ = nullptr;
  feedback_section_ = nullptr;
  filter_envelope_section_ = nullptr;
  filter_section_ = nullptr;
  formant_section_ = nullptr;
  mono_lfo_1_section_ = nullptr;
  mono_lfo_2_section_ = nullptr;
  oscillator_section_ = nullptr;
  poly_lfo_section_ = nullptr;
  reverb_section_ = nullptr;
  step_sequencer_section_ = nullptr;
  stutter_section_ = nullptr;
  volume_section_ = nullptr;

  aftertouch_mod_ = nullptr;
  note_mod_ = nullptr;
  velocity_mod_ = nullptr;
  mod_wheel_mod_ = nullptr;
  pitch_wheel_mod_ = nullptr;
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
  section_shadow.drawForRectangle(g, articulation_section_->getBounds());
  section_shadow.drawForRectangle(g, delay_section_->getBounds());
  section_shadow.drawForRectangle(g, feedback_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_section_->getBounds());
  section_shadow.drawForRectangle(g, formant_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_1_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_2_section_->getBounds());
  section_shadow.drawForRectangle(g, oscillator_section_->getBounds());
  section_shadow.drawForRectangle(g, poly_lfo_section_->getBounds());
  section_shadow.drawForRectangle(g, reverb_section_->getBounds());
  section_shadow.drawForRectangle(g, step_sequencer_section_->getBounds());
  section_shadow.drawForRectangle(g, stutter_section_->getBounds());
  section_shadow.drawForRectangle(g, volume_section_->getBounds());

  section_shadow.drawForRectangle(g, Rectangle<int>(8, 630 - (44 / 2), 722, 44));

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(CELL_PADDING, 608.0f, getWidth() - 2.0f * CELL_PADDING, 44.0f, 3.0f);

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

  for (auto slider : slider_lookup_)
    slider.second->drawShadow(g);
}

void SynthesisInterface::resized() {
  float column_1_x = CELL_PADDING;
  float column_2_x = column_1_x + CELL_PADDING + COLUMN_WIDTH_1;
  float column_3_x = column_2_x + CELL_PADDING + COLUMN_WIDTH_2;
  
  amplitude_envelope_section_->setBounds(column_2_x, 160.0f, 260.0f, 148.0f);
  articulation_section_->setBounds(column_3_x, 4.0f, 126.0f, 220.0f);
  delay_section_->setBounds(column_3_x, 232.0f, 126.0f, 84.0f);
  feedback_section_->setBounds(column_1_x, 216.0f, COLUMN_WIDTH_1, 92.0f);
  filter_envelope_section_->setBounds(column_2_x, 4.0f, 260.0f, 148.0f);
  filter_section_->setBounds(column_1_x, 316.0f, COLUMN_WIDTH_1, 158.0f);
  formant_section_->setBounds(column_1_x, 482.0f, 196.0f, 118.0f);
  mono_lfo_1_section_->setBounds(column_2_x, 482.0f, 126.0f, 118.0f);
  mono_lfo_2_section_->setBounds(470.0f, 482.0f, 126.0f, 118.0f);
  oscillator_section_->setBounds(column_1_x, 4.0f, COLUMN_WIDTH_1, 204.0f);
  poly_lfo_section_->setBounds(column_3_x, 482.0f, 126.0f, 118.0f);
  reverb_section_->setBounds(column_3_x, 324.0f, 126.0f, 84.0f);
  step_sequencer_section_->setBounds(column_2_x, 316.0f, 260.0f, 158.0f);
  stutter_section_->setBounds(212.0f, 482.0f, 116.0f, 118.0f);
  volume_section_->setBounds(column_3_x, 416.0f, 126.0f, 58.0f);

  aftertouch_mod_->setBounds(614, 614, 32, 32);
  note_mod_->setBounds(346, 614, 32, 32);
  velocity_mod_->setBounds(480, 614, 32, 32);
  mod_wheel_mod_->setBounds(173, 614, 32, 32);
  pitch_wheel_mod_->setBounds(20, 614, 32, 32);
}
