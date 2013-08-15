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

#include "linear_slope.h"

#include "utils.h"

#include <math.h>

namespace laf {

  LinearSlope::LinearSlope() : Processor(kNumInputs, 1) {
    last_value_ = 0.0;
  }

  void LinearSlope::process() {
    int i = 0;
    if (inputs_[kTriggerJump]->source->triggered) {
      int trigger_offset = inputs_[kTriggerJump]->source->trigger_offset;
      for (; i < trigger_offset; ++i) {
        last_value_ = tick(i);
        outputs_[0]->buffer[i] = last_value_;
      }

      last_value_ = inputs_[kTarget]->at(i);
    }

    for (; i < BUFFER_SIZE; ++i) {
      last_value_ = tick(i);
      outputs_[0]->buffer[i] = last_value_;
    }
  }

  inline laf_sample LinearSlope::tick(int i) {
    laf_sample target = inputs_[kTarget]->at(i);
    if (utils::closeToZero(inputs_[kRunSeconds]->at(i)))
      return inputs_[kTarget]->at(i);

    laf_sample increment = 1.0 / (sample_rate_ * inputs_[kRunSeconds]->at(0));
    if (target <= last_value_)
      return CLAMP(target, last_value_, last_value_ - increment);
    return CLAMP(last_value_, target, last_value_ + increment);
  }
} // namespace laf
