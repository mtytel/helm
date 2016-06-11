/* Copyright 2013-2016 Matt Tytel
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
#include "fonts.h"

#define KNOB_WIDTH 40

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

  addSlider(saturation_ = new SynthSlider("filter_saturation"));
  saturation_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
}

FeedbackSection::~FeedbackSection() {
  transpose_ = nullptr;
  amount_ = nullptr;
  tune_ = nullptr;
  saturation_ = nullptr;
}

void FeedbackSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));

  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("TRANSPOSE"), transpose_);
  drawTextForComponent(g, TRANS("TUNE"), tune_);
  drawTextForComponent(g, TRANS("AMOUNT"), amount_);
  drawTextForComponent(g, TRANS("SATURATION"), saturation_);
}

void FeedbackSection::resized() {
  float space = (getWidth() - (4.0f * KNOB_WIDTH)) / 5.0f;
  int y = 30;

  transpose_->setBounds(space, y, KNOB_WIDTH, KNOB_WIDTH);
  tune_->setBounds((KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  amount_->setBounds(2 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  saturation_->setBounds(3 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);

  SynthSection::resized();
}
