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

#include "delay_section.h"

#include "colors.h"
#include "fonts.h"
#include "synth_button.h"
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

  addButton(on_ = new SynthButton("delay_on"));
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

  int text_height = size_ratio_ * TEXT_HEIGHT;
  float font_size = size_ratio_ * 10.0f;
  int knob_width = getStandardKnobSize();

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(font_size));
  
  drawTextForComponent(g, TRANS("FEEDB"), feedback_);
  drawTextForComponent(g, TRANS("MIX"), dry_wet_);
  g.drawText(TRANS("FREQUENCY"),
             frequency_->getBounds().getX() - size_ratio_ * 5.0f,
             feedback_->getBounds().getY() + knob_width + size_ratio_ * 4,
             frequency_->getBounds().getWidth() + text_height + size_ratio_ * 10,
             size_ratio_ * 10.0f, Justification::centred, false);
}

void DelaySection::resized() {
  int title_width = getTitleWidth();
  on_->setBounds(size_ratio_ * 2.0f, 0, title_width, title_width);
  int knob_width = getStandardKnobSize();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int text_height = size_ratio_ * TEXT_HEIGHT;

  float space = (getWidth() - (2.0f * knob_width) - text_width - text_height) / 4.0f;
  int knob_y = size_ratio_ * 30;
  int text_y = size_ratio_ * 44;

  frequency_->setBounds(space, text_y, text_width, text_height);
  sync_->setBounds(space + text_width, text_y, text_height, text_height);
  tempo_->setBounds(frequency_->getBounds());
  feedback_->setBounds(text_width + text_height + 2 * space, knob_y, knob_width, knob_width);
  dry_wet_->setBounds(text_width + text_height + knob_width + 3 * space, knob_y,
                      knob_width, knob_width);

  SynthSection::resized();

  frequency_->setPopupDisplayEnabled(false, false, nullptr);
  tempo_->setPopupDisplayEnabled(false, false, nullptr);
}
