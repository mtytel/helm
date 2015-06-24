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

#include "sub_section.h"

#define WAVE_VIEWER_RESOLUTION 80
#define WAVE_SELECTOR_WIDTH 10
#define WAVE_SECTION_WIDTH 100
#define KNOB_WIDTH 40

SubSection::SubSection(String name) : SynthSection(name) {
  addSlider(wave_selector_ = new WaveSelector("sub_waveform"));
  wave_selector_->setSliderStyle(Slider::LinearBar);
  wave_selector_->setStringLookup(mopo::strings::waveforms);

  addAndMakeVisible(wave_viewer_ = new WaveViewer(WAVE_VIEWER_RESOLUTION));
  wave_viewer_->setWaveSlider(wave_selector_);

  addSlider(shuffle_ = new SynthSlider("sub_shuffle"));
  shuffle_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(volume_ = new SynthSlider("sub_volume"));
  volume_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
}

SubSection::~SubSection() {
  volume_ = nullptr;
  wave_viewer_ = nullptr;
  wave_selector_ = nullptr;
}

void SubSection::paintBackground(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));

  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("AMP"), volume_);
  drawTextForComponent(g, TRANS("SHUFFLE"), shuffle_);
}

void SubSection::resized() {
  wave_selector_->setBounds(0, 20, WAVE_SECTION_WIDTH, WAVE_SELECTOR_WIDTH);
  wave_viewer_->setBounds(0, wave_selector_->getBottom(),
                          WAVE_SECTION_WIDTH, getHeight() - wave_selector_->getBottom());

  int knob_y = 30;
  int space = (getWidth() - 2 * KNOB_WIDTH - WAVE_SECTION_WIDTH) / 3;
  shuffle_->setBounds(WAVE_SECTION_WIDTH + space, knob_y,
                      KNOB_WIDTH, KNOB_WIDTH);
  volume_->setBounds(WAVE_SECTION_WIDTH + KNOB_WIDTH + 2 * space, knob_y,
                     KNOB_WIDTH, KNOB_WIDTH);
  SynthSection::resized();
}
