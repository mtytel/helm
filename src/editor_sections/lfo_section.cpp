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

#include "lfo_section.h"

#include "fonts.h"
#include "modulation_look_and_feel.h"
#include "text_look_and_feel.h"

#define WAVE_VIEWER_RESOLUTION 128
#define KNOB_SECTION_WIDTH 45
#define KNOB_WIDTH 32
#define SLIDER_WIDTH 10
#define TEXT_HEIGHT 16
#define TEXT_WIDTH 42

LfoSection::LfoSection(String name, std::string value_prepend, bool retrigger) :
    SynthSection(name) {
  static const int TEMPO_DRAG_SENSITIVITY = 150;

  retrigger_ = new RetriggerSelector(value_prepend + "_retrigger");
  retrigger_->setSliderStyle(Slider::LinearBar);
  retrigger_->setStringLookup(mopo::strings::freq_retrigger_styles);
  if (retrigger)
    addSlider(retrigger_);

  addSlider(amplitude_ = new SynthSlider(value_prepend + "_amplitude"));
  amplitude_->setSliderStyle(Slider::LinearBarVertical);
  amplitude_->setBipolar();
  amplitude_->snapToZero(true);

  addSlider(frequency_ = new SynthSlider(value_prepend + "_frequency"));
  frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(tempo_ = new SynthSlider(value_prepend + "_tempo"));
  tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tempo_->setStringLookup(mopo::strings::synced_frequencies);
  tempo_->setLookAndFeel(TextLookAndFeel::instance());
  tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);

  addSlider(sync_ = new TempoSelector(value_prepend + "_sync"));
  sync_->setSliderStyle(Slider::LinearBar);
  sync_->setTempoSlider(tempo_);
  sync_->setFreeSlider(frequency_);
  sync_->setStringLookup(mopo::strings::freq_sync_styles);

  addSlider(wave_selector_ = new WaveSelector(value_prepend + "_waveform"));
  wave_selector_->setSliderStyle(Slider::LinearBar);
  wave_selector_->setStringLookup(mopo::strings::waveforms);

  addAndMakeVisible(wave_viewer_ = new WaveViewer(WAVE_VIEWER_RESOLUTION));
  wave_viewer_->setName(value_prepend);
  wave_viewer_->setAmplitudeSlider(amplitude_);
  wave_viewer_->setWaveSlider(wave_selector_);
  wave_viewer_->setControlRate();

  addModulationButton(modulation_button_ = new ModulationButton(value_prepend));
  modulation_button_->setLookAndFeel(ModulationLookAndFeel::instance());
}

LfoSection::~LfoSection() {
  retrigger_ = nullptr;
  wave_viewer_ = nullptr;
  wave_selector_ = nullptr;
  frequency_ = nullptr;
  tempo_ = nullptr;
  sync_ = nullptr;
}

void LfoSection::paintBackground(Graphics& g) {
  static const DropShadow component_shadow(Colour(0x88000000), 2, Point<int>(0, 1));

  SynthSection::paintBackground(g);
  g.setColour(Colour(0xffbbbbbb));
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));

  if (retrigger_->isVisible()) {
    g.drawText(TRANS("FREQUENCY"),
               retrigger_->getBounds().getX(), frequency_->getBounds().getY() + TEXT_HEIGHT + 6,
               frequency_->getBounds().getWidth() + 2 * TEXT_HEIGHT, 10,
               Justification::centred, false);
  }
  else {
    g.drawText(TRANS("FREQUENCY"),
               frequency_->getBounds().getX() - 5, frequency_->getBounds().getY() + TEXT_HEIGHT + 6,
               frequency_->getBounds().getWidth() + TEXT_HEIGHT + 10, 10,
               Justification::centred, false);
  }

  component_shadow.drawForRectangle(g, wave_viewer_->getBounds());
}

void LfoSection::resized() {
  int wave_height = getHeight() - 20 - KNOB_SECTION_WIDTH - SLIDER_WIDTH;
  wave_selector_->setBounds(SLIDER_WIDTH, 20, getWidth() - SLIDER_WIDTH, SLIDER_WIDTH);
  wave_viewer_->setBounds(SLIDER_WIDTH, 20 + SLIDER_WIDTH, getWidth() - SLIDER_WIDTH, wave_height);
  amplitude_->setBounds(0, 20 + SLIDER_WIDTH, SLIDER_WIDTH, wave_height);

  int y = getHeight() - (KNOB_SECTION_WIDTH + KNOB_WIDTH) / 2;
  modulation_button_->setBounds(10.0f, y, MODULATION_BUTTON_WIDTH, MODULATION_BUTTON_WIDTH);
  retrigger_->setBounds(proportionOfWidth(0.5f) - TEXT_HEIGHT, y, TEXT_HEIGHT, TEXT_HEIGHT);

  frequency_->setBounds(proportionOfWidth(0.5f), y, TEXT_WIDTH, TEXT_HEIGHT);
  sync_->setBounds(frequency_->getBounds().getX() + TEXT_WIDTH, frequency_->getBounds().getY(),
                   TEXT_HEIGHT, TEXT_HEIGHT);

  tempo_->setBounds(frequency_->getBounds());

  SynthSection::resized();
}

void LfoSection::animate(bool animate) {
  SynthSection::animate(animate);
  wave_viewer_->showRealtimeFeedback(animate);
}

void LfoSection::reset() {
  wave_viewer_->resetWavePath();
  wave_viewer_->repaint();
  SynthSection::reset();
}
