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

#define KNOB_WIDTH 40

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

  g.setColour(Colors::controlLabelText);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  
  drawTextForComponent(g, TRANS("FEEDB"), feedback_);
  drawTextForComponent(g, TRANS("DAMP"), damping_);
  drawTextForComponent(g, TRANS("WET"), dry_wet_);
}

void ReverbSection::resized() {
  on_->setBounds(2, 0, 20, 20);

  float space = (getWidth() - (3.0f * KNOB_WIDTH)) / 4.0f;
  int y = 36;

  feedback_->setBounds(space, y, KNOB_WIDTH, KNOB_WIDTH);
  damping_->setBounds((KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);
  dry_wet_->setBounds(2 * (KNOB_WIDTH + space) + space, y, KNOB_WIDTH, KNOB_WIDTH);

  SynthSection::resized();
}
