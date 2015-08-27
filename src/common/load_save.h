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

#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include "JuceHeader.h"

#include "helm_engine.h"

class FileSorterAscending {
  public:
    FileSorterAscending() { }

    static int compareElements(File a, File b) {
      return a.getFullPathName().toLowerCase().compare(b.getFullPathName().toLowerCase());
    }
};

class FileSorterDescending {
  public:
    FileSorterDescending() { }

    static int compareElements(File a, File b) {
      return b.getFullPathName().toLowerCase().compare(a.getFullPathName().toLowerCase());
    }
};

class LoadSave {
  public:
    static var stateToVar(mopo::HelmEngine* synth,
                          const CriticalSection& critical_section);

    static void varToState(mopo::HelmEngine* synth,
                           const CriticalSection& critical_section,
                           var state);

    static File getSystemPatchDirectory();

    static File getUserPatchDirectory();
};

#endif  // LOAD_SAVE_H
