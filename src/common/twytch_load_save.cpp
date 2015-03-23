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

#include "twytch_load_save.h"

var TwytchLoadSave::stateToVar(mopo::TwytchEngine* synth,
                               const CriticalSection& critical_section) {
  mopo::control_map controls = synth->getControls();
  DynamicObject* state_object = new DynamicObject();

  ScopedLock lock(critical_section);
  for (auto control : controls)
    state_object->setProperty(String(control.first), control.second->value());

  std::set<mopo::ModulationConnection*> modulations = synth->getModulationConnections();
  Array<var> modulation_states;
  for (mopo::ModulationConnection* connection: modulations) {
    DynamicObject* mod_object = new DynamicObject();
    mod_object->setProperty("source", connection->source.c_str());
    mod_object->setProperty("destination", connection->destination.c_str());
    mod_object->setProperty("amount", connection->amount.value());
    modulation_states.add(mod_object);
  }

  state_object->setProperty("modulations", modulation_states);
  return state_object;
}

void TwytchLoadSave::varToState(mopo::TwytchEngine* synth,
                                const CriticalSection& critical_section,
                                var state) {
  mopo::control_map controls = synth->getControls();
  DynamicObject* object_state = state.getDynamicObject();

  ScopedLock lock(critical_section);
  NamedValueSet properties = object_state->getProperties();
  int size = properties.size();
  for (int i = 0; i < size; ++i) {
    Identifier id = properties.getName(i);
    if (id.isValid()) {
      std::string name = id.toString().toStdString();
      if (controls.count(name)) {
        mopo::mopo_float value = properties.getValueAt(i);
        controls[name]->set(value);
      }
    }
  }

  synth->clearModulations();
  Array<var>* modulations = object_state->getProperty("modulations").getArray();
  var* modulation = modulations->begin();
  for (; modulation != modulations->end(); ++modulation) {
    DynamicObject* mod = modulation->getDynamicObject();
    std::string source = mod->getProperty("source").toString().toStdString();
    std::string destination = mod->getProperty("destination").toString().toStdString();
    mopo::ModulationConnection* connection = new mopo::ModulationConnection(source, destination);
    connection->amount.set(mod->getProperty("amount"));
    synth->connectModulation(connection);
  }
}
