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

#include "operators.h"
#include "twytch_common.h"

#include <vector>

namespace mopo {
  class ProcessorRouter;

  class TwytchModule {
    public:
      TwytchModule();
      virtual ~TwytchModule() { } // Should probably delete things.

      control_map getControls() { return controls_; }

      const Processor::Output* getModulationSource(std::string name) {
        return mod_sources_[name];
      }

      Processor* getMonoModulationDestination(std::string name) {
        return mono_mod_destinations_[name];
      }

      Processor* getPolyModulationDestination(std::string name) {
        return poly_mod_destinations_[name];
      }

      output_map getModulationSources() { return mod_sources_; }

      output_map getMonoModulations() { return mono_modulation_readout_; }

      output_map getPolyModulations() { return poly_modulation_readout_; }

      virtual ProcessorRouter* getMonoRouter() = 0;
      virtual ProcessorRouter* getPolyRouter() = 0;

    protected:
      Processor* createMonoModControl(std::string name, mopo_float start_val,
                                      bool control_rate, bool smooth_value = false);
      Processor* createPolyModControl(std::string name, mopo_float start_val,
                                      bool control_rate, bool smooth_value = false);

      control_map controls_;
      output_map mod_sources_;
      input_map mono_mod_destinations_;
      input_map poly_mod_destinations_;
      output_map mono_modulation_readout_;
      output_map poly_modulation_readout_;
  };
} // namespace mopo

#endif // TWYTCH_MODULE_H
