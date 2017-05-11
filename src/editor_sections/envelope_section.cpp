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

#include "envelope_section.h"

#include "colors.h"
#include "fonts.h"
#include "modulation_look_and_feel.h"
#include "synth_slider.h"

#define SLIDER_SECTION_WIDTH 70
#define MOD_SECTION_WIDTH 36
#define MOD_BUTTON_WIDTH 32
#define TEXT_WIDTH 10
#define SLIDER_WIDTH 20

EnvelopeSection::EnvelopeSection(String name, std::string value_prepend) : SynthSection(name) {

  addSlider(attack_ = new SynthSlider(value_prepend + "_attack"));
  attack_->setSliderStyle(Slider::LinearBar);

  addSlider(decay_ = new SynthSlider(value_prepend + "_decay"));
  decay_->setSliderStyle(Slider::LinearBar);

  addSlider(release_ = new SynthSlider(value_prepend + "_release"));
  release_->setSliderStyle(Slider::LinearBar);

  addSlider(sustain_ = new SynthSlider(value_prepend + "_sustain"));
  sustain_->setSliderStyle(Slider::LinearBar);

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

void EnvelopeSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));

  SynthSection::paintBackground(g);
  component_shadow.drawForRectangle(g, envelope_->getBounds());

  g.setColour(Colors::background);
  g.fillRect(getWidth() - TEXT_WIDTH - SLIDER_SECTION_WIDTH, 20, TEXT_WIDTH, getHeight() - 20);

  g.setColour(Colors::controlLabelText);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));

  g.drawText(TRANS("A"), attack_->getX() - TEXT_WIDTH, attack_->getY(),
             TEXT_WIDTH, attack_->getHeight(),
             Justification::centred, true);
  g.drawText(TRANS("D"), decay_->getX() - TEXT_WIDTH, decay_->getY(),
             TEXT_WIDTH, decay_->getHeight(),
             Justification::centred, true);
  g.drawText(TRANS("S"), sustain_->getX() - TEXT_WIDTH, sustain_->getY(),
             TEXT_WIDTH, sustain_->getHeight(),
             Justification::centred, true);
  g.drawText(TRANS("R"), release_->getX() - TEXT_WIDTH, release_->getY(),
             TEXT_WIDTH, release_->getHeight(),
             Justification::centred, true);
}

void EnvelopeSection::resized() {
  int envelope_width = getWidth() - SLIDER_SECTION_WIDTH - TEXT_WIDTH - MOD_SECTION_WIDTH;
  envelope_->setBounds(MOD_SECTION_WIDTH, 20, envelope_width, getHeight() - 20);

  float space = (getHeight() - (4.0f * SLIDER_WIDTH) - 20) / 3.0f;
  int x = getWidth() - SLIDER_SECTION_WIDTH;

  float mod_button_x = (MOD_SECTION_WIDTH - MOD_BUTTON_WIDTH) / 2.0f;
  float mod_button_y = (getHeight() - MOD_BUTTON_WIDTH + 20) / 2.0f;
  modulation_button_->setBounds(mod_button_x, mod_button_y, MOD_BUTTON_WIDTH, MOD_BUTTON_WIDTH);
  attack_->setBounds(x, 20, SLIDER_SECTION_WIDTH, SLIDER_WIDTH);
  decay_->setBounds(x, 20 + space + SLIDER_WIDTH, SLIDER_SECTION_WIDTH, SLIDER_WIDTH);
  sustain_->setBounds(x, 20 + 2 * (space + SLIDER_WIDTH), SLIDER_SECTION_WIDTH, SLIDER_WIDTH);
  release_->setBounds(x, 20 + 3 * (space + SLIDER_WIDTH), SLIDER_SECTION_WIDTH, SLIDER_WIDTH);

  SynthSection::resized();
}

void EnvelopeSection::reset() {
  envelope_->resetEnvelopeLine();
  envelope_->repaint();
  SynthSection::reset();
}
