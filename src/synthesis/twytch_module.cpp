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

#include "twytch_module.h"

#include "processor_router.h"
#include "smooth_value.h"

namespace mopo {

  TwytchModule::TwytchModule() { }

  Processor* TwytchModule::createMonoModControl(std::string name, mopo_float start_val,
                                                bool control_rate, bool smooth_value) {
    ProcessorRouter* mono_owner = getMonoRouter();
    Value* val = nullptr;
    if (smooth_value) {
      val = new SmoothValue(start_val);
      mono_owner->addProcessor(val);
    }
    else
      val = new Value(start_val);

    controls_[name] = val;

    VariableAdd* mono_total = new VariableAdd();
    mono_total->setControlRate(control_rate);
    mono_total->plugNext(val);
    mono_owner->addProcessor(mono_total);
    mono_mod_destinations_[name] = mono_total;
    mono_modulation_readout_[name] = mono_total->output();
    return mono_total;
  }

  Processor* TwytchModule::createPolyModControl(std::string name, mopo_float start_val,
                                                bool control_rate, bool smooth_value) {
    Processor* mono_total = createMonoModControl(name, start_val, control_rate, smooth_value);
    ProcessorRouter* poly_owner = getPolyRouter();

    VariableAdd* poly_total = new VariableAdd();
    poly_total->setControlRate(control_rate);
    poly_owner->addProcessor(poly_total);
    poly_mod_destinations_[name] = poly_total;

    Add* modulation_total = new Add();
    modulation_total->setControlRate(control_rate);
    modulation_total->plug(mono_total, 0);
    modulation_total->plug(poly_total, 1);
    poly_owner->addProcessor(modulation_total);

    poly_owner->registerOutput(poly_total->output());
    poly_modulation_readout_[name] = poly_owner->output(poly_owner->numOutputs() - 1);
    return modulation_total;
  }
} // namespace mopo
