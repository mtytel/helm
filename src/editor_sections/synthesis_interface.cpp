/* Copyright 2013-2017 Matt Tytel
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

#include "colors.h"
#include "fonts.h"
#include "midi_keyboard.h"
#include "modulation_look_and_feel.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"
#include <iomanip>

#define DYNAMIC_WIDTH 220.0f

SynthesisInterface::SynthesisInterface(
    mopo::control_map controls, MidiKeyboardState* keyboard_state) : SynthSection("synthesis") {
  addSubSection(amplitude_envelope_section_ = new EnvelopeSection("AMPLITUDE ENVELOPE", "amp"));
  addSubSection(delay_section_ = new DelaySection("DELAY"));
  addSubSection(dynamic_section_ = new DynamicSection("DYNAMICS"));
  addSubSection(extra_envelope_section_ = new EnvelopeSection("MOD ENVELOPE", "mod"));
  addSubSection(extra_mod_section_ = new ExtraModSection("KEYBOARD MOD"));
  addSubSection(feedback_section_ = new FeedbackSection("FEEDBACK"));
  addSubSection(filter_envelope_section_ = new EnvelopeSection("FILTER ENVELOPE", "fil"));
  addSubSection(filter_section_ = new FilterSection("FILTER"));
  addSubSection(formant_section_ = new FormantSection("FORMANT"));
  addSubSection(mono_lfo_1_section_ = new LfoSection("MONO LFO 1", "mono_lfo_1", true, true));
  addSubSection(mono_lfo_2_section_ = new LfoSection("MONO LFO 2", "mono_lfo_2", true, true));
  keyboard_ = new MidiKeyboard(*keyboard_state, MidiKeyboardComponent::horizontalKeyboard);
  addAndMakeVisible(keyboard_);
  keyboard_->setWantsKeyboardFocus(false);
  keyboard_->setMouseClickGrabsKeyboardFocus(false);

  addSubSection(mixer_section_ = new MixerSection("MIXER"));
  addSubSection(oscillator_section_ = new OscillatorSection("OSCILLATORS"));
  addSubSection(poly_lfo_section_ = new LfoSection("POLY LFO", "poly_lfo", false));
  addSubSection(reverb_section_ = new ReverbSection("REVERB"));
  addSubSection(distortion_section_ = new DistortionSection("DISTORTION"));
  addSubSection(step_sequencer_section_ = new StepSequencerSection("STEP SEQUENCER"));
  addSubSection(stutter_section_ = new StutterSection("STUTTER"));
  addSubSection(sub_section_ = new SubSection("SUB"));
  addSubSection(voice_section_ = new VoiceSection("VOICE"));

  keyboard_->setColour(MidiKeyboardComponent::whiteNoteColourId, Colour(0xff444444));
  keyboard_->setColour(MidiKeyboardComponent::blackNoteColourId, Colour(0xff222222));
  keyboard_->setColour(MidiKeyboardComponent::keySeparatorLineColourId, Colour(0x00000000));
  keyboard_->setColour(MidiKeyboardComponent::shadowColourId, Colour(0x00000000));
  keyboard_->setColour(MidiKeyboardComponent::upDownButtonBackgroundColourId, Colour(0xff222222));
  keyboard_->setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colors::audio);
  keyboard_->setColour(MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colour(0x4403a9f4));
  keyboard_->setLowestVisibleKey(36);

  setAllValues(controls);
  setOpaque(false);
}

SynthesisInterface::~SynthesisInterface() {
  amplitude_envelope_section_ = nullptr;
  delay_section_ = nullptr;
  distortion_section_ = nullptr;
  dynamic_section_ = nullptr;
  extra_envelope_section_ = nullptr;
  extra_mod_section_ = nullptr;
  feedback_section_ = nullptr;
  filter_envelope_section_ = nullptr;
  filter_section_ = nullptr;
  formant_section_ = nullptr;
  keyboard_ = nullptr;
  mono_lfo_1_section_ = nullptr;
  mono_lfo_2_section_ = nullptr;
  mixer_section_ = nullptr;
  oscillator_section_ = nullptr;
  poly_lfo_section_ = nullptr;
  reverb_section_ = nullptr;
  step_sequencer_section_ = nullptr;
  stutter_section_ = nullptr;
  sub_section_ = nullptr;
  voice_section_ = nullptr;
}

void SynthesisInterface::paintBackground(Graphics& g) {
  static const DropShadow section_shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));

  section_shadow.drawForRectangle(g, amplitude_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, delay_section_->getBounds());
  section_shadow.drawForRectangle(g, distortion_section_->getBounds());
  section_shadow.drawForRectangle(g, dynamic_section_->getBounds());
  section_shadow.drawForRectangle(g, extra_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, extra_mod_section_->getBounds());
  section_shadow.drawForRectangle(g, feedback_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_envelope_section_->getBounds());
  section_shadow.drawForRectangle(g, filter_section_->getBounds());
  section_shadow.drawForRectangle(g, formant_section_->getBounds());
  section_shadow.drawForRectangle(g, keyboard_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_1_section_->getBounds());
  section_shadow.drawForRectangle(g, mono_lfo_2_section_->getBounds());
  section_shadow.drawForRectangle(g, mixer_section_->getBounds());
  section_shadow.drawForRectangle(g, oscillator_section_->getBounds());
  section_shadow.drawForRectangle(g, poly_lfo_section_->getBounds());
  section_shadow.drawForRectangle(g, reverb_section_->getBounds());
  section_shadow.drawForRectangle(g, step_sequencer_section_->getBounds());
  section_shadow.drawForRectangle(g, stutter_section_->getBounds());
  section_shadow.drawForRectangle(g, sub_section_->getBounds());
  section_shadow.drawForRectangle(g, voice_section_->getBounds());

  paintChildrenBackgrounds(g);
}

void SynthesisInterface::resized() {
  int column_1_x = padding_;
  int column_2_x = column_1_x + padding_ + section_one_width_;
  int column_3_x = column_2_x + padding_ + section_two_width_;
  int section_three_left_width = (section_three_width_ - padding_) / 2;
  int section_three_right_width = section_three_width_ - padding_ - section_three_left_width;
  int column_4_x = column_3_x + padding_ + section_three_left_width;

  int sub_width = 0.53125f * section_one_width_;
  int mixer_width = section_one_width_ - padding_ - sub_width;

  int audio_height = size_ratio_ * 290.0f;
  int oscillators_height = size_ratio_ * 180.0f;
  int sub_mixer_height = audio_height - oscillators_height - padding_;
  int filter_height = size_ratio_ * 196.0f;
  int feedback_height = audio_height - filter_height - padding_;
  int envelopes_height = size_ratio_ * 120.0f;
  int step_lfo_height = size_ratio_ * 148.0f;
  int dynamics_height = size_ratio_ * 64.0f;
  int keyboard_padding = size_ratio_ * 5.0f;
  int stutter_height = size_ratio_ * 141.0f;
  int formant_height = audio_height - stutter_height - padding_;
  int delay_height = size_ratio_ * 91.0f;
  int reverb_height = size_ratio_ * 91.0f;
  int distortion_height = audio_height - delay_height - reverb_height - 2.0f * padding_;

  oscillator_section_->setBounds(column_1_x, padding_, section_one_width_, oscillators_height);
  sub_section_->setBounds(column_1_x, oscillator_section_->getBottom() + padding_,
                          sub_width, sub_mixer_height);
  mixer_section_->setBounds(sub_section_->getRight() + padding_, sub_section_->getY(),
                            mixer_width, sub_mixer_height);
  amplitude_envelope_section_->setBounds(column_1_x, sub_section_->getBottom() + padding_,
                                         section_one_width_, envelopes_height);

  feedback_section_->setBounds(column_2_x, padding_, section_two_width_, feedback_height);
  filter_section_->setBounds(column_2_x, feedback_section_->getBottom() + padding_,
                             section_two_width_, filter_height);
  filter_envelope_section_->setBounds(column_2_x, filter_section_->getBottom() + padding_,
                                      section_two_width_, envelopes_height);

  int lfo_width = 0.421875f * section_one_width_;
  int step_sequencer_width = section_one_width_ + section_two_width_ + padding_ -
                             3 * (lfo_width + padding_);

  int step_lfo_y = amplitude_envelope_section_->getBottom() + padding_;
  mono_lfo_1_section_->setBounds(column_1_x, step_lfo_y,
                                 lfo_width, step_lfo_height);
  mono_lfo_2_section_->setBounds(mono_lfo_1_section_->getRight() + padding_, step_lfo_y,
                                 lfo_width, step_lfo_height);
  poly_lfo_section_->setBounds(mono_lfo_2_section_->getRight() + padding_, step_lfo_y,
                               lfo_width, step_lfo_height);
  step_sequencer_section_->setBounds(poly_lfo_section_->getRight() + padding_, step_lfo_y,
                                     step_sequencer_width, step_lfo_height);

  stutter_section_->setBounds(column_3_x, padding_, section_three_left_width, stutter_height);
  formant_section_->setBounds(column_3_x, stutter_section_->getBottom() + padding_,
                              section_three_left_width, formant_height);
  extra_envelope_section_->setBounds(column_3_x, formant_section_->getBottom() + padding_,
                                     section_three_width_, envelopes_height);
  extra_mod_section_->setBounds(column_3_x, extra_envelope_section_->getBottom() + padding_,
                                extra_envelope_section_->getWidth(), step_lfo_height);

  distortion_section_->setBounds(column_4_x, padding_,
                                 section_three_right_width, distortion_height);
  delay_section_->setBounds(column_4_x, distortion_section_->getBottom() + padding_,
                            section_three_right_width, delay_height);
  reverb_section_->setBounds(column_4_x, delay_section_->getBottom() + padding_,
                             section_three_right_width, reverb_height);

  int dynamic_width = size_ratio_ * DYNAMIC_WIDTH;
  int dynamics_y = getHeight() - padding_ - dynamics_height;
  
  voice_section_->setBounds(column_1_x, dynamics_y,
                            dynamic_width, dynamics_height);
  dynamic_section_->setBounds(extra_envelope_section_->getRight() - dynamic_width, dynamics_y,
                              dynamic_width, dynamics_height);
  keyboard_->setBounds(voice_section_->getRight() + padding_,
                       voice_section_->getY() + keyboard_padding,
                       dynamic_section_->getX() - voice_section_->getRight() - 2 * padding_,
                       dynamics_height - 2 * keyboard_padding);
  keyboard_->setKeyWidth(size_ratio_ * 16.0f);

  SynthSection::resized();
}
