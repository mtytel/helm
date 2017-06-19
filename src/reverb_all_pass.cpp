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

#include "reverb_all_pass.h"

namespace mopo {

  ReverbAllPass::ReverbAllPass(int size) : Processor(ReverbAllPass::kNumInputs, 1) {
    memory_ = new Memory(size);
  }

  ReverbAllPass::ReverbAllPass(const ReverbAllPass& other) : Processor(other) {
    this->memory_ = new Memory(*other.memory_);
  }

  ReverbAllPass::~ReverbAllPass() {
    delete memory_;
  }

  void ReverbAllPass::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));
    MOPO_ASSERT(inputMatchesBufferSize(kFeedback));

    mopo_float* dest = output()->buffer;

    const mopo_float* audio_buffer = input(kAudio)->source->buffer;
    const mopo_float* feedback_buffer = input(kFeedback)->source->buffer;
    int period = input(kSampleDelay)->at(0);

    for (int i = 0; i < buffer_size_; ++i)
      tick(i, dest, period, audio_buffer, feedback_buffer);
  }
} // namespace mopo
