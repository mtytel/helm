/* Copyright 2013-2017 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linear_slope.h"

#include "utils.h"

#include <cmath>

namespace mopo {

  LinearSlope::LinearSlope() : Processor(kNumInputs, 1) {
    last_value_ = 0.0;
  }

  void LinearSlope::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kTarget));
    MOPO_ASSERT(inputMatchesBufferSize(kRunSeconds));

    int i = 0;
    if (input(kTriggerJump)->source->triggered) {
      int trigger_offset = input(kTriggerJump)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i);

      last_value_ = input(kTarget)->at(i);
    }

    for (; i < buffer_size_; ++i)
      tick(i);
  }

  inline void LinearSlope::tick(int i) {
    mopo_float target = input(kTarget)->at(i);
    if (utils::closeToZero(input(kRunSeconds)->at(i)))
      last_value_ = input(kTarget)->at(i);

    mopo_float increment = 1.0 / (sample_rate_ * input(kRunSeconds)->at(0));
    if (target <= last_value_)
      last_value_ = utils::clamp(last_value_ - increment, target, last_value_);
    else
      last_value_ = utils::clamp(last_value_ + increment, last_value_, target);
    output(0)->buffer[i] = last_value_;
  }
} // namespace mopo
