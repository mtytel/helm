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

#include "synth_section.h"

#include "colors.h"
#include "fonts.h"
#include "open_gl_component.h"
#include "synth_gui_interface.h"
#include "synth_slider.h"

#define TITLE_WIDTH 20
#define SHADOW_WIDTH 3
#define KNOB_SIZE 40
#define SMALL_KNOB_SIZE 32
#define MODULATION_BUTTON_WIDTH 32

void SynthSection::reset() {
  for (auto& sub_section : sub_sections_)
    sub_section.second->reset();
}

void SynthSection::resized() {
  Component::resized();
}

void SynthSection::paint(Graphics& g) { }

void SynthSection::paintBackground(Graphics& g) {
  static const DropShadow button_shadow(Colour(0xff000000), size_ratio_ * 3.0f, Point<int>(0, 0));

  paintContainer(g);
  // Draw shadow divider.
  float shadow_top = size_ratio_ * (TITLE_WIDTH - SHADOW_WIDTH);
  int title_width = getTitleWidth();
  g.setGradientFill(ColourGradient(Colour(0x22000000), 0.0f, shadow_top,
                                   Colour(0x66000000), 0.0f, title_width,
                                   false));
  g.fillRoundedRectangle(0, 0, getWidth(), title_width, 1.0f);

  // Draw text title.
  g.setColour(Colors::tab_heading_text);
  g.setFont(Fonts::instance()->proportional_light().withPointHeight(size_ratio_ * 14.0f));
  g.drawText(TRANS(getName()), 0, 0, getWidth(), title_width,
             Justification::centred, true);

  paintKnobShadows(g);
  paintChildrenBackgrounds(g);
}

void SynthSection::paintContainer(Graphics& g) {
  g.setColour(Colour(0xff303030));
  g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), size_ratio_ * 3.0f);
}

void SynthSection::setSizeRatio(float ratio) {
  size_ratio_ = ratio;

  for (auto& sub_section : sub_sections_)
    sub_section.second->setSizeRatio(ratio);
}

void SynthSection::paintKnobShadows(Graphics& g) {
  for (auto& slider : slider_lookup_)
    slider.second->drawShadow(g);
}

void SynthSection::paintChildrenBackgrounds(Graphics& g) {
  for (auto& sub_section : sub_sections_)
    paintChildBackground(g, sub_section.second);

  for (auto& open_gl_component : open_gl_components_)
    paintOpenGLBackground(g, open_gl_component);
}

void SynthSection::paintChildBackground(Graphics& g, SynthSection* child) {
  g.saveState();
  g.reduceClipRegion(child->getBounds());
  g.setOrigin(child->getPosition());
  child->paintBackground(g);
  g.restoreState();
}

void SynthSection::paintOpenGLBackground(Graphics &g, OpenGLComponent* open_gl_component) {
  g.saveState();
  g.reduceClipRegion(open_gl_component->getBounds());
  g.setOrigin(open_gl_component->getPosition());
  open_gl_component->paintBackground(g);
  g.restoreState();
}

void SynthSection::initOpenGLComponents(OpenGLContext& open_gl_context) {
  for (auto& open_gl_component : open_gl_components_)
    open_gl_component->init(open_gl_context);

  for (auto& sub_section : sub_sections_)
    sub_section.second->initOpenGLComponents(open_gl_context);
}

void SynthSection::renderOpenGLComponents(OpenGLContext& open_gl_context, bool animate) {
  for (auto& open_gl_component : open_gl_components_)
    open_gl_component->render(open_gl_context, animate);

  for (auto& sub_section : sub_sections_)
    sub_section.second->renderOpenGLComponents(open_gl_context, animate);
}

void SynthSection::destroyOpenGLComponents(OpenGLContext& open_gl_context) {
  for (auto& open_gl_component : open_gl_components_)
    open_gl_component->destroy(open_gl_context);

  for (auto& sub_section : sub_sections_)
    sub_section.second->destroyOpenGLComponents(open_gl_context);
}

void SynthSection::sliderValueChanged(Slider* moved_slider) {
  std::string name = moved_slider->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->getSynth()->valueChangedInternal(name, moved_slider->getValue());
}

void SynthSection::buttonClicked(juce::Button *clicked_button) {
  std::string name = clicked_button->getName().toStdString();
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent)
    parent->getSynth()->valueChangedInternal(name, clicked_button->getToggleState() ? 1.0 : 0.0);
}

void SynthSection::guiChanged(SynthButton* button) {
  if (button == activator_)
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

void SynthSection::addOpenGLComponent(OpenGLComponent* open_gl_component) {
  open_gl_components_.insert(open_gl_component);
  addAndMakeVisible(open_gl_component);
}

void SynthSection::setActivator(SynthButton* activator) {
  activator_ = activator;
  setActive(activator_->getToggleStateValue().getValue());
  activator->addButtonListener(this);
}

float SynthSection::getTitleWidth() {
  return size_ratio_ * TITLE_WIDTH;
}

float SynthSection::getStandardKnobSize() {
  return size_ratio_ * KNOB_SIZE;
}

float SynthSection::getSmallKnobSize() {
  return size_ratio_ * SMALL_KNOB_SIZE;
}

float SynthSection::getModButtonWidth() {
  return size_ratio_ * MODULATION_BUTTON_WIDTH;
}

void SynthSection::drawTextForComponent(Graphics &g, String text, Component *component, int space) {
  float room = size_ratio_ * 30.0f;
  float height = size_ratio_ * 10.0f;
  float adjust_space = size_ratio_ * space;
  g.drawText(text, component->getX() - room,
             component->getY() + component->getHeight() + adjust_space,
             component->getWidth() + 2 * room, height, Justification::centred, false);
}

void SynthSection::setActive(bool active) {
  for (auto& slider : slider_lookup_)
    slider.second->setActive(active);
  for (auto& sub_section : sub_sections_)
    sub_section.second->setActive(active);
}

void SynthSection::animate(bool animate) {
  for (auto& sub_section : sub_sections_)
    sub_section.second->animate(animate);
}

void SynthSection::setAllValues(mopo::control_map& controls) {
  for (auto& slider : all_sliders_) {
    if (controls.count(slider.first)) {
      slider.second->setValue(controls[slider.first]->value(),
                              NotificationType::dontSendNotification);
      slider.second->valueChanged();
    }
  }

  for (auto& button : all_buttons_) {
    if (controls.count(button.first)) {
      bool toggle = controls[button.first]->value();
      button.second->setToggleState(toggle, NotificationType::sendNotificationSync);
    }
  }

  repaint();
}

void SynthSection::setValue(const std::string& name, mopo::mopo_float value,
                            NotificationType notification) {
  if (all_sliders_.count(name)) {
    all_sliders_[name]->setValue(value, notification);
    all_sliders_[name]->notifyGuis();
  }
  if (all_buttons_.count(name))
    all_buttons_[name]->setToggleState(value, notification);
}
