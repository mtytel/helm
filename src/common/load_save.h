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

#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include "JuceHeader.h"

#include "helm_engine.h"

class MidiManager;
class SynthBase;

class FileSorterAscending {
public:
  FileSorterAscending() { }

  static int compareElements(File a, File b) {
    if (a.getFileName() == "Factory Presets")
      return -1;
    else if (b.getFileName() == "Factory Presets")
      return 1;

    if (a.getFileName() == "Old Factory Presets")
      return 1;
    else if (b.getFileName() == "Old Factory Presets")
      return -1;

    return a.getFullPathName().toLowerCase().compare(b.getFullPathName().toLowerCase());
  }
};

class LoadSave {
  public:
    static var stateToVar(SynthBase* synth,
                          std::map<std::string, String>& save_info,
                          const CriticalSection& critical_section);

    static void loadControls(SynthBase* synth,
                             const NamedValueSet& properties);

    static void loadModulations(SynthBase* synth,
                                const Array<var>* modulations);

    static void loadSaveState(std::map<std::string, String>& save_info,
                              const NamedValueSet& properties);

    static void initSynth(SynthBase* synth, std::map<std::string, String>& save_info);
  
    static void varToState(SynthBase* synth,
                           std::map<std::string, String>& save_info,
                           var state);

    static String getAuthor(var state);
    static String getLicense(var state);

    static File getConfigFile();
    static var getConfigVar();
    static bool isInstalled();
    static bool wasUpgraded();
    static bool shouldCheckForUpdates();
    static bool shouldAnimateWidgets();
    static float loadWindowSize();
    static String loadVersion();
    static bool shouldAskForPayment();
    static void saveVarToConfig(var config_state);
    static void saveLayoutConfig(mopo::StringLayout* layout);
    static void saveVersionConfig();
    static void saveLastAskedForMoney();
    static void saveShouldAskForMoney(bool should_ask);
    static void savePaid();
    static void saveUpdateCheckConfig(bool check_for_updates);
    static void saveAnimateWidgets(bool check_for_updates);
    static void saveWindowSize(float window_size);
    static void saveMidiMapConfig(MidiManager* midi_manager);
    static void loadConfig(MidiManager* midi_manager, mopo::StringLayout* layout = nullptr);

    static std::wstring getComputerKeyboardLayout();
    static std::pair<wchar_t, wchar_t> getComputerKeyboardOctaveControls();

    static File getFactoryBankDirectory();
    static File getBankDirectory();
    static File getUserBankDirectory();
    static File getDidPayInitiallyFile();
    static void exportBank(String bank_name);
    static void importBank();
    static int compareVersionStrings(String a, String b);

    static int getNumPatches();
    static File getPatchFile(int bank_index, int folder_index, int patch_index);
    static Array<File> getAllPatches();
    static File loadPatch(int bank_index, int folder_index, int patch_index,
                          SynthBase* synth, std::map<std::string, String>& gui_state);
    static void loadPatchFile(File file, SynthBase* synth,
                              std::map<std::string, String>& gui_state);
};

#endif  // LOAD_SAVE_H
