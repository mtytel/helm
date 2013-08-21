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

#include "delay.h"

namespace laf {

  Delay::Delay() : Processor(Delay::kNumInputs, 1) { }

  void Delay::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = tick(i);
  }

  inline laf_float Delay::tick(int i) {
    laf_float input = inputs_[kAudio]->at(i);
    laf_float wet = inputs_[kWet]->at(i);
    laf_float period = inputs_[kDelayTime]->at(i) * sample_rate_;
    laf_float feedback = inputs_[kFeedback]->at(i);

    laf_float read = memory_.get(period);
    memory_.push(input + read * feedback);
    return INTERPOLATE(input, read, wet);
  }
} // namespace laf
