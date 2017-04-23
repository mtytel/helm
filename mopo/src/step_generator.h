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

#pragma once
#ifndef STEP_GENERATOR_H
#define STEP_GENERATOR_H

#include "processor.h"

#define DEFAULT_MAX_STEPS 128

namespace mopo {

  class StepGenerator : public Processor {
    public:
      enum Inputs {
        kFrequency,
        kNumSteps,
        kReset,
        kSteps,
        kNumInputs
      };

      enum Outputs {
        kValue,
        kStep,
        kNumOutputs
      };

      StepGenerator(int max_steps = DEFAULT_MAX_STEPS);

      virtual Processor* clone() const override {
        return new StepGenerator(*this);
      }

      void process() override;
      void correctToTime(mopo_float samples);

    protected:
      unsigned int max_steps_;
      mopo_float offset_;
      unsigned int current_step_;
  };
} // namespace mopo

#endif // STEP_GENERATOR_H
