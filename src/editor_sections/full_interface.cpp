/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "full_interface.h"

#include "text_look_and_feel.h"
#include "twytch_engine.h"
#include "twytch_common.h"
#include "synth_gui_interface.h"

FullInterface::FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                             mopo::output_map mono_modulations,
                             mopo::output_map poly_modulations) : SynthSection("full_interface") {
  addSubSection(synthesis_interface_ = new SynthesisInterface (controls));

  addSlider(arp_frequency_ = new SynthSlider ("arp_frequency"));
  arp_frequency_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  arp_frequency_->setLookAndFeel(TextLookAndFeel::instance());

  addSlider(arp_gate_ = new SynthSlider("arp_gate"));
  arp_gate_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(arp_octaves_ = new SynthSlider("arp_octaves"));
  arp_octaves_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

  addSlider(arp_pattern_ = new SynthSlider("arp_pattern"));
  arp_pattern_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
  arp_pattern_->setStringLookup(mopo::strings::arp_patterns);

  addAndMakeVisible(oscilloscope_ = new Oscilloscope (512));
  addButton(arp_on_ = new ToggleButton("arp_on"));
  arp_on_->setButtonText(String::empty);
  arp_on_->setColour(ToggleButton::textColourId, Colours::white);

  addSlider(beats_per_minute_ = new SynthSlider("beats_per_minute"));
  beats_per_minute_->setSliderStyle(Slider::LinearBar);
  beats_per_minute_->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 20);

  addAndMakeVisible(global_tool_tip_ = new GlobalToolTip());

  addSlider(arp_tempo_ = new SynthSlider("arp_tempo"));
  arp_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  arp_tempo_->setLookAndFeel(TextLookAndFeel::instance());
  arp_tempo_->setStringLookup(mopo::strings::synced_frequencies);

  addSlider(arp_sync_ = new TempoSelector("arp_sync"));
  arp_sync_->setSliderStyle (Slider::LinearBar);
  arp_sync_->setStringLookup(mopo::strings::freq_sync_styles);
  arp_sync_->setTempoSlider(arp_tempo_);
  arp_sync_->setFreeSlider(arp_frequency_);

  addAndMakeVisible(patch_browser_ = new PatchBrowser());

  setAllValues(controls);
  createModulationSliders(modulation_sources, mono_modulations, poly_modulations);
  setOpaque(true);
}

FullInterface::~FullInterface() {
  synthesis_interface_ = nullptr;
  arp_frequency_ = nullptr;
  arp_gate_ = nullptr;
  arp_octaves_ = nullptr;
  arp_pattern_ = nullptr;
  oscilloscope_ = nullptr;
  arp_on_ = nullptr;
  beats_per_minute_ = nullptr;
  global_tool_tip_ = nullptr;
  arp_sync_ = nullptr;
  patch_browser_ = nullptr;
}

void FullInterface::paintBackground(Graphics& g) {
  static const DropShadow shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
  static const DropShadow logo_shadow(Colour(0xff000000), 5, Point<int>(0, 0));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_128_2x_png,
                                                      BinaryData::helm_icon_128_2x_pngSize);
  static const Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_32_2x_png,
                                                            BinaryData::helm_icon_32_2x_pngSize);
  g.setColour(Colour(0xff212121));
  g.fillRect(getLocalBounds());

  shadow.drawForRectangle(g, Rectangle<int>(472, 8, 266, 60));
  shadow.drawForRectangle(g, Rectangle<int>(746, 8, 220, 60));

  shadow.drawForRectangle(g, Rectangle<int>(376, 8, 88, 60));
  shadow.drawForRectangle(g, Rectangle<int>(368 - 124, 8, 124, 60));

  g.saveState();
  g.setOrigin(14, 6);
  logo_shadow.drawForImage(g, helm_small);
  g.drawImage(helm, 0, 0, 64, 64, 0, 0, helm.getWidth(), helm.getHeight());
  g.restoreState();

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(472.0f, 8.0f, 266.0f, 60.0f, 3.000f);

  g.setColour(Colour(0xff303030));
  g.fillRect(746, 8, 220, 60);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("GATE"),
             599 -(52 / 2), 54, 52, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("FREQUENCY"),
             510, 54, 58, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("OCTAVES"),
             647 -(60 / 2), 54, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("PATTERN"),
             703 -(60 / 2), 54, 60, 10,
             Justification::centred, true);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("BPM"),
             746, 56, 44, 10,
             Justification::centred, true);

  g.setGradientFill(ColourGradient(Colour(0x00000000), 490.0f, 0.0f,
                                   Colours::black, 494.0f, 0.0f, false));
  g.fillRect(472, 8, 20, 60);

  component_shadow.drawForRectangle(g, patch_browser_->getBounds());

  paintKnobShadows(g);

  g.saveState();
  g.addTransform(AffineTransform::rotation(-mopo::PI / 2.0f, 460, 20));
  g.setColour(Colour(0xff999999));
  g.setFont(roboto_light.withPointHeight(13.40f));
  g.drawText(TRANS("ARP"),
             408, 35, 52, 12,
             Justification::centred, true);
  g.restoreState();
}

void FullInterface::resized() {
  synthesis_interface_->setBounds(8, 72, getWidth() - 16, 672);
  arp_frequency_->setBounds(510, 28, 42, 16);
  arp_sync_->setBounds(552, 28, 16, 16);

  arp_gate_->setBounds(579, 12, 40, 40);
  arp_octaves_->setBounds(627, 12, 40, 40);
  arp_pattern_->setBounds(683, 12, 40, 40);
  oscilloscope_->setBounds(376, 8, 88, 60);
  arp_on_->setBounds(472, 10, 20, 20);
  beats_per_minute_->setBounds(790, 48, 176, 20);
  global_tool_tip_->setBounds(244, 8, 124, 60);
  patch_browser_->setBounds(746, 8, 220, 40);
  modulation_manager_->setBounds(getBounds());
  arp_tempo_->setBounds(arp_frequency_->getBounds());

  SynthSection::resized();
}

void FullInterface::setOutputMemory(const mopo::Memory *output_memory) {
  oscilloscope_->setOutputMemory(output_memory);
}

void FullInterface::createModulationSliders(mopo::output_map modulation_sources,
                                            mopo::output_map mono_modulations,
                                            mopo::output_map poly_modulations) {
  std::map<std::string, SynthSlider*> all_sliders = getAllSliders();
  std::map<std::string, SynthSlider*> modulatable_sliders;

  for (auto destination : mono_modulations) {
    if (all_sliders.count(destination.first))
      modulatable_sliders[destination.first] = all_sliders[destination.first];
  }

  modulation_manager_ = new ModulationManager(modulation_sources,
                                              getAllModulationButtons(),
                                              modulatable_sliders,
                                              mono_modulations, poly_modulations);
  modulation_manager_->setOpaque(false);
  addAndMakeVisible(modulation_manager_);
}

void FullInterface::setToolTipText(String parameter, String value) {
  if (global_tool_tip_)
    global_tool_tip_->setText(parameter, value);
}
