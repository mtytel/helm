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

#include "trigger_filters.h"

namespace laf {

  LegatoFilter::LegatoFilter() : Processor(kNumInputs, 1),
                                 last_value_(kOff) { }

  void LegatoFilter::process() {
    outputs_[0]->clearTrigger();
    if (!inputs_[kTrigger]->source->triggered)
      return;

    if (inputs_[kTrigger]->source->trigger_value == kOn &&
        last_value_ == kOn && inputs_[kLegato]->at(0)) {
      return;
    }

    last_value_ = inputs_[kTrigger]->source->trigger_value;
    outputs_[0]->trigger(inputs_[kTrigger]->source->trigger_value,
                         inputs_[kTrigger]->source->trigger_offset);
  }
} // namespace laf
