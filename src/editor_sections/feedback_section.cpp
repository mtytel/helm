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

#include "feedback_section.h"

#include "colors.h"
#include "fonts.h"

FeedbackSection::FeedbackSection(String name) : SynthSection(name) {
  static const int TRANSPOSE_MOUSE_SENSITIVITY = 800;

  addSlider(transpose_ = new SynthSlider("osc_feedback_transpose"));
  transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  transpose_->setBipolar();
  transpose_->setMouseDragSensitivity(TRANSPOSE_MOUSE_SENSITIVITY);

  addSlider(amount_ = new SynthSlider("osc_feedback_amount"));
  amount_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  amount_->setBipolar();

  addSlider(tune_ = new SynthSlider("osc_feedback_tune"));
  tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tune_->setBipolar();
}

FeedbackSection::~FeedbackSection() {
  transpose_ = nullptr;
  amount_ = nullptr;
  tune_ = nullptr;
}

void FeedbackSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));
  
  drawTextForComponent(g, TRANS("TRANSPOSE"), transpose_);
  drawTextForComponent(g, TRANS("TUNE"), tune_);
  drawTextForComponent(g, TRANS("AMOUNT"), amount_);
}

void FeedbackSection::resized() {
  int knob_width = getStandardKnobSize();
  float space = (getWidth() - (3.0f * knob_width)) / 4.0f;
  int y = size_ratio_ * 26;

  transpose_->setBounds(space, y, knob_width, knob_width);
  tune_->setBounds((knob_width + space) + space, y, knob_width, knob_width);
  amount_->setBounds(2 * (knob_width + space) + space, y, knob_width, knob_width);

  SynthSection::resized();
}
