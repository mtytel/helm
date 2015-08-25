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

#include "load_save.h"

#define LINUX_SYSTEM_PATCH_DIRECTORY "/usr/share/helm/patches"
#define LINUX_USER_PATCH_DIRECTORY "~/.helm/User Patches"

namespace {

  String createPatchLicense(String patch_name, String author) {
    return patch_name + " (c) by " + author + newLine +
           newLine +
           patch_name + " is licensed under a" + newLine +
           "Creative Commons Attribution 4.0 International License." + newLine +
           newLine +
           "You should have received a copy of the license along with this" + newLine +
           "work.  If not, see <http://creativecommons.org/licenses/by/4.0/>.";
  }
} // namespace

var LoadSave::stateToVar(mopo::HelmEngine* synth,
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

void LoadSave::varToState(mopo::HelmEngine* synth,
                          const CriticalSection& critical_section,
                          var state) {
  if (!state.isObject())
    return;

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

File LoadSave::getSystemPatchDirectory() {
  File patch_dir = File("");
#ifdef LINUX
  patch_dir = File(LINUX_SYSTEM_PATCH_DIRECTORY);
#elif defined(__APPLE__)
  File data_dir = File::getSpecialLocation(File::commonApplicationDataDirectory);
  patch_dir = data_dir.getChildFile(String("Audio/Presets/") + "Helm");
#elif defined(_WIN32)
  File data_dir = File::getSpecialLocation(File::globalApplicationsDirectory );
  patch_dir = data_dir.getChildFile("Helm/Patches");
#endif

  if (!patch_dir.exists())
    patch_dir.createDirectory();
  return patch_dir;
}

File LoadSave::getUserPatchDirectory() {
  File patch_dir = File("");
#ifdef LINUX
  patch_dir = File(LINUX_USER_PATCH_DIRECTORY);
#elif defined(__APPLE__)
  File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
  patch_dir = data_dir.getChildFile(String("Audio/Presets/") + "Helm");
#elif defined(_WIN32)
  File data_dir = File::getSpecialLocation(File::globalApplicationsDirectory );
  patch_dir = data_dir.getChildFile("Helm/UserPatches");
#endif

  if (!patch_dir.exists())
    patch_dir.createDirectory();
  return patch_dir;
}
