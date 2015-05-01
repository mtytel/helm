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

#include "synth_gui_interface.h"

#include "twytch_load_save.h"

void SynthGuiInterface::valueChanged(std::string name, mopo::mopo_float value) {
  ScopedLock lock(getCriticalSection());
  controls_[name]->set(value);
}

void SynthGuiInterface::changeModulationAmount(std::string source, std::string destination,
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

mopo::ModulationConnection* SynthGuiInterface::getConnection(std::string source,
                                                             std::string destination) {
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
SynthGuiInterface::getSourceConnections(std::string source) {
  ScopedLock lock(getCriticalSection());
  return synth_->getSourceConnections(source);
}

std::vector<mopo::ModulationConnection*>
SynthGuiInterface::getDestinationConnections(std::string destination) {
  ScopedLock lock(getCriticalSection());
  return synth_->getDestinationConnections(destination);
}

int SynthGuiInterface::getNumActiveVoices() {
  ScopedLock lock(getCriticalSection());
  return synth_->getNumActiveVoices();
}

mopo::Processor::Output* SynthGuiInterface::getModSource(std::string name) {
  ScopedLock lock(getCriticalSection());
  return synth_->getModulationSource(name);
}

var SynthGuiInterface::saveToVar() {
  return TwytchLoadSave::stateToVar(synth_, getCriticalSection());
}

void SynthGuiInterface::loadFromVar(juce::var state) {
  TwytchLoadSave::varToState(synth_, getCriticalSection(), state);
  updateFullGui();
}

void SynthGuiInterface::armMidiLearn(std::string name, mopo::mopo_float min, mopo::mopo_float max) {
  getMidiManager()->armMidiLearn(name, min, max);
}

void SynthGuiInterface::cancelMidiLearn() {
  getMidiManager()->cancelMidiLearn();
}

void SynthGuiInterface::clearMidiLearn(std::string name) {
  getMidiManager()->clearMidiLearn(name);
}