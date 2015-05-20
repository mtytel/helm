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

FullInterface::FullInterface (mopo::control_map controls, mopo::output_map modulation_sources,
                              mopo::output_map mono_modulations,
                              mopo::output_map poly_modulations) {
  addAndMakeVisible (synthesis_interface_ = new SynthesisInterface (controls));
  addAndMakeVisible (arp_frequency_ = new SynthSlider ("arp_frequency"));
  arp_frequency_->setRange (-1, 4, 0);
  arp_frequency_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
  arp_frequency_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
  arp_frequency_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
  arp_frequency_->setColour (Slider::textBoxTextColourId, Colours::white);
  arp_frequency_->addListener (this);

  addAndMakeVisible (arp_gate_ = new SynthSlider ("arp_gate"));
  arp_gate_->setRange (0, 1, 0);
  arp_gate_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
  arp_gate_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
  arp_gate_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
  arp_gate_->setColour (Slider::textBoxTextColourId, Colours::white);
  arp_gate_->addListener (this);

  addAndMakeVisible (arp_octaves_ = new SynthSlider ("arp_octaves"));
  arp_octaves_->setRange (1, 4, 1);
  arp_octaves_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
  arp_octaves_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
  arp_octaves_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
  arp_octaves_->setColour (Slider::textBoxTextColourId, Colours::white);
  arp_octaves_->addListener (this);

  addAndMakeVisible (arp_pattern_ = new SynthSlider ("arp_pattern"));
  arp_pattern_->setRange (0, 4, 1);
  arp_pattern_->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
  arp_pattern_->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
  arp_pattern_->setColour (Slider::rotarySliderFillColourId, Colour (0x7fffffff));
  arp_pattern_->setColour (Slider::textBoxTextColourId, Colours::white);
  arp_pattern_->addListener (this);

  addAndMakeVisible (oscilloscope_ = new Oscilloscope (512));
  addAndMakeVisible (arp_on_ = new ToggleButton ("arp_on"));
  arp_on_->setButtonText (String::empty);
  arp_on_->addListener (this);
  arp_on_->setColour (ToggleButton::textColourId, Colours::white);

  addAndMakeVisible (beats_per_minute_ = new SynthSlider ("beats_per_minute"));
  beats_per_minute_->setRange (20, 300, 0);
  beats_per_minute_->setSliderStyle (Slider::LinearBar);
  beats_per_minute_->setTextBoxStyle (Slider::TextBoxAbove, false, 150, 20);
  beats_per_minute_->setColour (Slider::backgroundColourId, Colour (0xff333333));
  beats_per_minute_->setColour (Slider::textBoxTextColourId, Colours::white);
  beats_per_minute_->setColour (Slider::textBoxOutlineColourId, Colour (0x00808080));
  beats_per_minute_->addListener (this);

  addAndMakeVisible (global_tool_tip_ = new GlobalToolTip());
  addAndMakeVisible (arp_sync_ = new TempoSelector ("arp_sync"));
  arp_sync_->setRange (0, 6, 1);
  arp_sync_->setSliderStyle (Slider::LinearBar);
  arp_sync_->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
  arp_sync_->setColour (Slider::backgroundColourId, Colour (0xff333333));
  arp_sync_->setColour (Slider::trackColourId, Colour (0xff9765bc));
  arp_sync_->setColour (Slider::textBoxOutlineColourId, Colour (0xffbbbbbb));
  arp_sync_->addListener (this);

  addAndMakeVisible (patch_browser_ = new PatchBrowser());

  arp_tempo_ = new SynthSlider("arp_tempo");
  addAndMakeVisible(arp_tempo_);
  arp_tempo_->setRange(0, sizeof(mopo::synced_freq_ratios) / sizeof(mopo::Value) - 1, 1);
  arp_tempo_->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
  arp_tempo_->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
  arp_tempo_->setColour(Slider::rotarySliderFillColourId, Colour(0x7fffffff));
  arp_tempo_->setColour(Slider::textBoxTextColourId, Colour(0xffffffff));
  arp_tempo_->setLookAndFeel(TextLookAndFeel::instance());
  arp_tempo_->addListener(this);

  arp_frequency_->setUnits("Hz");
  arp_frequency_->setScalingType(mopo::ValueDetails::kExponential);
  arp_frequency_->setLookAndFeel(TextLookAndFeel::instance());
  arp_tempo_->setStringLookup(mopo::strings::synced_frequencies);
  arp_pattern_->setStringLookup(mopo::strings::arp_patterns);

  arp_sync_->setStringLookup(mopo::strings::freq_sync_styles);
  arp_sync_->setTempoSlider(arp_tempo_);
  arp_sync_->setFreeSlider(arp_frequency_);

  for (int i = 0; i < getNumChildComponents(); ++i) {
    SynthSlider* slider = dynamic_cast<SynthSlider*>(getChildComponent(i));
    if (slider)
      slider_lookup_[slider->getName().toStdString()] = slider;

    Button* button = dynamic_cast<Button*>(getChildComponent(i));
    if (button)
      button_lookup_[button->getName().toStdString()] = button;
  }

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

void FullInterface::paint(Graphics& g) {
  static const DropShadow shadow(Colour(0xcc000000), 3, Point<int>(0, 1));
  static const DropShadow component_shadow(Colour(0xcc000000), 5, Point<int>(0, 1));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  static Font roboto_light(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                             BinaryData::RobotoLight_ttfSize));

  g.setColour(Colour(0xff212121));
  g.fillRect(getLocalBounds());

  shadow.drawForRectangle(g, Rectangle<int>(472, 8, 266, 60));
  shadow.drawForRectangle(g, Rectangle<int>(16, 8, 220, 60));

  shadow.drawForRectangle(g, Rectangle<int>(376, 8, 88, 60));
  shadow.drawForRectangle(g, Rectangle<int>(368 - 124, 8, 124, 60));

  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(472.0f, 8.0f, 266.0f, 60.0f, 3.000f);

  g.setColour(Colour(0xff303030));
  g.fillRect(16, 8, 220, 60);

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
             38 -(44 / 2), 56, 44, 10,
             Justification::centred, true);

  g.setGradientFill(ColourGradient(Colour(0x00000000), 490.0f, 0.0f,
                                   Colours::black, 494.0f, 0.0f, false));
  g.fillRect(472, 8, 20, 60);

  component_shadow.drawForRectangle(g, patch_browser_->getBounds());

  for (auto slider : slider_lookup_)
    slider.second->drawShadow(g);

  g.addTransform(AffineTransform::rotation(-mopo::PI / 2.0f, 460, 20));
  g.setColour(Colour(0xff999999));
  g.setFont(roboto_light.withPointHeight(13.40f));
  g.drawText(TRANS("ARP"),
             408, 35, 52, 12,
             Justification::centred, true);
}

