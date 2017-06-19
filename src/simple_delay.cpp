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

#include "simple_delay.h"

#define MAX_CLEAR_SAMPLES 5000

namespace mopo {

  SimpleDelay::SimpleDelay(int size) : Processor(SimpleDelay::kNumInputs, 1) {
    memory_ = new Memory(size);
  }

  SimpleDelay::SimpleDelay(const SimpleDelay& other) : Processor(other) {
    this->memory_ = new Memory(*other.memory_);
  }

  SimpleDelay::~SimpleDelay() {
    delete memory_;
  }

  void SimpleDelay::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));
    MOPO_ASSERT(inputMatchesBufferSize(kFeedback));
    MOPO_ASSERT(inputMatchesBufferSize(kSampleDelay));

    mopo_float* dest = output()->buffer;
    const mopo_float* audio = input(kAudio)->source->buffer;
    const mopo_float* feedback = input(kFeedback)->source->buffer;
    if (feedback[0] == 0.0 && feedback[buffer_size_ - 1] == 0.0) {
      memcpy(dest, audio, sizeof(mopo_float) * buffer_size_);
      memory_->pushBlock(audio, buffer_size_);
      return;
    }

    const mopo_float* period = input(kSampleDelay)->source->buffer;

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;

      for (; i < trigger_offset; ++i)
        tick(i, dest, audio, period, feedback);

      int clear_samples = std::min(MAX_CLEAR_SAMPLES, ((int)period[i]) + 1);
      memory_->pushZero(clear_samples);
    }

    for (int i = 0; i < buffer_size_; ++i)
      tick(i, dest, audio, period, feedback);
  }
} // namespace mopo
