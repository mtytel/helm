/* Copyright 2013 Little IO
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

#include "delay.h"

namespace mopo {

  Delay::Delay() : Processor(Delay::kNumInputs, 1) { }

  void Delay::process() {
    for (int i = 0; i < buffer_size_; ++i)
      outputs_[0]->buffer[i] = tick(i);
  }

  inline mopo_float Delay::tick(int i) {
    mopo_float input = inputs_[kAudio]->at(i);
    mopo_float wet = inputs_[kWet]->at(i);
    mopo_float period = inputs_[kDelayTime]->at(i) * sample_rate_;
    mopo_float feedback = inputs_[kFeedback]->at(i);

    mopo_float read = memory_.get(period);
    memory_.push(input + read * feedback);
    return INTERPOLATE(input, read, wet);
  }
} // namespace mopo
