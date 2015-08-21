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

#include "extra_mod_section.h"
#include "modulation_look_and_feel.h"

#define BUTTON_WIDTH 32

ExtraModSection::ExtraModSection(String name) : SynthSection(name) {
  addModulationButton(aftertouch_mod_ = new ModulationButton("aftertouch"));
  aftertouch_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(note_mod_ = new ModulationButton("note"));
  note_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(velocity_mod_ = new ModulationButton("velocity"));
  velocity_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(mod_wheel_mod_ = new ModulationButton("mod_wheel"));
  mod_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton(pitch_wheel_mod_ = new ModulationButton("pitch_wheel"));
  pitch_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
}

ExtraModSection::~ExtraModSection() {
  aftertouch_mod_ = nullptr;
  note_mod_ = nullptr;
  velocity_mod_ = nullptr;
  mod_wheel_mod_ = nullptr;
  pitch_wheel_mod_ = nullptr;
}

void ExtraModSection::drawTextToRightOfComponent(Graphics& g, Component* component, String text) {
  static const int SPACE = 6;
  g.drawText(text, component->getRight() + SPACE, component->getY(),
             getWidth() - component->getRight() - component->getX(),
             component->getHeight(), Justification::centredLeft, false);
}

void ExtraModSection::paintBackground(Graphics& g) {
  static Font roboto_reg(Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                           BinaryData::RobotoRegular_ttfSize));
  SynthSection::paintBackground(g);
  
  g.setColour(Colour(0xffbbbbbb));
  g.setFont(roboto_reg.withPointHeight(10.0f));
  drawTextToRightOfComponent(g, aftertouch_mod_, TRANS("AFTERTOUCH"));
  drawTextToRightOfComponent(g, note_mod_, TRANS("NOTE"));
  drawTextToRightOfComponent(g, velocity_mod_, TRANS("VELOCITY"));
  drawTextToRightOfComponent(g, mod_wheel_mod_, TRANS("MOD WHEEL"));
  drawTextToRightOfComponent(g, pitch_wheel_mod_, TRANS("PITCH WHEEL"));
}

void ExtraModSection::resized() {
  int x = 20;
  float space = (getHeight() - 20 - (5.0f * BUTTON_WIDTH)) / 6.0f;

  aftertouch_mod_->setBounds(x, 20 + space, BUTTON_WIDTH, BUTTON_WIDTH);
  note_mod_->setBounds(x, aftertouch_mod_->getBottom() + space, BUTTON_WIDTH, BUTTON_WIDTH);
  velocity_mod_->setBounds(x, note_mod_->getBottom() + space, BUTTON_WIDTH, BUTTON_WIDTH);
  mod_wheel_mod_->setBounds(x, velocity_mod_->getBottom() + space, BUTTON_WIDTH, BUTTON_WIDTH);
  pitch_wheel_mod_->setBounds(x, mod_wheel_mod_->getBottom() + space, BUTTON_WIDTH, BUTTON_WIDTH);

  SynthSection::resized();
}
