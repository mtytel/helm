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

#include "reverb_section.h"

#include "colors.h"
#include "fonts.h"
#include "synth_button.h"

ReverbSection::ReverbSection(String name) : SynthSection(name) {

  addSlider(feedback_ = new SynthSlider("reverb_feedback"));
  feedback_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(damping_ = new SynthSlider("reverb_damping"));
  damping_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(dry_wet_ = new SynthSlider("reverb_dry_wet"));
  dry_wet_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(on_ = new SynthButton("reverb_on"));
  setActivator(on_);
}

ReverbSection::~ReverbSection() {
  on_ = nullptr;
  feedback_ = nullptr;
  damping_ = nullptr;
  dry_wet_ = nullptr;
}

void ReverbSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));
  
  drawTextForComponent(g, TRANS("FEEDB"), feedback_);
  drawTextForComponent(g, TRANS("DAMP"), damping_);
  drawTextForComponent(g, TRANS("MIX"), dry_wet_);
}

void ReverbSection::resized() {
  int title_width = getTitleWidth();
  on_->setBounds(size_ratio_ * 2.0f, 0, title_width, title_width);

  int knob_width = getStandardKnobSize();
  float space = (getWidth() - (3.0f * knob_width)) / 4.0f;
  int y = size_ratio_ * 30;

  feedback_->setBounds(space, y, knob_width, knob_width);
  damping_->setBounds((knob_width + space) + space, y, knob_width, knob_width);
  dry_wet_->setBounds(2 * (knob_width + space) + space, y, knob_width, knob_width);

  SynthSection::resized();
}
