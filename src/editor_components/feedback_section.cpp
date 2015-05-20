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

#include "feedback_section.h"

#define KNOB_WIDTH 40

FeedbackSection::FeedbackSection(String name) : SynthSection(name) {
  addSlider(feedback_transpose_ = new SynthSlider("osc_feedback_transpose"));
  feedback_transpose_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  feedback_transpose_->setBipolar();

  addSlider(feedback_amount_ = new SynthSlider("osc_feedback_amount"));
  feedback_amount_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  feedback_amount_->setBipolar();

  addSlider(feedback_tune_ = new SynthSlider("osc_feedback_tune"));
  feedback_tune_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  feedback_tune_->setBipolar();

  addSlider(saturation_ = new SynthSlider("filter_saturation"));
  saturation_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  saturation_->addListener(this);
}

FeedbackSection::~FeedbackSection() {
  feedback_transpose_ = nullptr;
  feedback_amount_ = nullptr;
  feedback_tune_ = nullptr;
  saturation_ = nullptr;
}

void FeedbackSection::paint(Graphics& g) {
  static const DropShadow component_shadow(Colour(0xbb000000), 4, Point<int>(0, 0));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paint(g);

  g.setColour(Colour(0xffbbbbbb));

  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextForSlider(g, TRANS("TRANSPOSE"), feedback_transpose_);
  drawTextForSlider(g, TRANS("TUNE"), feedback_tune_);
  drawTextForSlider(g, TRANS("AMOUNT"), feedback_amount_);
  drawTextForSlider(g, TRANS("SATURATION"), saturation_);
}

void FeedbackSection::resized() {
  float space = (getWidth() - (4.0f * KNOB_WIDTH)) / 5.0f;
  int y = 30;

  feedback_transpose_->setBounds(space, y, KNOB_WIDTH, KNOB_WIDTH);
  feedback_tune_->setBounds((KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  feedback_amount_->setBounds(2 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  saturation_->setBounds(3 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
}
