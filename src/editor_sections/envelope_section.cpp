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
#define TEXT_WIDTH 10
#define SLIDER_WIDTH 20

EnvelopeSection::EnvelopeSection(String name, std::string value_prepend) : SynthSection(name) {

  addSlider(attack_ = new SynthSlider(value_prepend + "_attack"));
  attack_->setSliderStyle(Slider::LinearBar);
  attack_->setPopupPlacement(BubbleComponent::below);

  addSlider(decay_ = new SynthSlider(value_prepend + "_decay"));
  decay_->setSliderStyle(Slider::LinearBar);
  decay_->setPopupPlacement(BubbleComponent::below);

  addSlider(release_ = new SynthSlider(value_prepend + "_release"));
  release_->setSliderStyle(Slider::LinearBar);
  release_->setPopupPlacement(BubbleComponent::below);

  addSlider(sustain_ = new SynthSlider(value_prepend + "_sustain"));
  sustain_->setSliderStyle(Slider::LinearBar);
  sustain_->setPopupPlacement(BubbleComponent::below);

  addOpenGLComponent(envelope_ = new OpenGLEnvelope());
  envelope_->setName(value_prepend + "_envelope");
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

  int title_width = getTitleWidth();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int slider_section_width = size_ratio_ * SLIDER_SECTION_WIDTH;

  SynthSection::paintBackground(g);
  component_shadow.drawForRectangle(g, envelope_->getBounds());

  g.setColour(Colors::background);
  g.fillRect(getWidth() - text_width - slider_section_width, title_width,
             text_width, getHeight() - title_width);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));

  g.drawText(TRANS("A"), attack_->getX() - text_width, attack_->getY(),
             text_width, attack_->getHeight(),
             Justification::centred, true);
  g.drawText(TRANS("D"), decay_->getX() - text_width, decay_->getY(),
             text_width, decay_->getHeight(),
             Justification::centred, true);
  g.drawText(TRANS("S"), sustain_->getX() - text_width, sustain_->getY(),
             text_width, sustain_->getHeight(),
             Justification::centred, true);
  g.drawText(TRANS("R"), release_->getX() - text_width, release_->getY(),
             text_width, release_->getHeight(),
             Justification::centred, true);
}

void EnvelopeSection::resized() {
  int title_width = getTitleWidth();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int slider_section_width = size_ratio_ * SLIDER_SECTION_WIDTH;
  int slider_width = size_ratio_ * SLIDER_WIDTH;
  int mod_section_width = size_ratio_ * MOD_SECTION_WIDTH;
  int mod_button_width = getModButtonWidth();

  int envelope_width = getWidth() - slider_section_width - text_width - mod_section_width;
  envelope_->setBounds(mod_section_width, title_width, envelope_width, getHeight() - title_width);

  float space = (getHeight() - (4.0f * slider_width) - title_width) / 3.0f;
  int x = getWidth() - slider_section_width;

  float mod_button_x = (mod_section_width - mod_button_width) / 2.0f;
  float mod_button_y = (getHeight() - mod_button_width + title_width) / 2.0f;
  modulation_button_->setBounds(mod_button_x, mod_button_y, mod_button_width, mod_button_width);
  attack_->setBounds(x, title_width, slider_section_width, slider_width);
  decay_->setBounds(x, title_width + space + slider_width, slider_section_width, slider_width);
  sustain_->setBounds(x, title_width + 2 * (space + slider_width),
                      slider_section_width, slider_width);
  release_->setBounds(x, title_width + 3 * (space + slider_width),
                      slider_section_width, slider_width);

  SynthSection::resized();
}

void EnvelopeSection::reset() {
  envelope_->resetEnvelopeLine();
  envelope_->repaint();
  SynthSection::reset();
}
