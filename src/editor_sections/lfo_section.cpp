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

#include "colors.h"
#include "fonts.h"
#include "modulation_look_and_feel.h"
#include "text_look_and_feel.h"

#define WAVE_VIEWER_RESOLUTION 128
#define CONTROL_SECTION_WIDTH 45
#define SLIDER_WIDTH 10
#define TEXT_HEIGHT 16
#define TEXT_WIDTH 42

LfoSection::LfoSection(String name, std::string value_prepend, bool retrigger, bool can_animate) :
    SynthSection(name), can_animate_(can_animate) {
  static const int TEMPO_DRAG_SENSITIVITY = 150;

  retrigger_ = new RetriggerSelector(value_prepend + "_retrigger");
  retrigger_->setSliderStyle(Slider::LinearBar);
  retrigger_->setStringLookup(mopo::strings::freq_retrigger_styles);
  if (retrigger)
    addSlider(retrigger_);

  addSlider(amplitude_ = new SynthSlider(value_prepend + "_amplitude"));
  amplitude_->setSliderStyle(Slider::LinearBarVertical);
  amplitude_->setBipolar();
  amplitude_->snapToValue(true, 0.0);

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

  addOpenGLComponent(wave_viewer_ = new OpenGLWaveViewer(WAVE_VIEWER_RESOLUTION));
  wave_viewer_->setAmplitudeSlider(amplitude_);
  wave_viewer_->setWaveSlider(wave_selector_);
  wave_viewer_->setName(value_prepend);

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
  g.setColour(Colors::control_label_text);
  float text_height = size_ratio_ * 10.0f;
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(text_height));
  float text_slider_height = size_ratio_ * TEXT_HEIGHT;
  float text_buffer = size_ratio_ * 6.0f;

  if (retrigger_->isVisible()) {
    g.drawText(TRANS("FREQUENCY"),
               retrigger_->getBounds().getX(), frequency_->getBounds().getBottom() + text_buffer,
               frequency_->getBounds().getWidth() + 2 * text_slider_height, text_height + 1,
               Justification::centred, false);
  }
  else {
    float extra_bump = size_ratio_ * 5.0f;
    g.drawText(TRANS("FREQUENCY"),
               frequency_->getBounds().getX() - extra_bump,
               frequency_->getBounds().getBottom() + text_buffer,
               frequency_->getBounds().getWidth() + text_slider_height + 2 * extra_bump,
               text_height + 1,
               Justification::centred, false);
  }

  component_shadow.drawForRectangle(g, wave_viewer_->getBounds());
}

void LfoSection::resized() {
  int control_section_width = size_ratio_ * CONTROL_SECTION_WIDTH;
  int y = getHeight() - control_section_width + size_ratio_ * 6.0f;
  int mod_button_width = getModButtonWidth();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int text_height = size_ratio_ * TEXT_HEIGHT;
  int slider_width = size_ratio_ * SLIDER_WIDTH;
  int title_width = getTitleWidth();
  int mod_button_x = size_ratio_ * 10.0f;

  int wave_height = getHeight() - title_width - control_section_width - slider_width;
  wave_selector_->setBounds(slider_width, title_width, getWidth() - slider_width, slider_width);
  wave_viewer_->setBounds(slider_width, title_width + slider_width,
                          getWidth() - slider_width, wave_height);

  amplitude_->setBounds(0, title_width + slider_width, slider_width, wave_height);

  modulation_button_->setBounds(mod_button_x, y, mod_button_width, mod_button_width);
  retrigger_->setBounds(proportionOfWidth(0.5f) - text_height, y, text_height, text_height);

  frequency_->setBounds(proportionOfWidth(0.5f), y, text_width, text_height);
  sync_->setBounds(frequency_->getBounds().getX() + text_width, frequency_->getBounds().getY(),
                   text_height, text_height);

  tempo_->setBounds(frequency_->getBounds());

  SynthSection::resized();

  frequency_->setPopupDisplayEnabled(false, false, nullptr);
  tempo_->setPopupDisplayEnabled(false, false, nullptr);
}

void LfoSection::reset() {
  if (wave_viewer_) {
    wave_viewer_->resetWavePath();
    wave_viewer_->repaint();
  }

  SynthSection::reset();
}
