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

#include "portamento_slope.h"

#include "utils.h"

#include <cmath>
#include <iostream>

namespace mopo {

  PortamentoSlope::PortamentoSlope() : Processor(kNumInputs, 1) {
    last_value_ = 0.0;
  }

  void PortamentoSlope::processBypass(int start) {
    mopo_float* dest = output(0)->buffer;
    const mopo_float* src = input(kTarget)->source->buffer;
    utils::copyBuffer(dest + start, src, (buffer_size_ - start));
    last_value_ = dest[buffer_size_ - 1];
  }

  void PortamentoSlope::processTriggers() {
    output()->clearTrigger();

    if (input(kTriggerJump)->source->triggered) {
      int offset = input(kTriggerJump)->source->trigger_offset;
      output()->trigger(input(kTarget)->at(offset), offset);
    }
    else if (input(kTriggerStart)->source->triggered) {
      float value = input(kTriggerStart)->source->trigger_value;
      output()->trigger(value, input(kTriggerStart)->source->trigger_offset);
    }
  }

  void PortamentoSlope::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kTarget));

    processTriggers();
    int state = static_cast<int>(input(kPortamentoType)->at(0));
    mopo_float run_seconds = input(kRunSeconds)->at(0);
    if (state == kPortamentoOff || utils::closeToZero(run_seconds)) {
      processBypass(0);
      return;
    }

    mopo_float increment = 0.4 / (sample_rate_ * input(kRunSeconds)->at(0));
    mopo_float decay = 0.07 / (sample_rate_ * input(kRunSeconds)->at(0));
    const mopo_float* targets = input(kTarget)->source->buffer;

    int i = 0;
    int note_number = static_cast<int>(input(kNoteNumber)->source->trigger_value);

    if (state == kPortamentoAuto && note_number <= 1 && input(kTriggerJump)->source->triggered) {
      int trigger_offset = input(kTriggerJump)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, targets[i], increment, decay);

      last_value_ = input(kTarget)->at(trigger_offset);
    }
    else if (input(kTriggerStart)->source->triggered) {
      int trigger_offset = input(kTriggerStart)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, targets[i], increment, decay);

      last_value_ = input(kTriggerStart)->source->trigger_value;
    }

    if (last_value_ == input(kTarget)->at(0) &&
        last_value_ == input(kTarget)->at(buffer_size_ - 1)) {
      processBypass(i);
    }
    else {
      for (; i < buffer_size_; ++i)
        tick(i, targets[i], increment, decay);
    }
  }

  inline void PortamentoSlope::tick(int i, mopo_float target,
                                    mopo_float increment, mopo_float decay) {
    if (target <= last_value_)
      last_value_ = utils::clamp(last_value_ - increment, target, last_value_);
    else
      last_value_ = utils::clamp(last_value_ + increment, last_value_, target);

    mopo_float movement = target - last_value_;
    movement *= fabs(movement);
    last_value_ += movement * decay;
    output()->buffer[i] = last_value_;
  }
} // namespace mopo
