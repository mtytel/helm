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

#pragma once
#ifndef TWYTCH_MODULE_H
#define TWYTCH_MODULE_H

#include "mopo.h"
#include "twytch_common.h"

#include <vector>

namespace mopo {
  class ProcessorRouter;

  class TwytchModule {
    public:
      enum ControlSkewType {
        kLinear,
        kQuadratic,
        kExponential
      };

      TwytchModule();
      virtual ~TwytchModule() { } // Should probably delete things.

      // Returns a map of all controls of this module and all submodules.
      control_map getControls();

      Processor::Output* getModulationSource(std::string name);
      Processor* getModulationDestination(std::string name, bool poly);
      Processor* getMonoModulationDestination(std::string name);
      Processor* getPolyModulationDestination(std::string name);

      output_map getModulationSources();
      output_map getMonoModulations();
      output_map getPolyModulations();

      virtual ProcessorRouter* getMonoRouter() = 0;
      virtual ProcessorRouter* getPolyRouter() = 0;

    protected:
      // Creates a synthesizer control that you can modulate monophonically.
      Processor* createMonoModControl(std::string name, mopo_float start_val,
                                      bool control_rate, bool smooth_value = false,
                                      ControlSkewType skew = kLinear);

      // Creates a synthesizer control that you can modulate polyphonically and monophonically.
      Processor* createPolyModControl(std::string name, mopo_float start_val,
                                      bool control_rate, bool smooth_value = false,
                                      ControlSkewType skew = kLinear);

      // Creates a switch from free running frequencies to tempo synced frequencies.
      Processor* createTempoSyncSwitch(std::string name, Processor* frequency,
                                       Processor* bps, bool poly = false);

      void addSubmodule(TwytchModule* module) { sub_modules_.push_back(module); }

      std::vector<TwytchModule*> sub_modules_;

      control_map controls_;
      output_map mod_sources_;
      input_map mono_mod_destinations_;
      input_map poly_mod_destinations_;
      output_map mono_modulation_readout_;
      output_map poly_modulation_readout_;
  };
} // namespace mopo

#endif // TWYTCH_MODULE_H
