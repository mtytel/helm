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
#ifndef DELAY_H
#define DELAY_H

#include "memory.h"
#include "processor.h"
#include "utils.h"

namespace mopo {

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class Delay : public Processor {
    public:
      enum Inputs {
        kAudio,
        kWet,
        kSampleDelay,
        kFeedback,
        kNumInputs
      };

      Delay(int size);
      Delay(const Delay& other);
      virtual ~Delay();

      virtual Processor* clone() const override { return new Delay(*this); }
      virtual void process() override;

      inline void tick(int i, const mopo_float* audio, mopo_float* dest);

    protected:
      Memory* memory_;
      mopo_float current_feedback_;
      mopo_float current_wet_;
      mopo_float current_dry_;
      mopo_float current_period_;
  };
} // namespace mopo

#endif // DELAY_H
