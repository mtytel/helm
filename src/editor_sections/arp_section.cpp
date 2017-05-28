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

#define KNOB_WIDTH 40
#define TEXT_WIDTH 42
#define TEXT_HEIGHT 16

#define TITLE_WIDTH 20
#define SHADOW_WIDTH 3

ArpSection::ArpSection(String name) : SynthSection(name) {
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

  addSlider(pattern_ = new SynthSlider("arp_pattern"));
  pattern_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
  pattern_->setStringLookup(mopo::strings::arp_patterns);

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

  float shadow_left = TITLE_WIDTH - SHADOW_WIDTH;
  float shadow_right = TITLE_WIDTH;
  g.setGradientFill(ColourGradient(Colour(0x22000000), shadow_left, 0.0f,
                                   Colour(0x66000000), shadow_right, 0.0f,
                                   false));
  g.fillRoundedRectangle(0, 0, TITLE_WIDTH, getHeight(), 1.0f);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(10.0f));
  
  drawTextForComponent(g, TRANS("GATE"), gate_);
  drawTextForComponent(g, TRANS("OCTAVES"), octaves_);
  drawTextForComponent(g, TRANS("PATTERN"), pattern_);
  g.drawText(TRANS("FREQUENCY"),
             frequency_->getBounds().getX() - 5, gate_->getBounds().getY() + KNOB_WIDTH + 4,
             frequency_->getBounds().getWidth() + TEXT_HEIGHT + 10,
             10, Justification::centred, false);

  g.saveState();
  g.addTransform(AffineTransform::rotation(-mopo::PI / 2.0f, 0, 0));
  g.setColour(Colour(0xff999999));
  g.setFont(Fonts::instance()->proportional_light().withPointHeight(13.40f));
  g.drawText(TRANS("ARP"), -getHeight(), 0, getHeight() - 20, 20, Justification::centred, false);
  g.restoreState();

  paintKnobShadows(g);
}

void ArpSection::resized() {
  float space = (getWidth() - TEXT_WIDTH - TEXT_HEIGHT - (3.0f * KNOB_WIDTH) - 20) / 5.0f;
  int y = 4;

  int text_y = y + (KNOB_WIDTH - TEXT_HEIGHT) / 2;
  frequency_->setBounds(20 + space, text_y, TEXT_WIDTH, TEXT_HEIGHT);
  sync_->setBounds(20 + space + TEXT_WIDTH, text_y, TEXT_HEIGHT, TEXT_HEIGHT);
  gate_->setBounds(20 + 2 * space + TEXT_HEIGHT + TEXT_WIDTH, y, KNOB_WIDTH, KNOB_WIDTH);
  octaves_->setBounds(20 + 3 * space + TEXT_HEIGHT + TEXT_WIDTH + KNOB_WIDTH, y,
                      KNOB_WIDTH, KNOB_WIDTH);
  pattern_->setBounds(20 + 4 * space + TEXT_HEIGHT + TEXT_WIDTH + 2 * KNOB_WIDTH, y,
                      KNOB_WIDTH, KNOB_WIDTH);
  tempo_->setBounds(frequency_->getBounds());
  on_->setBounds(0, 2, 20, 20);

  SynthSection::resized();
}