void FullInterface::resized() {
  synthesis_interface_->setBounds(746 - 738, 744 - 672, 738, 672);
  arp_frequency_->setBounds(510, 28, 42, 16);
  arp_sync_->setBounds(552, 28, 16, 16);

  arp_gate_->setBounds(599 - (40 / 2), 12, 40, 40);
  arp_octaves_->setBounds(647 - (40 / 2), 12, 40, 40);
  arp_pattern_->setBounds(703 - (40 / 2), 12, 40, 40);
  oscilloscope_->setBounds(376, 8, 88, 60);
  arp_on_->setBounds(474, 12, 16, 16);
  beats_per_minute_->setBounds(148 - (176 / 2), 48, 176, 20);
  global_tool_tip_->setBounds(368 - 124, 8, 124, 60);
  patch_browser_->setBounds(16, 8, 220, 40);
  modulation_manager_->setBounds(getBounds());
  arp_tempo_->setBounds(arp_frequency_->getBounds());
}

void FullInterface::sliderValueChanged(Slider* sliderThatWasMoved) {
  std::string name = sliderThatWasMoved->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->valueChanged(name, sliderThatWasMoved->getValue());
}

void FullInterface::buttonClicked(Button* buttonThatWasClicked) {
  if (buttonThatWasClicked == arp_on_) {
    std::string name = buttonThatWasClicked->getName().toStdString();
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent)
      parent->valueChanged(name, buttonThatWasClicked->getToggleState() ? 1.0 : 0.0);
  }
}

void FullInterface::setAllValues(mopo::control_map& controls) {
  for (auto slider : slider_lookup_)
    slider.second->setValue(controls[slider.first]->value());

  for (auto button : button_lookup_) {
    if (controls.count(button.first)) {
      button.second->setToggleState((bool)controls[button.first]->value(),
                                    NotificationType::sendNotification);
    }
  }
  synthesis_interface_->setAllValues(controls);
}

void FullInterface::setValue(std::string name, mopo::mopo_float value,
                             NotificationType notification) {
  if (slider_lookup_.count(name))
    slider_lookup_[name]->setValue(value, notification);
  else
    synthesis_interface_->setValue(name, value, notification);
}

void FullInterface::setOutputMemory(const mopo::Memory *output_memory) {
  oscilloscope_->setOutputMemory(output_memory);
}

SynthSlider* FullInterface::getSlider(std::string name) {
  if (slider_lookup_.count(name))
    return slider_lookup_[name];
  return synthesis_interface_->getSlider(name);
}

void FullInterface::createModulationSliders(mopo::output_map modulation_sources,
                                            mopo::output_map mono_modulations,
                                            mopo::output_map poly_modulations) {
  std::map<std::string, SynthSlider*> modulatable_sliders;

  for (auto destination : mono_modulations) {
    SynthSlider* slider = getSlider(destination.first);
    if (slider)
      modulatable_sliders[destination.first] = slider;
  }
  modulation_manager_ = new ModulationManager(modulation_sources,
                                              modulatable_sliders,
                                              mono_modulations, poly_modulations);
  modulation_manager_->setOpaque(false);
  addAndMakeVisible(modulation_manager_);
}

void FullInterface::changeModulator(std::string source) {
  modulation_manager_->changeModulator(source);
}

void FullInterface::forgetModulator() {
  modulation_manager_->forgetModulator();
}

void FullInterface::modulationChanged(std::string source) {
  synthesis_interface_->modulationChanged(source);
}

std::string FullInterface::getCurrentModulator() {
  return modulation_manager_->getCurrentModulator();
}

void FullInterface::setToolTipText(String parameter, String value) {
  if (global_tool_tip_)
    global_tool_tip_->setText(parameter, value);
}
