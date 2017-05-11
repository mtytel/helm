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
#include "text_look_and_feel.h"

#define KNOB_WIDTH 40
#define TEXT_WIDTH 40
#define TEXT_HEIGHT 16

DynamicSection::DynamicSection(String name) : SynthSection(name) {
  addSlider(portamento_ = new SynthSlider("portamento"));
  portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(portamento_type_ = new SynthSlider("portamento_type"));
  portamento_type_->setSliderStyle(Slider::LinearBar);
  portamento_type_->setStringLookup(mopo::strings::off_auto_on);

  addButton(legato_ = new ToggleButton("legato"));
  legato_->setLookAndFeel(TextLookAndFeel::instance());
  legato_->setButtonText("");
}

DynamicSection::~DynamicSection() {
  portamento_ = nullptr;
  portamento_type_ = nullptr;
  legato_ = nullptr;
}

void DynamicSection::paintBackground(Graphics& g) {
  SynthSection::paintContainer(g);

  g.setColour(Colors::controlLabelText);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  
  drawTextForComponent(g, TRANS("PORTA"), portamento_);
  drawTextForComponent(g, TRANS("PORTA TYPE"), portamento_type_,
                       4 + (KNOB_WIDTH - TEXT_HEIGHT) / 3);
  drawTextForComponent(g, TRANS("LEGATO"), legato_,
                       4 + (KNOB_WIDTH - TEXT_HEIGHT) / 3);

  static const int ROOM = 20;
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(8.0f));
  int type_y = portamento_type_->getY() - 12;
  g.drawText(TRANS("OFF"), portamento_type_->getX() - ROOM,
             type_y, 2 * ROOM, 10, Justification::centred, false);
  g.drawText(TRANS("AUTO"), portamento_type_->getX() - ROOM + portamento_type_->getWidth() / 2,
             type_y, 2 * ROOM, 10, Justification::centred, false);
  g.drawText(TRANS("ON"), portamento_type_->getRight() - ROOM,
             type_y, 2 * ROOM, 10, Justification::centred, false);
}

void DynamicSection::resized() {
  float space_x = (getWidth() - (3.0f * KNOB_WIDTH)) / 4.0f;
  float space_y = (getHeight() - (KNOB_WIDTH + TEXT_HEIGHT)) / 2.0f;
  float extra_text_space = 2 * (KNOB_WIDTH - TEXT_HEIGHT) / 3;

  portamento_->setBounds(space_x, space_y, KNOB_WIDTH, KNOB_WIDTH);
  portamento_type_->setBounds(KNOB_WIDTH + 2 * space_x, space_y + extra_text_space,
                              TEXT_WIDTH, TEXT_HEIGHT);
  legato_->setBounds(2 * KNOB_WIDTH + 3 * space_x, space_y + extra_text_space,
                     TEXT_WIDTH, TEXT_HEIGHT);

  SynthSection::resized();
}
