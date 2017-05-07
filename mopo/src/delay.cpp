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

namespace mopo {

  Delay::Delay(int size) : Processor(Delay::kNumInputs, 1) {
    memory_ = new Memory(size);
    current_feedback_ = 0.0;
  }

  Delay::Delay(const Delay& other) : Processor(other) {
    this->memory_ = new Memory(*other.memory_);
    current_feedback_ = 0.0;
  }

  Delay::~Delay() {
    delete memory_;
  }

  void Delay::process() {
    mopo_float new_feedback = input(kFeedback)->at(0);
    mopo_float new_wet = input(kWet)->at(0);

    mopo_float feedback_inc = (new_feedback - current_feedback_) / buffer_size_;
    mopo_float wet_inc = (new_wet - current_wet_) / buffer_size_;
    
    for (int i = 0; i < buffer_size_; ++i) {
      current_feedback_ += feedback_inc;
      current_wet_ += wet_inc;
      tick(i);
    }
  }
} // namespace mopo
