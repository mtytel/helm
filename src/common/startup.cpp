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

#include "startup.h"
#include "load_save.h"
#include "JuceHeader.h"
#include "synth_base.h"

#define OLD_LINUX_USER_BANK_DIRECTORY "~/.helm/User Patches"

namespace {
  File getOldUserPatchesDirectory() {
    File patch_dir = File("");
#ifdef LINUX
    patch_dir = File(OLD_LINUX_USER_BANK_DIRECTORY);
#elif defined(__APPLE__)
    File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
    patch_dir = data_dir.getChildFile(String("Audio/Presets/") + "Helm");
#elif defined(_WIN32)
    File data_dir = File::getSpecialLocation(File::userApplicationDataDirectory);
    patch_dir = data_dir.getChildFile("Helm/patches");
    File folder_dir = patch_dir.getChildFile("User Patches");
#endif
    return patch_dir;
  }

  bool isBankDirectory(File bank) {
    Array<File> patches;
    bank.findChildFiles(patches, File::findFiles, true, String("*.") + mopo::PATCH_EXTENSION);

    return patches.size() && patches[0].getParentDirectory().getParentDirectory() == bank;
  }
} // namespace

void Startup::doStartupChecks(MidiManager* midi_manager, mopo::StringLayout* layout) {
  if (!LoadSave::isInstalled())
    return;

  fixPatchesFolder();

  if (LoadSave::wasUpgraded()) {
    storeOldFactoryPatches();
    copyFactoryPatches();
    LoadSave::saveVersionConfig();
    LoadSave::saveLastAskedForMoney();
  }
  else
    LoadSave::loadConfig(midi_manager, layout);
}

bool Startup::isFirstStartup() {
  return !LoadSave::getConfigFile().exists();
}

void Startup::storeOldFactoryPatches() {
  String old_version = LoadSave::loadVersion();

  if (LoadSave::isInstalled() && LoadSave::compareVersionStrings(old_version, "0.9.0") < 0) {
    File bank_directory = LoadSave::getBankDirectory();
    File old_presets_dest = bank_directory.getChildFile("Old Factory Presets");

    if (old_presets_dest.exists() || old_presets_dest.createDirectory().ok()) {
      File factory_directory = bank_directory.getChildFile("Factory Presets");
      File factory_dest = old_presets_dest.getChildFile("Factory Presets");
      if (factory_dest.exists() || factory_dest.createDirectory().ok()) {
        Array<File> factory_presets;
        factory_directory.findChildFiles(factory_presets, File::findFiles, true,
                                         String("*.") + mopo::PATCH_EXTENSION);

        for (File factory_preset : factory_presets) {
          File new_dest = factory_dest.getChildFile(factory_preset.getFileName());
          factory_preset.moveFileTo(new_dest);
        }

        factory_directory.deleteRecursively();
      }

      File cris_directory = bank_directory.getChildFile("Cris Owl Alvarez");
      File cris_dest = old_presets_dest.getChildFile("Cris Owl Alvarez");
      if (cris_dest.exists() || cris_dest.createDirectory().ok()) {
        Array<File> cris_presets;
        cris_directory.findChildFiles(cris_presets, File::findFiles, true,
                                      String("*.") + mopo::PATCH_EXTENSION);

        for (File cris_preset : cris_presets) {
          File new_dest = cris_dest.getChildFile(cris_preset.getFileName());
          cris_preset.moveFileTo(new_dest);
        }

        cris_directory.deleteRecursively();
      }
    }
  }
}

void Startup::copyFactoryPatches() {
  File factory_bank_dir = LoadSave::getFactoryBankDirectory();
  File destination = LoadSave::getBankDirectory();
  Array<File> factory_banks;
  factory_bank_dir.findChildFiles(factory_banks, File::findDirectories, false);

  for (File factory_bank : factory_banks) {
    if (isBankDirectory(factory_bank)) {
      Array<File> patches;
      factory_bank.findChildFiles(patches, File::findFiles, true,
                                  String("*.") + mopo::PATCH_EXTENSION);

      for (File patch : patches) {
        String relative_path = patch.getRelativePathFrom(factory_bank_dir);
        File patch_location = destination.getChildFile(relative_path);

        if (!patch_location.getParentDirectory().exists())
          patch_location.getParentDirectory().createDirectory();
        patch.copyFileTo(patch_location);
      }
    }
  }
}

void Startup::fixPatchesFolder() {
  File bank_directory = LoadSave::getBankDirectory();
  if (!bank_directory.exists())
    bank_directory.createDirectory();

  File user_bank = LoadSave::getUserBankDirectory();
  if (!user_bank.exists())
    user_bank.createDirectory();

  // Moving old patches from v0.4.1 and earlier to new locations.
  File old_user_bank = getOldUserPatchesDirectory();
  File default_user_folder = user_bank.getChildFile("Default");
  Array<File> misplaced_files;
  old_user_bank.findChildFiles(misplaced_files, File::findFiles, false,
                               String("*.") + mopo::PATCH_EXTENSION);
  if (misplaced_files.size() && !default_user_folder.exists())
    default_user_folder.createDirectory();

  for (int i = 0; i < misplaced_files.size(); ++i) {
    File new_patch_location = default_user_folder.getChildFile(misplaced_files[i].getFileName());
    misplaced_files[i].moveFileTo(new_patch_location);
  }
}

void Startup::updateAllPatches(SynthBase* synth,
                               std::map<std::string, String>* gui_state,
                               const CriticalSection& critical_section) {
  File user_bank = LoadSave::getBankDirectory();
  Array<File> all_patches;
  user_bank.findChildFiles(all_patches, File::findFiles, true,
                           String("*.") + mopo::PATCH_EXTENSION);

  for (File patch : all_patches) {
    var parsed_json_state;
    if (JSON::parse(patch.loadFileAsString(), parsed_json_state).wasOk()) {
      LoadSave::varToState(synth, *gui_state, parsed_json_state);
      String author = LoadSave::getAuthor(parsed_json_state);
      if (author.isEmpty())
        author = patch.getParentDirectory().getParentDirectory().getFileName();

      std::map<std::string, String> gui_state;
      gui_state["author"] = author;
      gui_state["patch_name"] = patch.getFileNameWithoutExtension();
      gui_state["folder_name"] = patch.getParentDirectory().getFileName();

      var save_var = LoadSave::stateToVar(synth, gui_state, critical_section);
      patch.replaceWithText(JSON::toString(save_var));
    }
  }
}
