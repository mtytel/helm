/* Copyright 2013-2016 Matt Tytel
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

SynthGuiInterface::SynthGuiInterface(SynthBase* synth) : synth_(synth) {
  gui_ = new FullInterface(synth->getControls(),
                           synth->getEngine()->getModulationSources(),
                           synth->getEngine()->getMonoModulations(),
                           synth->getEngine()->getPolyModulations(),
                           synth->getKeyboardState());
}

void SynthGuiInterface::updateFullGui() {
  gui_->setAllValues(synth_->getControls());
}

void SynthGuiInterface::updateGuiControl(const std::string& name, mopo::mopo_float value) {
  gui_->setValue(name, value, NotificationType::dontSendNotification);
}
