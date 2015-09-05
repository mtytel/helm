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

#include "startup.h"
#include "load_save.h"
#include "JuceHeader.h"

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
    File data_dir = File::getSpecialLocation(File::globalApplicationsDirectory);
    patch_dir = data_dir.getChildFile("Helm/User Patches");
#endif
    return patch_dir;
  }
} // namespace

void Startup::doStartupChecks() {
  fixPatchesFolder();
  checkConfigFile();
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

void Startup::checkConfigFile() {

}
