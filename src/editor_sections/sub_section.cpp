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

#include "modulation_look_and_feel.h"
#include "text_look_and_feel.h"

#define WAVE_VIEWER_RESOLUTION 128

SubSection::SubSection(String name) : SynthSection(name) {
  addSlider(wave_selector_ = new WaveSelector("sub_waveform"));
  wave_selector_->setSliderStyle(Slider::LinearBar);
  wave_selector_->setStringLookup(mopo::strings::waveforms);

  addAndMakeVisible(wave_viewer_ = new WaveViewer(WAVE_VIEWER_RESOLUTION));
  wave_viewer_->setWaveSlider(wave_selector_);
}

SubSection::~SubSection() {
  wave_viewer_ = nullptr;
  wave_selector_ = nullptr;
}

void SubSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paintBackground(g);

  component_shadow.drawForRectangle(g, wave_viewer_->getBounds());
}

void SubSection::resized() {
  SynthSection::resized();
}
