/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "trigger_operators.h"

namespace laf {

  TriggerCombiner::TriggerCombiner() : Processor(2, 1) { }

  void TriggerCombiner::process() {
    outputs_[0]->clearTrigger();

    if (inputs_[0]->source->triggered) {
      outputs_[0]->trigger(inputs_[0]->source->trigger_value,
                           inputs_[0]->source->trigger_offset);
    }
    else if (inputs_[1]->source->triggered) {
      outputs_[0]->trigger(inputs_[1]->source->trigger_value,
                           inputs_[1]->source->trigger_offset);
    }
  }

  LegatoFilter::LegatoFilter() : Processor(kNumInputs, kNumOutputs),
                                 last_value_(kOff) { }

  void LegatoFilter::process() {
    outputs_[kRetrigger]->clearTrigger();
    outputs_[kRemain]->clearTrigger();
    if (!inputs_[kTrigger]->source->triggered)
      return;

    if (inputs_[kTrigger]->source->trigger_value == kOn &&
        last_value_ == kOn && inputs_[kLegato]->at(0)) {
      outputs_[kRemain]->trigger(inputs_[kTrigger]->source->trigger_value,
                                 inputs_[kTrigger]->source->trigger_offset);
    }
    else {
      outputs_[kRetrigger]->trigger(inputs_[kTrigger]->source->trigger_value,
                                    inputs_[kTrigger]->source->trigger_offset);
    }
    last_value_ = inputs_[kTrigger]->source->trigger_value;
  }

  PortamentoFilter::PortamentoFilter() : Processor(kNumInputs, 1),
                                         last_value_(kOff) { }

  void PortamentoFilter::process() {
    outputs_[0]->clearTrigger();
    if (!inputs_[kTrigger]->source->triggered)
      return;

    int state = static_cast<int>(inputs_[kPortamento]->at(0));
    if (inputs_[kTrigger]->source->trigger_value == kOn) {
      if (state == kPortamentoOff || (state == kPortamentoAuto &&
                                      last_value_ != kOn)) {
        outputs_[0]->trigger(inputs_[kTrigger]->source->trigger_value,
                             inputs_[kTrigger]->source->trigger_offset);
      }
    }

    last_value_ = inputs_[kTrigger]->source->trigger_value;
  }
} // namespace laf
