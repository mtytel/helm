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

#include "articulation_section.h"
#include "text_look_and_feel.h"

#define KNOB_WIDTH 40
#define TEXT_WIDTH 40
#define TEXT_HEIGHT 16

ArticulationSection::ArticulationSection(String name) : SynthSection(name) {
  addSlider(polyphony_ = new SynthSlider("polyphony"));
  polyphony_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(portamento_ = new SynthSlider("portamento"));
  portamento_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(portamento_type_ = new SynthSlider("portamento_type"));
  portamento_type_->setSliderStyle(Slider::LinearBar);
  portamento_type_->setStringLookup(mopo::strings::off_auto_on);

  addSlider(velocity_track_ = new SynthSlider("velocity_track"));
  velocity_track_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(pitch_bend_ = new SynthSlider("pitch_bend_range"));
  pitch_bend_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(legato_ = new ToggleButton("legato"));
  legato_->setLookAndFeel(TextLookAndFeel::instance());
  legato_->setButtonText("");
}

ArticulationSection::~ArticulationSection() {
  polyphony_ = nullptr;
  portamento_ = nullptr;
  portamento_type_ = nullptr;
  pitch_bend_ = nullptr;
  velocity_track_ = nullptr;
  legato_ = nullptr;
}

void ArticulationSection::paintBackground(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("VOICES"), polyphony_);
  drawTextForComponent(g, TRANS("PORTA"), portamento_);
  drawTextForComponent(g, TRANS("PORTA TYPE"), portamento_type_);
  drawTextForComponent(g, TRANS("PITCH BEND"), pitch_bend_);
  drawTextForComponent(g, TRANS("VEL TRACK"), velocity_track_);
  drawTextForComponent(g, TRANS("LEGATO"), legato_);

  static const int ROOM = 20;
  g.setFont(roboto_reg.withPointHeight(8.0f));
  int type_y = portamento_type_->getY() - 12;
  g.drawText(TRANS("OFF"), portamento_type_->getX() - ROOM,
             type_y, 2 * ROOM, 10, Justification::centred, false);
  g.drawText(TRANS("AUTO"), portamento_type_->getX() - ROOM + portamento_type_->getWidth() / 2,
             type_y, 2 * ROOM, 10, Justification::centred, false);
  g.drawText(TRANS("ON"), portamento_type_->getRight() - ROOM,
             type_y, 2 * ROOM, 10, Justification::centred, false);
}

void ArticulationSection::resized() {
  float space_x = (getWidth() - (2.0f * KNOB_WIDTH)) / 3.0f;
  float space_y = (getHeight() - (2.0f * KNOB_WIDTH + TEXT_HEIGHT)) / 4.0f;

  polyphony_->setBounds(space_x, space_y, KNOB_WIDTH, KNOB_WIDTH);
  pitch_bend_->setBounds(KNOB_WIDTH + 2 * space_x, space_y, KNOB_WIDTH, KNOB_WIDTH);
  portamento_->setBounds(space_x, KNOB_WIDTH + 2 * space_y, KNOB_WIDTH, KNOB_WIDTH);
  velocity_track_->setBounds(KNOB_WIDTH + 2 * space_x, KNOB_WIDTH + 2 * space_y,
                             KNOB_WIDTH, KNOB_WIDTH);
  portamento_type_->setBounds(space_x, 2 * KNOB_WIDTH + 3 * space_y, TEXT_WIDTH, TEXT_HEIGHT);
  legato_->setBounds(KNOB_WIDTH + 2 * space_x, 2 * KNOB_WIDTH + 3 * space_y,
                     TEXT_WIDTH, TEXT_HEIGHT);

  SynthSection::resized();
}
