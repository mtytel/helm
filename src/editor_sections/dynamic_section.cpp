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

#include "dynamic_section.h"

#include "colors.h"
#include "fonts.h"
#include "synth_button.h"
#include "text_slider.h"
#include "text_look_and_feel.h"

#define TEXT_WIDTH 40
#define SELECTOR_WIDTH 75
#define TEXT_HEIGHT 16

DynamicSection::DynamicSection(String name) : SynthSection(name) {
  addSlider(portamento_ = new SynthSlider("portamento"));
  portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  portamento_->setPopupPlacement(BubbleComponent::above, 0);

  TextSlider* port_type = new TextSlider("portamento_type");
  addSlider(portamento_type_ = port_type);
  portamento_type_->setSliderStyle(Slider::LinearBar);
  portamento_type_->setStringLookup(mopo::strings::off_auto_on);
  portamento_type_->setPopupPlacement(BubbleComponent::above, 0);
  port_type->setShortStringLookup(mopo::strings::off_auto_on_slider);

  addButton(legato_ = new SynthButton("legato"));
  legato_->setLookAndFeel(TextLookAndFeel::instance());
  legato_->setButtonText("");
}

DynamicSection::~DynamicSection() {
  portamento_ = nullptr;
  portamento_type_ = nullptr;
  legato_ = nullptr;
}

void DynamicSection::paintBackground(Graphics& g) {
  paintContainer(g);
  paintKnobShadows(g);

  int knob_width = getStandardKnobSize();
  int text_height = size_ratio_ * TEXT_HEIGHT;

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));
  
  drawTextForComponent(g, TRANS("PORTA"), portamento_);
  drawTextForComponent(g, TRANS("PORTA TYPE"), portamento_type_,
                       size_ratio_ * 4.0f + (knob_width - text_height) / 3.0f);
  drawTextForComponent(g, TRANS("LEGATO"), legato_,
                       size_ratio_ * 4.0f + (knob_width - text_height) / 3.0f);
}

void DynamicSection::resized() {
  int knob_width = getStandardKnobSize();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int text_height = size_ratio_ * TEXT_HEIGHT;
  int selector_width = size_ratio_ * SELECTOR_WIDTH;

  float space_x = (getWidth() - (knob_width + selector_width + text_width)) / 4.0f;
  float space_y = (getHeight() - (knob_width + text_height)) / 2.0f;
  float extra_text_space = 2 * (knob_width - text_height) / 3;

  portamento_->setBounds(space_x, space_y, knob_width, knob_width);
  portamento_type_->setBounds(knob_width + 2 * space_x, space_y + extra_text_space,
                              selector_width, text_height);
  legato_->setBounds(knob_width + selector_width + 3 * space_x, space_y + extra_text_space,
                     text_width, text_height);

  SynthSection::resized();
}
