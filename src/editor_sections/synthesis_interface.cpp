/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "synthesis_interface.h"

#include <iomanip>
#include "modulation_look_and_feel.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

#define COLUMN_WIDTH_1 320.0f
#define COLUMN_WIDTH_2 320.0f
#define COLUMN_WIDTH_3 150.0f
#define COLUMN_WIDTH_4 150.0f
#define CELL_PADDING 8.0f

SynthesisInterface::SynthesisInterface(mopo::control_map controls) : SynthSection("synthesis") {
  addSubSection(amplitude_envelope_section_ = new EnvelopeSection("AMPLITUDE ENVELOPE", "amp"));
  addSubSection(articulation_section_ = new ArticulationSection("ARTICULATION"));
  addSubSection(delay_section_ = new DelaySection("DELAY"));
  addSubSection(extra_mod_section_ = new ExtraModSection("KEYBOARD MOD"));
  addSubSection(feedback_section_ = new FeedbackSection("FEEDBACK"));
  addSubSection(filter_envelope_section_ = new EnvelopeSection("FILTER ENVELOPE", "fil"));
  addSubSection(filter_section_ = new FilterSection("FILTER"));
  addSubSection(formant_section_ = new FormantSection("FORMANT"));
  addSubSection(mono_lfo_1_section_ = new LfoSection("MONO LFO 1", "mono_lfo_1"));
  addSubSection(mono_lfo_2_section_ = new LfoSection("MONO LFO 2", "mono_lfo_2"));
  addSubSection(noise_section_ = new NoiseSection("NOISE"));
  addSubSection(oscillator_section_ = new OscillatorSection("OSCILLATORS"));
  addSubSection(poly_lfo_section_ = new LfoSection("POLY LFO", "poly_lfo"));
  addSubSection(reverb_section_ = new ReverbSection("REVERB"));
  addSubSection(step_sequencer_section_ = new StepSequencerSection("STEP SEQUENCER"));
  addSubSection(stutter_section_ = new StutterSection("STUTTER"));
  addSubSection(sub_section_ = new SubSection("SUB"));
  addSubSection(volume_section_ = new VolumeSection("VOLUME"));

  setAllValues(controls);
  setOpaque(true);
}

SynthesisInterface::~SynthesisInterface() {
  amplitude_envelope_section_ = nullptr;
  articulation_section_ = nullptr;
  delay_section_ = nullptr;
  extra_mod_section_ = nullptr;
  feedback_section_ = nullptr;
  filter_envelope_section_ = nullptr;
  filter_section_ = nullptr;
  formant_section_ = nullptr;
  mono_lfo_1_section_ = nullptr;
  mono_lfo_2_section_ = nullptr;
  noise_section_ = nullptr;
  oscillator_section_ = nullptr;
  poly_lfo_section_ = nullptr;
  reverb_section_ = nullptr;
  step_sequencer_section_ = nullptr;
  stutter_section_ = nullptr;
  sub_section_ = nullptr;
  volume_section_ = nullptr;
}

void SynthesisInterface::paintBackground(Graphics& g) {
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
  section_shadow.drawForRectangle(g, extra_mod_section_->getBounds());
  section_shadow.drawForRectangle(g, feedback_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_section_->getBounds());
  section_shadow.drawForRectangle(g, formant_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_1_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_2_section_->getBounds());
  section_shadow.drawForRectangle(g, noise_section_->getBounds());
  section_shadow.drawForRectangle(g, oscillator_section_->getBounds());
  section_shadow.drawForRectangle(g, poly_lfo_section_->getBounds());
  section_shadow.drawForRectangle(g, reverb_section_->getBounds());
  section_shadow.drawForRectangle(g, step_sequencer_section_->getBounds());
  section_shadow.drawForRectangle(g, stutter_section_->getBounds());
  section_shadow.drawForRectangle(g, sub_section_->getBounds());
  section_shadow.drawForRectangle(g, volume_section_->getBounds());
}

void SynthesisInterface::resized() {
  float column_1_x = CELL_PADDING;
  float column_2_x = column_1_x + CELL_PADDING + COLUMN_WIDTH_1;
  float column_3_x = column_2_x + CELL_PADDING + COLUMN_WIDTH_2;
  float column_4_x = column_3_x + CELL_PADDING + COLUMN_WIDTH_3;

  oscillator_section_->setBounds(column_1_x, 4.0f, COLUMN_WIDTH_1, 190.0f);
  sub_section_->setBounds(column_1_x, oscillator_section_->getBottom() + CELL_PADDING,
                          200, 92.0f);
  noise_section_->setBounds(sub_section_->getRight() + CELL_PADDING, sub_section_->getY(),
                            oscillator_section_->getWidth() -
                            CELL_PADDING - sub_section_->getWidth(),
                            sub_section_->getHeight());
  amplitude_envelope_section_->setBounds(column_1_x, 302.0f, COLUMN_WIDTH_1, 120.0f);

  feedback_section_->setBounds(column_2_x, 4.0f, COLUMN_WIDTH_2, 92.0f);
  filter_section_->setBounds(column_2_x, feedback_section_->getBottom() + CELL_PADDING,
                             COLUMN_WIDTH_2, 190.0f);
  filter_envelope_section_->setBounds(column_2_x, filter_section_->getBottom() + CELL_PADDING,
                                      COLUMN_WIDTH_2, 120.0f);

  float lfo_width = (COLUMN_WIDTH_1 + COLUMN_WIDTH_2 - 3 * CELL_PADDING) / 5.0f;
  float step_sequencer_width = 2 * lfo_width + CELL_PADDING;

  float step_lfo_y = amplitude_envelope_section_->getBottom() + CELL_PADDING;
  float step_lfo_height = 148.0f;
  mono_lfo_1_section_->setBounds(column_1_x, step_lfo_y,
                                 lfo_width, step_lfo_height);
  mono_lfo_2_section_->setBounds(mono_lfo_1_section_->getRight() + CELL_PADDING, step_lfo_y,
                                 lfo_width, step_lfo_height);
  poly_lfo_section_->setBounds(mono_lfo_2_section_->getRight() + CELL_PADDING, step_lfo_y,
                               lfo_width, step_lfo_height);
  step_sequencer_section_->setBounds(poly_lfo_section_->getRight() + CELL_PADDING, step_lfo_y,
                                     step_sequencer_width, step_lfo_height);

  stutter_section_->setBounds(column_3_x, 4.0f, COLUMN_WIDTH_3, 141.0f);
  formant_section_->setBounds(column_3_x, stutter_section_->getBottom() + CELL_PADDING,
                              COLUMN_WIDTH_3, 141.0f);
  extra_mod_section_->setBounds(column_3_x, formant_section_->getBottom() + CELL_PADDING,
                              COLUMN_WIDTH_3, 276.0f);

  delay_section_->setBounds(column_4_x, 4.0f, COLUMN_WIDTH_4, 105.0f);
  reverb_section_->setBounds(column_4_x, delay_section_->getBottom() + CELL_PADDING,
                             COLUMN_WIDTH_4, 105.0f);
  volume_section_->setBounds(column_4_x, reverb_section_->getBottom() + CELL_PADDING,
                             COLUMN_WIDTH_4, 64.0f);
  articulation_section_->setBounds(column_4_x, extra_mod_section_->getY(),
                                   COLUMN_WIDTH_4, extra_mod_section_->getHeight());

  SynthSection::resized();
}
