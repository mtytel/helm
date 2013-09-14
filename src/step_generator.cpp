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

#include "step_generator.h"

#include <math.h>

namespace laf {

  StepGenerator::StepGenerator(int max_steps) :
      Processor(kNumInputs + max_steps, 1), offset_(0.0),
      current_step_(0), num_steps_(max_steps) { }

  void StepGenerator::process() {
    num_steps_ = static_cast<int>(inputs_[kNumSteps]->at(0));

    int i = 0;
    if (inputs_[kReset]->source->triggered &&
        inputs_[kReset]->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_[kReset]->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        outputs_[0]->buffer[i] = tick(i);

      offset_ = 0.0;
      current_step_ = 0;
    }
    for (; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = tick(i);
  }

  inline laf_float StepGenerator::tick(int i) {
    laf_float frequency = inputs_[kFrequency]->at(i);

    offset_ += frequency / sample_rate_;
    double integral;
    offset_ = modf(offset_, &integral);
    current_step_ += integral;
    return inputs_[kSteps + current_step_]->at(i);
  }
} // namespace laf
