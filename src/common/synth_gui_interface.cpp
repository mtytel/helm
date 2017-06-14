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

#include "synth_gui_interface.h"

#include "default_look_and_feel.h"
#include "load_save.h"
#include "synth_base.h"

SynthGuiInterface::SynthGuiInterface(SynthBase* synth, bool use_gui) : synth_(synth) {
  if (use_gui) {
    gui_ = new FullInterface(synth->getControls(),
                             synth->getEngine()->getModulationSources(),
                             synth->getEngine()->getMonoModulations(),
                             synth->getEngine()->getPolyModulations(),
                             synth->getKeyboardState());
  }
}

void SynthGuiInterface::updateFullGui() {
  if (gui_ == nullptr)
    return;

  gui_->setAllValues(synth_->getControls());
  gui_->reset();
  gui_->resetModulations();
}

void SynthGuiInterface::updateGuiControl(const std::string& name, mopo::mopo_float value) {
  if (gui_ == nullptr)
    return;

  gui_->setValue(name, value, NotificationType::dontSendNotification);
}

mopo::mopo_float SynthGuiInterface::getControlValue(const std::string& name) {
  return synth_->getControls()[name]->value();
}

void SynthGuiInterface::setFocus() {
  if (gui_ == nullptr)
    return;

  gui_->setFocus();
}

void SynthGuiInterface::notifyChange() {
  if (gui_ == nullptr)
    return;

  gui_->notifyChange();
}

void SynthGuiInterface::notifyFresh() {
  if (gui_ == nullptr)
    return;

  gui_->notifyFresh();
}

void SynthGuiInterface::externalPatchLoaded(File patch) {
  if (gui_ == nullptr)
    return;

  gui_->externalPatchLoaded(patch);
}

void SynthGuiInterface::setGuiSize(int width, int height) {
  if (gui_ == nullptr)
    return;

  Rectangle<int> bounds = gui_->getBounds();
  bounds.setWidth(width);
  bounds.setHeight(height);
  bounds.setX(bounds.getX() + (bounds.getWidth() - width) / 2);
  gui_->getParentComponent()->setBounds(bounds);
}
