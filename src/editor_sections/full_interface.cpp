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

#include "full_interface.h"

#include "text_look_and_feel.h"
#include "helm_engine.h"
#include "helm_common.h"
#include "synth_gui_interface.h"

#define TOP_HEIGHT 64

FullInterface::FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                             mopo::output_map mono_modulations,
                             mopo::output_map poly_modulations) : SynthSection("full_interface") {
  addSubSection(synthesis_interface_ = new SynthesisInterface(controls));
  addSubSection(arp_section_ = new ArpSection("ARP"));

  addSlider(beats_per_minute_ = new SynthSlider("beats_per_minute"));
  beats_per_minute_->setSliderStyle(Slider::LinearBar);
  beats_per_minute_->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 20);
  beats_per_minute_->setColour(Slider::textBoxTextColourId, Colours::white);

  addAndMakeVisible(global_tool_tip_ = new GlobalToolTip());

  addAndMakeVisible(patch_browser_ = new PatchBrowser());
  addAndMakeVisible(oscilloscope_ = new Oscilloscope(512));

  setAllValues(controls);
  createModulationSliders(modulation_sources, mono_modulations, poly_modulations);

  logo_button_ = new ImageButton("logo_button");
  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  if (display.scale > 1.5) {
    Image helm = ImageCache::getFromMemory(BinaryData::helm_icon_128_2x_png,
                                           BinaryData::helm_icon_128_2x_pngSize);
    logo_button_->setImages(false, true, false,
                            helm, 1.0, Colours::transparentBlack,
                            helm, 1.0, Colour(0x11ffffff),
                            helm, 1.0, Colour(0x11000000));
  }
  else {
    Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_32_2x_png,
                                                 BinaryData::helm_icon_32_2x_pngSize);
  }
  addAndMakeVisible(logo_button_);
  logo_button_->addListener(this);

  about_section_ = new AboutSection("about");
  addAndMakeVisible(about_section_);

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
  static const DropShadow logo_shadow(Colour(0xff000000), 8, Point<int>(0, 0));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));
  static const Image helm_small = ImageCache::getFromMemory(BinaryData::helm_icon_32_2x_png,
                                                            BinaryData::helm_icon_32_2x_pngSize);
  g.setColour(Colour(0xff212121));
  g.fillRect(getLocalBounds());

  shadow.drawForRectangle(g, arp_section_->getBounds());
  shadow.drawForRectangle(g, global_tool_tip_->getBounds());
  shadow.drawForRectangle(g, oscilloscope_->getBounds());
  shadow.drawForRectangle(g, Rectangle<int>(92, 8, 244, TOP_HEIGHT));

  shadow.drawForRectangle(g, Rectangle<int>(16, 8, 68, 64));
  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(16.0f, 8.0f, 68.0f, 64.0f, 3.0f);

  g.saveState();
  g.setOrigin(18, 8);

  logo_shadow.drawForImage(g, helm_small);
  g.restoreState();
  
  g.setColour(Colour(0xff303030));
  g.fillRect(92, 8, 244, TOP_HEIGHT);

  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  g.drawText(TRANS("BPM"), patch_browser_->getX(), beats_per_minute_->getY(),
             44, beats_per_minute_->getHeight(),
             Justification::centred, false);

  component_shadow.drawForRectangle(g, patch_browser_->getBounds());

  paintKnobShadows(g);
}

void FullInterface::resized() {
  synthesis_interface_->setBounds(8, TOP_HEIGHT + 16,
                                  getWidth() - 12, getHeight() - TOP_HEIGHT - 12);
  oscilloscope_->setBounds(552, 8, 112, TOP_HEIGHT);
  arp_section_->setBounds(oscilloscope_->getRight() + 8, 8, 308, TOP_HEIGHT);
  patch_browser_->setBounds(92, 8, 244, 2 * TOP_HEIGHT / 3);
  beats_per_minute_->setBounds(141, patch_browser_->getBottom(),
                               200, TOP_HEIGHT - patch_browser_->getHeight());
  global_tool_tip_->setBounds(344, 8, 200, TOP_HEIGHT);
  modulation_manager_->setBounds(getBounds());
  about_section_->setBounds(getBounds());
  logo_button_->setBounds(18, 8, 64, 64);

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

void FullInterface::buttonClicked(Button* clicked_button) {
  if (clicked_button == logo_button_)
    about_section_->setVisible(true);
  else
    SynthSection::buttonClicked(clicked_button);
}