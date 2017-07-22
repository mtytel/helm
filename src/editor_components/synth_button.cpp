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

#include "synth_button.h"

#include "default_look_and_feel.h"
#include "full_interface.h"
#include "helm_common.h"
#include "synth_gui_interface.h"
#include "text_look_and_feel.h"

namespace {
  enum MenuIds {
    kCancel = 0,
    kArmMidiLearn,
    kClearMidiLearn
  };

  static void sliderPopupCallback(int result, SynthButton* button) {
    if (button != nullptr && result != kCancel)
      button->handlePopupResult(result);
  }
} // namespace

SynthButton::SynthButton(String name) : ToggleButton(name), active_(true),
                                        string_lookup_(nullptr), parent_(nullptr) {
  if (!mopo::Parameters::isParameter(name.toStdString()))
    return;

  setBufferedToImage(true);
}

void SynthButton::buttonStateChanged() {
  ToggleButton::buttonStateChanged();
  for (SynthButton::ButtonListener* listener : button_listeners_)
    listener->guiChanged(this);
  notifyTooltip();
}

String SynthButton::getTextFromValue(bool on) {
  int lookup = on ? 1 : 0;

  if (string_lookup_)
    return string_lookup_[lookup];

  return mopo::strings::off_on[lookup];
}

void SynthButton::setActive(bool active) {
  active_ = active;
  repaint();
}

void SynthButton::handlePopupResult(int result) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;

  SynthBase* synth = parent->getSynth();

  if (result == kArmMidiLearn)
    synth->armMidiLearn(getName().toStdString());
  else if (result == kClearMidiLearn)
    synth->clearMidiLearn(getName().toStdString());
}

void SynthButton::mouseDown(const MouseEvent& e) {
  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (parent == nullptr)
    return;
  SynthBase* synth = parent->getSynth();

  if (e.mods.isPopupMenu()) {
    PopupMenu m;
    m.setLookAndFeel(DefaultLookAndFeel::instance());

    m.addItem(kArmMidiLearn, "Learn MIDI Assignment");
    if (parent->getSynth()->isMidiMapped(getName().toStdString()))
      m.addItem(kClearMidiLearn, "Clear MIDI Assignment");

    m.showMenuAsync(PopupMenu::Options(),
                    ModalCallbackFunction::forComponent(sliderPopupCallback, this));
  }
  else {
    ToggleButton::mouseDown(e);

    if (parent)
      synth->beginChangeGesture(getName().toStdString());
  }
}

void SynthButton::mouseUp(const MouseEvent& e) {
  if (!e.mods.isPopupMenu()) {
    ToggleButton::mouseUp(e);

    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    if (parent)
      parent->getSynth()->endChangeGesture(getName().toStdString());
  }
}

void SynthButton::mouseEnter(const MouseEvent &e) {
  ToggleButton::mouseEnter(e);
  notifyTooltip();
}

void SynthButton::addButtonListener(SynthButton::ButtonListener* listener) {
  button_listeners_.push_back(listener);
}

void SynthButton::notifyTooltip() {
  if (!parent_)
    parent_ = findParentComponentOfClass<FullInterface>();
  if (parent_) {
    std::string name = getName().toStdString();
    if (mopo::Parameters::isParameter(name))
      name = mopo::Parameters::getDetails(name).display_name;

    parent_->setToolTipText(name, getTextFromValue(getToggleState()));
  }
}
