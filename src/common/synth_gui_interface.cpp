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

#include "synth_gui_interface.h"

#include "load_save.h"

void SynthGuiInterface::valueChanged(const std::string& name, mopo::mopo_float value) {
  ScopedLock lock(getCriticalSection());
  controls_[name]->set(value);
}

void SynthGuiInterface::valueChangedInternal(const std::string& name, mopo::mopo_float value) {
  valueChanged(name, value);
  setValueNotifyHost(name, value);
}

void SynthGuiInterface::valueChangedThroughMidi(const std::string& name, mopo::mopo_float value) {
  valueChangedExternal(name, value);
}

void SynthGuiInterface::patchChangedThroughMidi(File patch) {
  updateFullGui();
}

void SynthGuiInterface::valueChangedExternal(const std::string& name, mopo::mopo_float value) {
  valueChanged(name, value);
  updateGuiControl(name, value);
}

void SynthGuiInterface::changeModulationAmount(const std::string& source,
                                               const std::string& destination,
                                               mopo::mopo_float amount) {
  mopo::ModulationConnection* connection = synth_->getConnection(source, destination);
  if (connection == nullptr && amount != 0.0) {
    connection = new mopo::ModulationConnection(source, destination);
    connectModulation(connection);
  }

  ScopedLock lock(getCriticalSection());
  if (amount != 0.0)
    connection->amount.set(amount);
  else if (connection)
    disconnectModulation(connection);
}

mopo::ModulationConnection* SynthGuiInterface::getConnection(const std::string& source,
                                                             const std::string& destination) {
  ScopedLock lock(getCriticalSection());
  return synth_->getConnection(source, destination);
}

void SynthGuiInterface::connectModulation(mopo::ModulationConnection* connection) {
  ScopedLock lock(getCriticalSection());
  synth_->connectModulation(connection);
}

void SynthGuiInterface::disconnectModulation(mopo::ModulationConnection* connection) {
  ScopedLock lock(getCriticalSection());
  synth_->disconnectModulation(connection);
  delete connection;
}

std::vector<mopo::ModulationConnection*>
SynthGuiInterface::getSourceConnections(const std::string& source) {
  ScopedLock lock(getCriticalSection());
  return synth_->getSourceConnections(source);
}

std::vector<mopo::ModulationConnection*>
SynthGuiInterface::getDestinationConnections(const std::string& destination) {
  ScopedLock lock(getCriticalSection());
  return synth_->getDestinationConnections(destination);
}

int SynthGuiInterface::getNumActiveVoices() {
  ScopedTryLock lock(getCriticalSection());
  if (lock.isLocked())
    return synth_->getNumActiveVoices();
  return -1;
}

void SynthGuiInterface::lockSynth() {
  getCriticalSection().enter();
}

void SynthGuiInterface::unlockSynth() {
  getCriticalSection().exit();
}

mopo::Processor::Output* SynthGuiInterface::getModSource(const std::string& name) {
  ScopedLock lock(getCriticalSection());
  return synth_->getModulationSource(name);
}

var SynthGuiInterface::saveToVar(String author) {
  return LoadSave::stateToVar(synth_, getCriticalSection(), author);
}

void SynthGuiInterface::loadFromVar(juce::var state) {
  LoadSave::varToState(synth_, getCriticalSection(), state);
  updateFullGui();
}

void SynthGuiInterface::armMidiLearn(const std::string& name,
                                     mopo::mopo_float min, mopo::mopo_float max) {
  getMidiManager()->armMidiLearn(name, min, max);
}

void SynthGuiInterface::cancelMidiLearn() {
  getMidiManager()->cancelMidiLearn();
}

void SynthGuiInterface::clearMidiLearn(const std::string& name) {
  getMidiManager()->clearMidiLearn(name);
}

bool SynthGuiInterface::isMidiMapped(const std::string& name) {
  return getMidiManager()->isMidiMapped(name);
}
