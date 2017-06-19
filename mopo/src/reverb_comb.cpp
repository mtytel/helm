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

#include "reverb_comb.h"

namespace mopo {

  ReverbComb::ReverbComb(int size) : Processor(ReverbComb::kNumInputs, 1) {
    memory_ = new Memory(size);
    filtered_sample_ = 0.0;
  }

  ReverbComb::ReverbComb(const ReverbComb& other) : Processor(other) {
    this->memory_ = new Memory(*other.memory_);
    this->filtered_sample_ = 0.0;
  }

  ReverbComb::~ReverbComb() {
    delete memory_;
  }

  void ReverbComb::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));
    MOPO_ASSERT(inputMatchesBufferSize(kFeedback));
    MOPO_ASSERT(inputMatchesBufferSize(kDamping));

    mopo_float* dest = output()->buffer;
    const mopo_float* audio_buffer = input(kAudio)->source->buffer;
    int period = input(kSampleDelay)->source->buffer[0];
    const mopo_float* feedback_buffer = input(kFeedback)->source->buffer;
    const mopo_float* damping_buffer = input(kDamping)->source->buffer;

    for (int i = 0; i < buffer_size_; ++i)
      tick(i, dest, period, audio_buffer, feedback_buffer, damping_buffer);
  }
} // namespace mopo
