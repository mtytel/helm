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

#include "delay.h"

#define DEFAULT_PERIOD 100.0

namespace mopo {

  Delay::Delay(int size) : Processor(Delay::kNumInputs, 1) {
    memory_ = new Memory(size);
    current_feedback_ = 0.0;
    current_wet_ = 0.0;
    current_dry_ = 0.0;
    current_period_ = DEFAULT_PERIOD;
  }

  Delay::Delay(const Delay& other) : Processor(other) {
    this->memory_ = new Memory(*other.memory_);
    this->current_feedback_ = 0.0;
    this->current_wet_ = 0.0;
    this->current_dry_ = 0.0;
    this->current_period_ = DEFAULT_PERIOD;
  }

  Delay::~Delay() {
    delete memory_;
  }

  void Delay::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));

    const mopo_float* audio = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;

    mopo_float wet = utils::clamp(input(kWet)->at(0), 0.0, 1.0);
    mopo_float new_wet = sqrt(wet);
    mopo_float new_dry = sqrt(1.0 - wet);
    mopo_float wet_inc = (new_wet - current_wet_) / buffer_size_;
    mopo_float dry_inc = (new_dry - current_dry_) / buffer_size_;

    mopo_float new_feedback = input(kFeedback)->at(0);
    mopo_float feedback_inc = (new_feedback - current_feedback_) / buffer_size_;

    mopo_float new_period = utils::clamp(input(kSampleDelay)->at(0), 2.0, memory_->getSize() - 1.0);
    mopo_float period_inc = (new_period - current_period_) / buffer_size_;

    for (int i = 0; i < buffer_size_; ++i) {
      current_feedback_ += feedback_inc;
      current_wet_ += wet_inc;
      current_dry_ += dry_inc;
      current_period_ += period_inc;
      tick(i, audio, dest);
    }
  }

  inline void Delay::tick(int i, const mopo_float* audio, mopo_float* dest) {
    mopo_float read = memory_->get(current_period_);
    memory_->push(audio[i] + read * current_feedback_);
    dest[i] = current_dry_ * audio[i] + current_wet_ * read;
    MOPO_ASSERT(std::isfinite(dest[i]));
  }
} // namespace mopo
