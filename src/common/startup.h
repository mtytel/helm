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

#ifndef STARTUP_H
#define STARTUP_H

#include "JuceHeader.h"

#include <map>

class SynthBase;

namespace mopo {
  class StringLayout;
}

class MidiManager;

class Startup {
  public:
    static void doStartupChecks(MidiManager* midi_manager,
                                mopo::StringLayout* layout = nullptr);
    static bool isFirstStartup();
    static void storeOldFactoryPatches();
    static void copyFactoryPatches();
    static void fixPatchesFolder();
    static void updateAllPatches(SynthBase* synth,
                                 std::map<std::string, String>* gui_state,
                                 const CriticalSection& critical_section);
};

#endif  // STARTUP_H
