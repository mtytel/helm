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

#include "stutter_section.h"

#define KNOB_WIDTH 40

StutterSection::StutterSection(String name) : SynthSection(name) {
  addButton(on_ = new ToggleButton("stutter_on"));

  addSlider(stutter_rate_ = new SynthSlider("stutter_frequency"));
  stutter_rate_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(resample_rate_ = new SynthSlider("stutter_resample_frequency"));
  resample_rate_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
}

StutterSection::~StutterSection() {
  on_ = nullptr;
  stutter_rate_ = nullptr;
  resample_rate_ = nullptr;
}

void StutterSection::paint(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paint(g);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextForSlider(g, TRANS("FREQ"), stutter_rate_);
  drawTextForSlider(g, TRANS("RESAMPLE"), resample_rate_);
}

void StutterSection::resized() {
  float space = (getWidth() - (2.0f * KNOB_WIDTH)) / 3.0f;
  int y = (getHeight() - KNOB_WIDTH) / 2;

  on_->setBounds(2, 0, 20, 20);
  stutter_rate_->setBounds(space, y, KNOB_WIDTH, KNOB_WIDTH);
  resample_rate_->setBounds((KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
}
