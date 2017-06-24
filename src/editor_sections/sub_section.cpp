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

#include "sub_section.h"

#include "colors.h"
#include "fonts.h"
#include "synth_button.h"
#include "text_look_and_feel.h"

#define WAVE_VIEWER_RESOLUTION 80
#define WAVE_SELECTOR_WIDTH 10
#define WAVE_SECTION_WIDTH 100

SubSection::SubSection(String name) : SynthSection(name) {
  addSlider(wave_selector_ = new WaveSelector("sub_waveform"));
  wave_selector_->setSliderStyle(Slider::LinearBar);
  wave_selector_->setStringLookup(mopo::strings::waveforms);

  addAndMakeVisible(wave_viewer_ = new WaveViewer(WAVE_VIEWER_RESOLUTION));
  wave_viewer_->setWaveSlider(wave_selector_);

  addSlider(shuffle_ = new SynthSlider("sub_shuffle"));
  shuffle_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(sub_octave_ = new SynthButton("sub_octave"));
  sub_octave_->setLookAndFeel(TextLookAndFeel::instance());
  sub_octave_->setButtonText("-OCT");
}

SubSection::~SubSection() {
  wave_viewer_ = nullptr;
  wave_selector_ = nullptr;
}

void SubSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));
  drawTextForComponent(g, TRANS("SHUFFLE"), shuffle_);
}

void SubSection::resized() {
  int knob_y = size_ratio_ * 24;
  int knob_width = getStandardKnobSize();
  int title_width = getTitleWidth();
  int wave_section_width = size_ratio_ * WAVE_SECTION_WIDTH;
  int wave_selector_width = size_ratio_ * WAVE_SELECTOR_WIDTH;
  int space = (getWidth() - knob_width - wave_section_width) / 2;

  wave_selector_->setBounds(0, title_width, wave_section_width, wave_selector_width);
  wave_viewer_->setBounds(0, wave_selector_->getBottom(),
                          wave_section_width, getHeight() - wave_selector_->getBottom());

  shuffle_->setBounds(wave_section_width + space, knob_y,
                      knob_width, knob_width);

  int button_y = size_ratio_ * 82;
  sub_octave_->setBounds(wave_section_width + space, button_y, knob_width, size_ratio_ * 16);

  SynthSection::resized();
}

void SubSection::reset() {
  wave_viewer_->resetWavePath();
  wave_viewer_->repaint();
  SynthSection::reset();
}
