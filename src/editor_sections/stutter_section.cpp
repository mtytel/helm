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

#include "stutter_section.h"
#include "fonts.h"
#include "text_look_and_feel.h"

#define TEXT_Y 45
#define TEXT_HEIGHT 16
#define TEXT_WIDTH 42
#define SLIDER_HEIGHT 15

StutterSection::StutterSection(String name) : SynthSection(name) {
  static const int TEMPO_DRAG_SENSITIVITY = 150;

  addSlider(stutter_frequency_ = new SynthSlider("stutter_frequency"));
  stutter_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  stutter_frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(stutter_tempo_ = new SynthSlider("stutter_tempo"));
  stutter_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  stutter_tempo_->setStringLookup(mopo::strings::synced_frequencies);
  stutter_tempo_->setLookAndFeel(TextLookAndFeel::instance());
  stutter_tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);

  addSlider(stutter_sync_ = new TempoSelector("stutter_sync"));
  stutter_sync_->setSliderStyle(Slider::LinearBar);
  stutter_sync_->setTempoSlider(stutter_tempo_);
  stutter_sync_->setFreeSlider(stutter_frequency_);
  stutter_sync_->setStringLookup(mopo::strings::freq_sync_styles);

  addSlider(resample_frequency_ = new SynthSlider("stutter_resample_frequency"));
  resample_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  resample_frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(resample_tempo_ = new SynthSlider("stutter_resample_tempo"));
  resample_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  resample_tempo_->setStringLookup(mopo::strings::synced_frequencies);
  resample_tempo_->setLookAndFeel(TextLookAndFeel::instance());
  resample_tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);

  addSlider(resample_sync_ = new TempoSelector("stutter_resample_sync"));
  resample_sync_->setSliderStyle(Slider::LinearBar);
  resample_sync_->setTempoSlider(resample_tempo_);
  resample_sync_->setFreeSlider(resample_frequency_);
  resample_sync_->setStringLookup(mopo::strings::freq_sync_styles);

  addSlider(stutter_softness_ = new SynthSlider("stutter_softness"));
  stutter_softness_->setSliderStyle(Slider::LinearBar);

  addButton(on_ = new ToggleButton("stutter_on"));
  setActivator(on_);
}

StutterSection::~StutterSection() {
  on_ = nullptr;

  stutter_frequency_ = nullptr;
  stutter_tempo_ = nullptr;
  stutter_sync_ = nullptr;
  
  resample_frequency_ = nullptr;
  resample_tempo_ = nullptr;
  resample_sync_ = nullptr;

  stutter_softness_ = nullptr;
}

void StutterSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  g.drawText(TRANS("FREQUENCY"),
             stutter_frequency_->getBounds().getX() - 5,
             stutter_frequency_->getBounds().getY() + TEXT_HEIGHT + 6,
             stutter_frequency_->getBounds().getWidth() + TEXT_HEIGHT + 10,
             10, Justification::centred, false);
  g.drawText(TRANS("RESAMPLE"),
             resample_frequency_->getBounds().getX() - 5,
             resample_frequency_->getBounds().getY() + TEXT_HEIGHT + 6,
             resample_frequency_->getBounds().getWidth() + TEXT_HEIGHT + 10,
             10, Justification::centred, false);
  drawTextForComponent(g, TRANS("SOFTNESS"), stutter_softness_);
}

void StutterSection::resized() {
  float space = (getWidth() - 2.0f * (TEXT_WIDTH + TEXT_HEIGHT)) / 3.0f;

  on_->setBounds(2, 0, 20, 20);

  stutter_frequency_->setBounds(space, TEXT_Y, TEXT_WIDTH, TEXT_HEIGHT);
  stutter_tempo_->setBounds(stutter_frequency_->getBounds());
  stutter_sync_->setBounds(stutter_frequency_->getBounds().getRight(), TEXT_Y,
                           TEXT_HEIGHT, TEXT_HEIGHT);

  resample_frequency_->setBounds(stutter_sync_->getBounds().getRight() + space, TEXT_Y,
                                 TEXT_WIDTH, TEXT_HEIGHT);
  resample_tempo_->setBounds(resample_frequency_->getBounds());
  resample_sync_->setBounds(resample_frequency_->getBounds().getRight(), TEXT_Y,
                            TEXT_HEIGHT, TEXT_HEIGHT);

  stutter_softness_->setBounds(0, getHeight() - SLIDER_HEIGHT - 32, getWidth(), SLIDER_HEIGHT);
  SynthSection::resized();
}
