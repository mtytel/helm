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

#include "synth_section.h"

#include "synth_gui_interface.h"
#include "synth_slider.h"

#define TITLE_WIDTH 20
#define SHADOW_WIDTH 3

void SynthSection::resized() {
  Component::resized();

  const Desktop::Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = Image(Image::ARGB, scale * getWidth(), scale * getHeight(), true);
  Graphics g(background_);
  g.addTransform(AffineTransform::scale(scale, scale));

  paintBackground(g);
}

void SynthSection::paint(Graphics& g) {
  g.drawImage(background_,
              0, 0, getWidth(), getHeight(),
              0, 0, background_.getWidth(), background_.getHeight());
}

void SynthSection::paintBackground(Graphics& g) {
  static const DropShadow button_shadow(Colour(0xff000000), 3, Point<int>(0, 0));
  static Font title_font(Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf,
                                                          BinaryData::RobotoLight_ttfSize));
  paintContainer(g);
  // Draw shadow divider.
  float shadow_top = TITLE_WIDTH - SHADOW_WIDTH;
  float shadow_bottom = TITLE_WIDTH;
  g.setGradientFill(ColourGradient(Colour(0x22000000), 0.0f, shadow_top,
                                   Colour(0x66000000), 0.0f, shadow_bottom,
                                   false));
  g.fillRoundedRectangle(0, 0, getWidth(), TITLE_WIDTH, 1.0f);

  // Draw text title.
  g.setColour(Colour(0xff999999));
  g.setFont(title_font.withPointHeight(14.0f));
  g.drawText(TRANS(getName()), 0, 0, getWidth(), TITLE_WIDTH,
             Justification::centred, true);

  paintKnobShadows(g);
}

void SynthSection::paintContainer(Graphics& g) {
  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 3.000f);
}

void SynthSection::paintKnobShadows(Graphics& g) {
  for (auto slider : slider_lookup_)
    slider.second->drawShadow(g);
}

void SynthSection::sliderValueChanged(Slider* moved_slider) {
  std::string name = moved_slider->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  parent->valueChangedInternal(name, moved_slider->getValue());
}

void SynthSection::buttonClicked(juce::Button *clicked_button) {
  std::string name = clicked_button->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->valueChangedInternal(name, clicked_button->getToggleState() ? 1.0 : 0.0);

  if (clicked_button == activator_)
    setActive(activator_->getToggleStateValue().getValue());
}

void SynthSection::addButton(Button* button, bool show) {
  button_lookup_[button->getName().toStdString()] = button;
  all_buttons_[button->getName().toStdString()] = button;
  button->addListener(this);
  if (show)
    addAndMakeVisible(button);
}

void SynthSection::addModulationButton(ModulationButton* button, bool show) {
  modulation_buttons_[button->getName().toStdString()] = button;
  all_modulation_buttons_[button->getName().toStdString()] = button;
  if (show)
    addAndMakeVisible(button);
}

void SynthSection::addSlider(SynthSlider* slider, bool show) {
  slider_lookup_[slider->getName().toStdString()] = slider;
  all_sliders_[slider->getName().toStdString()] = slider;
  slider->addListener(this);
  if (show)
    addAndMakeVisible(slider);
}

void SynthSection::addSubSection(SynthSection* sub_section, bool show) {
  sub_sections_[sub_section->getName().toStdString()] = sub_section;

  std::map<std::string, SynthSlider*> sub_sliders = sub_section->getAllSliders();
  all_sliders_.insert(sub_sliders.begin(), sub_sliders.end());

  std::map<std::string, Button*> sub_buttons = sub_section->getAllButtons();
  all_buttons_.insert(sub_buttons.begin(), sub_buttons.end());

  std::map<std::string, ModulationButton*> sub_mod_buttons = sub_section->getAllModulationButtons();
  all_modulation_buttons_.insert(sub_mod_buttons.begin(), sub_mod_buttons.end());

  if (show)
    addAndMakeVisible(sub_section);
}

void SynthSection::setActivator(ToggleButton* activator) {
  activator_ = activator;
  setActive(activator_->getToggleStateValue().getValue());
}

void SynthSection::drawTextForComponent(Graphics &g, String text, Component *component) {
  static const int ROOM = 20;
  static const int HEIGHT = 10;
  static const int SPACE = 6;
  g.drawText(text, component->getX() - ROOM, component->getY() + component->getHeight() + SPACE,
             component->getWidth() + 2 * ROOM, HEIGHT, Justification::centred, false);
}

void SynthSection::setActive(bool active) {
  for (auto slider : slider_lookup_)
    slider.second->setActive(active);
  for (auto sub_section : sub_sections_)
    sub_section.second->setActive(active);
}

void SynthSection::setAllValues(mopo::control_map& controls) {
  for (auto slider : all_sliders_) {
    if (controls.count(slider.first))
      slider.second->setValue(controls[slider.first]->value());
  }

  for (auto button : all_buttons_) {
    if (controls.count(button.first)) {
      bool toggle = controls[button.first]->value();
      button.second->setToggleState(toggle, NotificationType::sendNotificationSync);
    }
  }

  repaint();
}

void SynthSection::setValue(std::string name, mopo::mopo_float value,
                            NotificationType notification) {
  if (all_sliders_.count(name))
    all_sliders_[name]->setValue(value, notification);
  else if (all_buttons_.count(name))
    all_buttons_[name]->setToggleState(value, notification);
}
