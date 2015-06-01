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
  addSubSection(synthesis_interface_ = new SynthesisInterface(controls));
  addSubSection(arp_section_ = new ArpSection("ARP"));

  addSlider(beats_per_minute_ = new SynthSlider("beats_per_minute"));
  beats_per_minute_->setSliderStyle(Slider::LinearBar);
  beats_per_minute_->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 20);

  addAndMakeVisible(global_tool_tip_ = new GlobalToolTip());

  addAndMakeVisible(patch_browser_ = new PatchBrowser());
  addAndMakeVisible(oscilloscope_ = new Oscilloscope(512));

  setAllValues(controls);
  createModulationSliders(modulation_sources, mono_modulations, poly_modulations);
  setOpaque(true);
}

FullInterface::~FullInterface() {
  arp_section_ = nullptr;
  synthesis_interface_ = nullptr;
  oscilloscope_ = nullptr;
  beats_per_minute_ = nullptr;
  global_tool_tip_ = nullptr;
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

  shadow.drawForRectangle(g, arp_section_->getBounds());
  shadow.drawForRectangle(g, Rectangle<int>(80, 8, 220, 60));

  shadow.drawForRectangle(g, Rectangle<int>(376, 8, 88, 60));
  shadow.drawForRectangle(g, Rectangle<int>(368 - 124, 8, 124, 60));

  g.saveState();
  g.setOrigin(14, 6);
  logo_shadow.drawForImage(g, helm_small);
  g.drawImage(helm, 0, 0, 64, 64, 0, 0, helm.getWidth(), helm.getHeight());
  g.restoreState();
  
  g.setColour(Colour(0xff303030));
  g.fillRect(80, 8, 220, 60);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("BPM"),
             80, 56, 44, 10,
             Justification::centred, true);

  component_shadow.drawForRectangle(g, patch_browser_->getBounds());

  paintKnobShadows(g);
}

void FullInterface::resized() {
  static const int arp_width = 308;
  static const int top_height = 64;

  arp_section_->setBounds(getWidth() - arp_width - 16.0f, 8.0f, arp_width, top_height);
  synthesis_interface_->setBounds(8, top_height + 16,
                                  getWidth() - 12, getHeight() - top_height - 12);
  oscilloscope_->setBounds(376, 8, 88, 60);
  beats_per_minute_->setBounds(124, 48, 176, top_height / 3);
  global_tool_tip_->setBounds(244, 8, 124, 60);
  patch_browser_->setBounds(80, 8, 220, 2 * top_height / 3);
  modulation_manager_->setBounds(getBounds());

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
