/* Copyright 2013-2015 Matt Tytel
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

#include "portamento_slope.h"

#include "utils.h"

#include <cmath>

namespace mopo {

  PortamentoSlope::PortamentoSlope() : Processor(kNumInputs, 1) {
    last_value_ = 0.0;
  }

  void PortamentoSlope::processBypass(int start) {
    for (int i = start; i < buffer_size_; ++i)
      output(0)->buffer[i] = input(kTarget)->at(i);
    last_value_ = output(0)->buffer[buffer_size_ - 1];
  }

  void PortamentoSlope::process() {
    int state = static_cast<int>(input(kPortamentoType)->at(0));
    int i = 0;
    int note_number = static_cast<int>(input(kNoteNumber)->source->trigger_value);

    if (state == kPortamentoOff) {
      processBypass(0);
      return;
    }
    else if (state == kPortamentoAuto && note_number <= 1 &&
             input(kTriggerJump)->source->triggered) {
      int trigger_offset = input(kTriggerJump)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i);

      last_value_ = input(kTarget)->at(i);
    }
    else if (input(kTriggerStart)->source->triggered) {
      int trigger_offset = input(kTriggerStart)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i);

      last_value_ = input(kTriggerStart)->source->trigger_value;
    }

    if (last_value_ == input(kTarget)->at(0) &&
        last_value_ == input(kTarget)->at(buffer_size_ - 1)) {
      processBypass(i);
    }
    else {
      for (; i < buffer_size_; ++i)
        tick(i);
    }
  }

  inline void PortamentoSlope::tick(int i) {
    mopo_float target = input(kTarget)->at(i);
    if (utils::closeToZero(input(kRunSeconds)->at(i)))
      last_value_ = target;

    mopo_float increment = 0.4 / (sample_rate_ * input(kRunSeconds)->at(0));
    if (target <= last_value_)
      last_value_ = utils::clamp(last_value_ - increment, target, last_value_);
    else
      last_value_ = utils::clamp(last_value_ + increment, last_value_, target);

    mopo_float movement = target - last_value_;
    movement *= fabs(movement);
    last_value_ += 0.07 * movement / (sample_rate_ * input(kRunSeconds)->at(0));
    output(0)->buffer[i] = last_value_;
  }
} // namespace mopo
