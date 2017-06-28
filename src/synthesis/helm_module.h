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

#pragma once
#ifndef HELM_MODULE_H
#define HELM_MODULE_H

#include "mopo.h"
#include "helm_common.h"

#include <vector>

namespace mopo {
  class ValueSwitch;

  class HelmModule : public virtual ProcessorRouter {
    public:
      HelmModule();
      virtual ~HelmModule() { } // Should probably delete things.

      // For initializing things that require parents that aren't set in constructor.
      virtual void init();

      // Returns a map of all controls of this module and all submodules.
      control_map getControls();

      Output* getModulationSource(std::string name);
      Processor* getModulationDestination(std::string name, bool poly);
      Processor* getMonoModulationDestination(std::string name);
      Processor* getPolyModulationDestination(std::string name);

      ValueSwitch* getModulationSwitch(std::string name, bool poly);
      ValueSwitch* getMonoModulationSwitch(std::string name);
      ValueSwitch* getPolyModulationSwitch(std::string name);
      void updateAllModulationSwitches();

      output_map& getModulationSources();
      virtual output_map& getMonoModulations();
      virtual output_map& getPolyModulations();
      virtual void correctToTime(mopo_float samples);

    protected:
      // Creates a basic linear non-scaled control.
      Value* createBaseControl(std::string name, bool smooth_value = false);

      // Creates a basic control for switching something on and off.
      ValueSwitch* createBaseSwitchControl(std::string name);

      // Creates a basic non-scaled linear control that you can modulate monophonically
      Output* createBaseModControl(std::string name, bool smooth_value = false);

      // Creates any control that you can modulate monophonically.
      Output* createMonoModControl(std::string name, bool control_rate,
                                   bool smooth_value = false);

      // Creates any control that you can modulate polyphonically and monophonically.
      Output* createPolyModControl(std::string name, bool control_rate,
                                   bool smooth_value = false);

      // Creates a switch from free running frequencies to tempo synced frequencies.
      Output* createTempoSyncSwitch(std::string name, Processor* frequency,
                                    Output* bps, bool poly = false,
                                    ValueSwitch* owner = nullptr);

      void addSubmodule(HelmModule* module) { sub_modules_.push_back(module); }

      std::vector<HelmModule*> sub_modules_;

      control_map controls_;
      output_map mod_sources_;
      input_map mono_mod_destinations_;
      input_map poly_mod_destinations_;
      output_map mono_modulation_readout_;
      output_map poly_modulation_readout_;
      std::map<std::string, ValueSwitch*> mono_modulation_switches_;
      std::map<std::string, ValueSwitch*> poly_modulation_switches_;
  };
} // namespace mopo

#endif // HELM_MODULE_H
