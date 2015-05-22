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

#include "synth_section.h"

#include "synth_gui_interface.h"
#include "synth_slider.h"

#define TITLE_WIDTH 20
#define SHADOW_WIDTH 3

void SynthSection::paint(Graphics& g) {
  static const DropShadow button_shadow(Colour(0xff000000), 3, Point<int>(0, 0));
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  // Draw border.
  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 3.000f);

  // Draw shadow divider.
  float shadow_top = TITLE_WIDTH - SHADOW_WIDTH;
  float shadow_bottom = TITLE_WIDTH;
  g.setGradientFill(ColourGradient(Colour(0x00000000), 0.0f, shadow_top,
                                   Colour(0x55000000), 0.0f, shadow_bottom,
                                   false));
  g.fillRect(0, 0, getWidth(), TITLE_WIDTH);

  // Draw text title.
  g.setColour(Colour(0xff999999));
  g.setFont(roboto_reg.withPointHeight(13.40f).withExtraKerningFactor(0.05f));
  g.drawText(TRANS(getName()), 0, 0, getWidth(), TITLE_WIDTH,
             Justification::centred, true);

  for (auto slider : slider_lookup_)
    slider.second->drawShadow(g);
}

void SynthSection::sliderValueChanged(Slider* moved_slider) {
  std::string name = moved_slider->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->valueChanged(name, moved_slider->getValue());
}

void SynthSection::buttonClicked(juce::Button *clicked_button) {
  std::string name = clicked_button->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->valueChanged(name, clicked_button->getToggleState() ? 1.0 : 0.0);
}

void SynthSection::setAllValues(mopo::control_map& controls) {
  for (auto slider : slider_lookup_) {
    if (controls.count(slider.first))
      slider.second->setValue(controls[slider.first]->value());
  }

  for (auto button : button_lookup_) {
    if (controls.count(button.first)) {
      bool toggle = controls[button.first]->value();
      button.second->setToggleState(toggle, NotificationType::sendNotification);
      button.second->repaint();
    }
  }

  for (auto sub_section : sub_sections_)
    sub_section.second->setAllValues(controls);
  
  repaint();
}

void SynthSection::addButton(Button* button, bool show) {
  button_lookup_[button->getName().toStdString()] = button;
  button->addListener(this);
  if (show)
    addAndMakeVisible(button);
}

void SynthSection::addSlider(SynthSlider* slider, bool show) {
  slider_lookup_[slider->getName().toStdString()] = slider;
  slider->addListener(this);
  if (show)
    addAndMakeVisible(slider);
}

void SynthSection::addSubSection(SynthSection* sub_section, bool show) {
  sub_sections_[sub_section->getName().toStdString()] = sub_section;
  if (show)
    addAndMakeVisible(sub_section);
}

void SynthSection::drawTextForSlider(Graphics &g, String text, SynthSlider *slider) {
  g.drawText(text, slider->getX() - 20, slider->getY() + slider->getHeight() + 6,
             slider->getWidth() + 40, 10, Justification::centred, false);
}