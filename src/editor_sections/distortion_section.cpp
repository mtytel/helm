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

#include "distortion_section.h"

#include "colors.h"
#include "fonts.h"
#include "synth_button.h"

DistortionSection::DistortionSection(String name) : SynthSection(name) {
  addSlider(type_ = new SynthSlider("distortion_type"));
  type_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  type_->setStringLookup(mopo::strings::distortion_types);

  addSlider(drive_ = new SynthSlider("distortion_drive"));
  drive_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(mix_ = new SynthSlider("distortion_mix"));
  mix_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(on_ = new SynthButton("distortion_on"));
  setActivator(on_);
}

DistortionSection::~DistortionSection() {
  on_ = nullptr;
  type_ = nullptr;
  drive_ = nullptr;
  mix_ = nullptr;
}

void DistortionSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));

  drawTextForComponent(g, TRANS("TYPE"), type_);
  drawTextForComponent(g, TRANS("DRIVE"), drive_);
  drawTextForComponent(g, TRANS("MIX"), mix_);
}

void DistortionSection::resized() {
  int title_width = getTitleWidth();
  int knob_width = getStandardKnobSize();
  on_->setBounds(size_ratio_ * 2.0f, 0, title_width, title_width);

  float space = (getWidth() - (3.0f * knob_width)) / 4.0f;
  int y = size_ratio_ * 30;

  type_->setBounds(space, y, knob_width, knob_width);
  drive_->setBounds((knob_width + space) + space, y, knob_width, knob_width);
  mix_->setBounds(2 * (knob_width + space) + space, y, knob_width, knob_width);

  SynthSection::resized();
}
