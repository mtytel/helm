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
#include "JuceHeader.h"
#include "helm_common.h"

#define LINUX_FACTORY_PATCH_DIRECTORY "/usr/share/helm/patches"
#define USER_BANK_NAME "User Patches"
#define LINUX_BANK_DIRECTORY "~/.helm/patches"

namespace {

  String createPatchLicense(String author) {
    return "Patch (c) by " + author +
           ".  This patch is licensed under a " +
           "Creative Commons Attribution 4.0 International License.  " +
           "You should have received a copy of the license along with this " +
           "work.  If not, see <http://creativecommons.org/licenses/by/4.0/>.";
  }
} // namespace

var LoadSave::stateToVar(mopo::HelmEngine* synth,
                         const CriticalSection& critical_section,
                         String author) {
  mopo::control_map controls = synth->getControls();
  DynamicObject* settings_object = new DynamicObject();

  ScopedLock lock(critical_section);
  for (auto control : controls)
    settings_object->setProperty(String(control.first), control.second->value());

  std::set<mopo::ModulationConnection*> modulations = synth->getModulationConnections();
  Array<var> modulation_states;
  for (mopo::ModulationConnection* connection: modulations) {
    DynamicObject* mod_object = new DynamicObject();
    mod_object->setProperty("source", connection->source.c_str());
    mod_object->setProperty("destination", connection->destination.c_str());
    mod_object->setProperty("amount", connection->amount.value());
    modulation_states.add(mod_object);
  }

  settings_object->setProperty("modulations", modulation_states);

  DynamicObject* state_object = new DynamicObject();
  state_object->setProperty("license", createPatchLicense("Author"));
  state_object->setProperty("synth_version", ProjectInfo::versionString);
  state_object->setProperty("author", author);
  state_object->setProperty("settings", settings_object);
  return state_object;
}

void LoadSave::loadControls(mopo::HelmEngine* synth,
                            const CriticalSection& critical_section,
                            const NamedValueSet& properties) {
  ScopedLock lock(critical_section);

  mopo::control_map controls = synth->getControls();
  for (auto control : controls) {
    String name = control.first;
    if (properties.contains(name)) {
      mopo::mopo_float value = properties[name];
      control.second->set(value);
    }
    else {
      mopo::ValueDetails details = mopo::Parameters::getDetails(name.toStdString());
      control.second->set(details.default_value);
    }
  }

  synth->clearModulations();
  Array<var>* modulations = properties["modulations"].getArray();
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

void LoadSave::varToState(mopo::HelmEngine* synth,
                          const CriticalSection& critical_section,
                          var state) {
  if (!state.isObject())
    return;

  DynamicObject* object_state = state.getDynamicObject();
  NamedValueSet properties = object_state->getProperties();

  if (properties.contains("synth_version")) {
    // If there is version specific loading, fill it in here.
    String version = properties["synth_version"];
    var settings = properties["settings"];
    DynamicObject* settings_object = settings.getDynamicObject();
    NamedValueSet settings_properties = settings_object->getProperties();
    loadControls(synth, critical_section, settings_properties);
  }
  else {
    // Version 0.4.1 and earlier.
    loadControls(synth, critical_section, properties);
  }
}

String LoadSave::getAuthor(var state) {
  if (!state.isObject())
    return "";

  DynamicObject* object_state = state.getDynamicObject();
  NamedValueSet properties = object_state->getProperties();
  if (properties.contains("author"))
    return properties["author"];
  return "";
}

File LoadSave::getFactoryBankDirectory() {
  File patch_dir = File("");
#ifdef LINUX
  patch_dir = File(LINUX_FACTORY_PATCH_DIRECTORY);
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

File LoadSave::getBankDirectory() {
  File patch_dir = File("");
#ifdef LINUX
  patch_dir = File(LINUX_BANK_DIRECTORY);
#elif defined(__APPLE__)
  File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
  patch_dir = data_dir.getChildFile(String("Audio/Presets/") + "Helm");
#elif defined(_WIN32)
  File data_dir = File::getSpecialLocation(File::globalApplicationsDirectory);
  patch_dir = data_dir.getChildFile("Helm/patches");
#endif

  if (!patch_dir.exists())
    patch_dir.createDirectory();
  return patch_dir;
}

File LoadSave::getUserBankDirectory() {
  File bank_dir = getBankDirectory();
  File folder_dir = bank_dir.getChildFile(USER_BANK_NAME);

  if (!folder_dir.exists())
    folder_dir.createDirectory();
  return folder_dir;
}
