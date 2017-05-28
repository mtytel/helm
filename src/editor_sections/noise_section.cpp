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

#include "noise_section.h"

#include "colors.h"
#include "fonts.h"

#define KNOB_WIDTH 40

NoiseSection::NoiseSection(String name) : SynthSection(name) {
  addSlider(volume_ = new SynthSlider("noise_volume"));
  volume_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
}

NoiseSection::~NoiseSection() {
  volume_ = nullptr;
}

void NoiseSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  SynthSection::paintBackground(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("AMP"), volume_);
}

void NoiseSection::resized() {
  volume_->setBounds((getWidth() - KNOB_WIDTH) / 2, 30,
                     KNOB_WIDTH, KNOB_WIDTH);
  SynthSection::resized();
}
