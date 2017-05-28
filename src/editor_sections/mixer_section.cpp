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

#include "mixer_section.h"

#include "colors.h"
#include "fonts.h"
#include "modulation_look_and_feel.h"
#include "synth_slider.h"

#define SLIDER_WIDTH 30
#define TEXT_SECTION_WIDTH 18

MixerSection::MixerSection(String name) : SynthSection(name) {
  addSlider(osc_1_ = new SynthSlider("osc_1_volume"));
  osc_1_->setSliderStyle(Slider::LinearBarVertical);
  osc_1_->flipColoring(true);

  addSlider(osc_2_ = new SynthSlider("osc_2_volume"));
  osc_2_->setSliderStyle(Slider::LinearBarVertical);
  osc_2_->flipColoring(true);

  addSlider(sub_ = new SynthSlider("sub_volume"));
  sub_->setSliderStyle(Slider::LinearBarVertical);
  sub_->flipColoring(true);

  addSlider(noise_ = new SynthSlider("noise_volume"));
  noise_->setSliderStyle(Slider::LinearBarVertical);
  noise_->flipColoring(true);
}

MixerSection::~MixerSection() {
  osc_1_ = nullptr;
  osc_2_ = nullptr;
  sub_ = nullptr;
  noise_ = nullptr;
}

void MixerSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  SynthSection::paintBackground(g);

  float text_y = getHeight() - TEXT_SECTION_WIDTH;
  float buffer = (getWidth() - 4 * SLIDER_WIDTH) / 3.0f;
  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));

  g.drawText(TRANS("OSC 1"), 0, text_y,
             SLIDER_WIDTH, TEXT_SECTION_WIDTH,
             Justification::centred, false);

  g.drawText(TRANS("OSC 2"), SLIDER_WIDTH + buffer, text_y,
             SLIDER_WIDTH, TEXT_SECTION_WIDTH,
             Justification::centred, false);

  g.drawText(TRANS("SUB"), 2 * (SLIDER_WIDTH + buffer), text_y,
             SLIDER_WIDTH, TEXT_SECTION_WIDTH,
             Justification::centred, false);

  g.drawText(TRANS("NOISE"), 3 * (SLIDER_WIDTH + buffer), text_y,
             SLIDER_WIDTH, TEXT_SECTION_WIDTH,
             Justification::centred, false);
}

void MixerSection::resized() {
  float buffer = (getWidth() - 4 * SLIDER_WIDTH) / 3.0f;
  float slider_height = getHeight() - TEXT_SECTION_WIDTH - 20;

  osc_1_->setBounds(0, 20, SLIDER_WIDTH, slider_height);
  osc_2_->setBounds(SLIDER_WIDTH + buffer, 20, SLIDER_WIDTH, slider_height);
  sub_->setBounds(2 * (SLIDER_WIDTH + buffer), 20, SLIDER_WIDTH, slider_height);
  noise_->setBounds(3 * (SLIDER_WIDTH + buffer), 20, SLIDER_WIDTH, slider_height);
  SynthSection::resized();
}
