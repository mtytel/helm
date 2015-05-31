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

#include "envelope_section.h"

#include "modulation_look_and_feel.h"
#include "synth_slider.h"

#define KNOB_SECTION_WIDTH 40
#define KNOB_WIDTH 32
#define TEXT_HEIGHT 16
#define TEXT_WIDTH 42

EnvelopeSection::EnvelopeSection(String name, std::string value_prepend) : SynthSection(name) {

  addSlider(attack_ = new SynthSlider(value_prepend + "_attack"));
  attack_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(decay_ = new SynthSlider(value_prepend + "_decay"));
  decay_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(release_ = new SynthSlider(value_prepend + "_release"));
  release_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(sustain_ = new SynthSlider(value_prepend + "_sustain"));
  sustain_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addAndMakeVisible(envelope_ = new GraphicalEnvelope());
  envelope_->setAttackSlider(attack_);
  envelope_->setDecaySlider(decay_);
  envelope_->setSustainSlider(sustain_);
  envelope_->setReleaseSlider(release_);

  addModulationButton(modulation_button_ = new ModulationButton(value_prepend + "_envelope"));
  modulation_button_->setLookAndFeel(ModulationLookAndFeel::instance());
}

EnvelopeSection::~EnvelopeSection() {
  envelope_ = nullptr;
  attack_ = nullptr;
  decay_ = nullptr;
  sustain_ = nullptr;
  release_ = nullptr;
}

void EnvelopeSection::paint(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  SynthSection::paint(g);
  component_shadow.drawForRectangle(g, envelope_->getBounds());
}

void EnvelopeSection::resized() {
  envelope_->setBounds(0, 20, getWidth(), getHeight() - 20 - KNOB_SECTION_WIDTH);

  float space = (getWidth() - (5.0f * KNOB_WIDTH)) / 6.0f;
  int y = getHeight() - (KNOB_SECTION_WIDTH + KNOB_WIDTH) / 2;

  modulation_button_->setBounds(10.0f, y, MODULATION_BUTTON_WIDTH, MODULATION_BUTTON_WIDTH);
  attack_->setBounds((KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  decay_->setBounds(2 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  sustain_->setBounds(3 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  release_->setBounds(4 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);

  SynthSection::resized();
}
