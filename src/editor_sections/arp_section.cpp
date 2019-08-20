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

#include "arp_section.h"

#include "colors.h"
#include "fonts.h"
#include "synth_button.h"
#include "text_look_and_feel.h"
#include "text_selector.h"

#define KNOB_WIDTH 40
#define TEXT_WIDTH 42
#define TEXT_HEIGHT 16

#define SHADOW_WIDTH 3

ArpSection::ArpSection(String name) : SynthSection(name) {
  static const int INDEX_DRAG_SENSITIVITY = 60;

  addSlider(frequency_ = new SynthSlider("arp_frequency"));
  frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(tempo_ = new SynthSlider("arp_tempo"));
  tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  tempo_->setLookAndFeel(TextLookAndFeel::instance());
  tempo_->setStringLookup(mopo::strings::synced_frequencies);

  addSlider(sync_ = new TempoSelector("arp_sync"));
  sync_->setStringLookup(mopo::strings::freq_sync_styles);
  sync_->setTempoSlider(tempo_);
  sync_->setFreeSlider(frequency_);

  addSlider(gate_ = new SynthSlider("arp_gate"));
  gate_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(octaves_ = new SynthSlider("arp_octaves"));
  octaves_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  octaves_->setMouseDragSensitivity(INDEX_DRAG_SENSITIVITY);

  addSlider(pattern_ = new TextSelector("arp_pattern"));
  pattern_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  pattern_->setStringLookup(mopo::strings::arp_patterns);
  pattern_->setMouseDragSensitivity(INDEX_DRAG_SENSITIVITY);
  pattern_->setLookAndFeel(TextLookAndFeel::instance());

  addButton(on_ = new SynthButton("arp_on"));
  setActivator(on_);
}

ArpSection::~ArpSection() {
  frequency_ = nullptr;
  tempo_ = nullptr;
  sync_ = nullptr;
  gate_ = nullptr;
  octaves_ = nullptr;
  pattern_ = nullptr;
  on_ = nullptr;
}

void ArpSection::paintBackground(Graphics& g) {
  SynthSection::paintContainer(g);
  float knob_width = getStandardKnobSize();
  float text_height = size_ratio_ * TEXT_HEIGHT;

  int title_width = getTitleWidth();
  float shadow_left = getTitleWidth() - size_ratio_ * SHADOW_WIDTH;

  g.setGradientFill(ColourGradient(Colour(0x22000000), shadow_left, 0.0f,
                                   Colour(0x66000000), title_width, 0.0f,
                                   false));
  g.fillRoundedRectangle(0, 0, title_width, getHeight(), 1.0f);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));
  
  drawTextForComponent(g, TRANS("GATE"), gate_);
  drawTextForComponent(g, TRANS("OCTAVES"), octaves_);
  
  int font_y = gate_->getBounds().getY() + knob_width + size_ratio_ * 4;
  g.drawText(TRANS("FREQUENCY"),
             frequency_->getBounds().getX() - size_ratio_ * 5, font_y,
             frequency_->getBounds().getWidth() + text_height + size_ratio_ * 10,
             size_ratio_ * 10, Justification::centred, false);
  g.drawText(TRANS("PATTERN"),
             pattern_->getBounds().getX() - size_ratio_ * 5, font_y,
             pattern_->getBounds().getWidth() + size_ratio_ * 10,
             size_ratio_ * 10, Justification::centred, false);

  g.saveState();
  g.addTransform(AffineTransform::rotation(-static_cast<float>(mopo::PI) / 2.0f, 0, 0));
  g.setColour(Colors::tab_heading_text);
  g.setFont(Fonts::instance()->proportional_light().withPointHeight(size_ratio_ * 13.40f));
  g.drawText(getName(), -getHeight(), 0, getHeight() - title_width, title_width,
             Justification::centred, false);
  g.restoreState();

  paintKnobShadows(g);
}

void ArpSection::resized() {
  int knob_width = getStandardKnobSize();
  int title_width = getTitleWidth();
  int text_width = size_ratio_ * TEXT_WIDTH;
  int text_height = size_ratio_ * TEXT_HEIGHT;

  float space = (getWidth() - 2 * (text_width + text_height + knob_width) - title_width) / 5.0f;
  int y = size_ratio_ * 4;

  int text_y = y + size_ratio_ * 12;
  frequency_->setBounds(title_width + space, text_y, text_width, text_height);
  sync_->setBounds(title_width + space + text_width, text_y, text_height, text_height);
  gate_->setBounds(title_width + 2 * space + text_height + text_width, y, knob_width, knob_width);
  octaves_->setBounds(title_width + 3 * space + text_height + text_width + knob_width, y,
                      knob_width, knob_width);
  pattern_->setBounds(title_width + 4 * space + text_height + text_width + 2 * knob_width, text_y,
                      text_width + text_height, text_height);
  tempo_->setBounds(frequency_->getBounds());
  on_->setBounds(0, size_ratio_ * 2, title_width, title_width);

  SynthSection::resized();

  frequency_->setPopupDisplayEnabled(false, false, nullptr);
  tempo_->setPopupDisplayEnabled(false, false, nullptr);
}
