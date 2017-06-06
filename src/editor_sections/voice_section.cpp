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

#include "voice_section.h"

#include "colors.h"
#include "fonts.h"
#include "text_look_and_feel.h"

#define TEXT_WIDTH 40
#define TEXT_HEIGHT 16

VoiceSection::VoiceSection(String name) : SynthSection(name) {
  static const int KNOB_SENSITIVITY = 500;

  addSlider(polyphony_ = new SynthSlider("polyphony"));
  polyphony_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  polyphony_->setMouseDragSensitivity(KNOB_SENSITIVITY);
  polyphony_->setPopupPlacement(BubbleComponent::above, 0);

  addSlider(velocity_track_ = new SynthSlider("velocity_track"));
  velocity_track_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  velocity_track_->setPopupPlacement(BubbleComponent::above, 0);

  addSlider(pitch_bend_ = new SynthSlider("pitch_bend_range"));
  pitch_bend_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  pitch_bend_->setMouseDragSensitivity(KNOB_SENSITIVITY);
  pitch_bend_->setPopupPlacement(BubbleComponent::above, 0);
}

VoiceSection::~VoiceSection() {
  polyphony_ = nullptr;
  pitch_bend_ = nullptr;
  velocity_track_ = nullptr;
}

void VoiceSection::paintBackground(Graphics& g) {
  paintContainer(g);
  paintKnobShadows(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));
  drawTextForComponent(g, TRANS("VOICES"), polyphony_);
  drawTextForComponent(g, TRANS("PITCH BEND"), pitch_bend_);
  drawTextForComponent(g, TRANS("VEL TRACK"), velocity_track_);
}

void VoiceSection::resized() {
  int knob_width = getStandardKnobSize();
  int text_height = size_ratio_ * TEXT_HEIGHT;

  float space_x = (getWidth() - (3.0f * knob_width)) / 4.0f;
  float space_y = (getHeight() - (knob_width + text_height)) / 2.0f;

  polyphony_->setBounds(space_x, space_y, knob_width, knob_width);
  pitch_bend_->setBounds(knob_width + 2 * space_x, space_y, knob_width, knob_width);
  velocity_track_->setBounds(2 * knob_width + 3 * space_x, space_y, knob_width, knob_width);

  SynthSection::resized();
}
