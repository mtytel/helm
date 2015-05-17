/* Copyright 2013-2015 Matt Tytel
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
#ifndef REVERB_COMB_H
#define REVERB_COMB_H

#include "memory.h"
#include "processor.h"

namespace mopo {

  // A comb filter with low pass filtering useful in a reverb processor.
  class ReverbComb : public Processor {
    public:
      enum Inputs {
        kAudio,
        kSampleDelay,
        kFeedback,
        kDamping,
        kNumInputs
      };

      ReverbComb(int size);
      ReverbComb(const ReverbComb& other);
      virtual ~ReverbComb() { }

      virtual Processor* clone() const { return new ReverbComb(*this); }
      virtual void process();

      void tick(int i) {
        mopo_float audio = input(kAudio)->at(i);
        mopo_float period = input(kSampleDelay)->at(i);
        mopo_float feedback = input(kFeedback)->at(i);
        mopo_float damping = input(kDamping)->at(i);

        mopo_float read = memory_->get(period);
        filtered_sample_ = INTERPOLATE(read, filtered_sample_, damping);

        mopo_float value = audio + filtered_sample_ * feedback;
        memory_->push(value);
        output(0)->buffer[i] = read;
      }

    protected:
      Memory* memory_;
      mopo_float filtered_sample_;
  };
} // namespace mopo

#endif // REVERB_COMB_H
