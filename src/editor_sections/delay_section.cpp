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

#include "delay_section.h"

#include "fonts.h"
#include "tempo_selector.h"
#include "text_look_and_feel.h"

#define KNOB_WIDTH 40
#define TEXT_WIDTH 42
#define TEXT_HEIGHT 16

DelaySection::DelaySection(String name) : SynthSection(name) {
  static const int TEMPO_DRAG_SENSITIVITY = 150;
  addSlider(frequency_ = new SynthSlider("delay_frequency"));
  frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(tempo_ = new SynthSlider("delay_tempo"));
  tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tempo_->setStringLookup(mopo::strings::synced_frequencies);
  tempo_->setLookAndFeel(TextLookAndFeel::instance());
  tempo_->setMouseDragSensitivity(TEMPO_DRAG_SENSITIVITY);

  addSlider(sync_ = new TempoSelector("delay_sync"));
  sync_->setSliderStyle(Slider::LinearBar);
  sync_->setTempoSlider(tempo_);
  sync_->setFreeSlider(frequency_);
  sync_->setStringLookup(mopo::strings::freq_sync_styles);

  addSlider(feedback_ = new SynthSlider("delay_feedback"));
  feedback_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  feedback_->setBipolar();

  addSlider(dry_wet_ = new SynthSlider("delay_dry_wet"));
  dry_wet_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addButton(on_ = new ToggleButton("delay_on"));
  setActivator(on_);
}

DelaySection::~DelaySection() {
  on_ = nullptr;
  frequency_ = nullptr;
  tempo_ = nullptr;
  sync_ = nullptr;
  feedback_ = nullptr;
  dry_wet_ = nullptr;
}

void DelaySection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colour(0xffbbbbbb));

  g.setFont(Fonts::getInstance()->proportional_regular().withPointHeight(10.0f));
  drawTextForComponent(g, TRANS("FEEDB"), feedback_);
  drawTextForComponent(g, TRANS("WET"), dry_wet_);

  g.drawText(TRANS("FREQUENCY"),
             frequency_->getBounds().getX() - 5, frequency_->getBounds().getY() + TEXT_HEIGHT + 6,
             frequency_->getBounds().getWidth() + TEXT_HEIGHT + 10,
             10, Justification::centred, false);
}

void DelaySection::resized() {
  on_->setBounds(2, 0, 20, 20);

  float space = (getWidth() - (2.0f * KNOB_WIDTH) - TEXT_WIDTH - TEXT_HEIGHT) / 4.0f;
  int knob_y = 36;
  int text_y = 42;

  frequency_->setBounds(space, text_y, TEXT_WIDTH, TEXT_HEIGHT);
  sync_->setBounds(space + TEXT_WIDTH, text_y, TEXT_HEIGHT, TEXT_HEIGHT);
  tempo_->setBounds(frequency_->getBounds());
  feedback_->setBounds(TEXT_WIDTH + TEXT_HEIGHT + 2 * space, knob_y, KNOB_WIDTH, KNOB_WIDTH);
  dry_wet_->setBounds(TEXT_WIDTH + TEXT_HEIGHT + KNOB_WIDTH + 3 * space, knob_y,
                        KNOB_WIDTH, KNOB_WIDTH);

  SynthSection::resized();
}
