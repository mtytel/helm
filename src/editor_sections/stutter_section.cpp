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

#include "stutter_section.h"
#include "fonts.h"

#define KNOB_WIDTH 40

StutterSection::StutterSection(String name) : SynthSection(name) {
  addSlider(stutter_rate_ = new SynthSlider("stutter_frequency"));
  stutter_rate_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(resample_rate_ = new SynthSlider("stutter_resample_frequency"));
  resample_rate_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(stutter_softness_ = new SynthSlider("stutter_softness"));
  stutter_softness_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(on_ = new ToggleButton("stutter_on"));
  setActivator(on_);
}

StutterSection::~StutterSection() {
  on_ = nullptr;
  stutter_rate_ = nullptr;
  resample_rate_ = nullptr;
}

void StutterSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(Fonts::getInstance()->proportional_regular().withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("FREQ"), stutter_rate_);
  drawTextForComponent(g, TRANS("RESAMPLE"), resample_rate_);
  drawTextForComponent(g, TRANS("SOFT"), stutter_softness_);
}

void StutterSection::resized() {
  float space = (getWidth() - (3.0f * KNOB_WIDTH)) / 4.0f;
  int y = (getHeight() - KNOB_WIDTH) / 2;

  on_->setBounds(2, 0, 20, 20);
  stutter_rate_->setBounds(space, y, KNOB_WIDTH, KNOB_WIDTH);
  resample_rate_->setBounds((KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  stutter_softness_->setBounds(2 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);

  SynthSection::resized();
}
