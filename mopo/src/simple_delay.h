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
#ifndef SIMPLE_DELAY_H
#define SIMPLE_DELAY_H

#include "memory.h"
#include "processor.h"

namespace mopo {

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class SimpleDelay : public Processor {
    public:
      enum Inputs {
        kAudio,
        kSampleDelay,
        kFeedback,
        kReset,
        kNumInputs
      };

      SimpleDelay(int size);
      SimpleDelay(const SimpleDelay& other);
      virtual ~SimpleDelay();

      virtual Processor* clone() const override {
        return new SimpleDelay(*this);
      }

      virtual void process() override;

      inline void tick(int i, mopo_float* dest,
                       const mopo_float* audio,
                       const mopo_float* period,
                       const mopo_float* feedback) {
        mopo_float read = memory_->get(period[i]);
        mopo_float value = audio[i] + read * feedback[i];
        memory_->push(value);
        dest[i] = value;
        MOPO_ASSERT(std::isfinite(value));
      }

    protected:
      Memory* memory_;
  };
} // namespace mopo

#endif // SIMPLE_DELAY_H
