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
#include "text_selector.h"
#include "text_look_and_feel.h"

#define KNOB_WIDTH 40
#define TEXT_WIDTH 40
#define SELECTOR_WIDTH 75
#define TEXT_HEIGHT 16

DynamicSection::DynamicSection(String name) : SynthSection(name) {
  addSlider(portamento_ = new SynthSlider("portamento"));
  portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  TextSelector* port_type = new TextSelector("portamento_type");
  addSlider(portamento_type_ = port_type);
  portamento_type_->setSliderStyle(Slider::LinearBar);
  portamento_type_->setStringLookup(mopo::strings::off_auto_on);
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

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  
  drawTextForComponent(g, TRANS("PORTA"), portamento_);
  drawTextForComponent(g, TRANS("PORTA TYPE"), portamento_type_,
                       4 + (KNOB_WIDTH - TEXT_HEIGHT) / 3);
  drawTextForComponent(g, TRANS("LEGATO"), legato_,
                       4 + (KNOB_WIDTH - TEXT_HEIGHT) / 3);
}

void DynamicSection::resized() {
  float space_x = (getWidth() - (KNOB_WIDTH + SELECTOR_WIDTH + TEXT_WIDTH)) / 4.0f;
  float space_y = (getHeight() - (KNOB_WIDTH + TEXT_HEIGHT)) / 2.0f;
  float extra_text_space = 2 * (KNOB_WIDTH - TEXT_HEIGHT) / 3;

  portamento_->setBounds(space_x, space_y, KNOB_WIDTH, KNOB_WIDTH);
  portamento_type_->setBounds(KNOB_WIDTH + 2 * space_x, space_y + extra_text_space,
                              SELECTOR_WIDTH, TEXT_HEIGHT);
  legato_->setBounds(KNOB_WIDTH + SELECTOR_WIDTH + 3 * space_x, space_y + extra_text_space,
                     TEXT_WIDTH, TEXT_HEIGHT);

  SynthSection::resized();
}
