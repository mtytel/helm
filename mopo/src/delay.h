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
#ifndef DELAY_H
#define DELAY_H

#include "memory.h"
#include "processor.h"

namespace mopo {

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class Delay : public Processor {
    public:
      enum Inputs {
        kAudio,
        kWet,
        kFrequency,
        kFeedback,
        kNumInputs
      };

      Delay(int size);
      Delay(const Delay& other);
      virtual ~Delay() { }

      virtual Processor* clone() const { return new Delay(*this); }
      virtual void process();

      void tick(int i) {
        mopo_float audio = input(kAudio)->at(i);
        mopo_float wet = input(kWet)->at(i);
        mopo_float period = sample_rate_ / input(kFrequency)->at(i);
        mopo_float feedback = input(kFeedback)->at(i);

        mopo_float read = memory_->get(period);
        memory_->push((audio + read) * feedback);
        output(0)->buffer[i] = INTERPOLATE(audio, read, wet);
      }

    protected:
      Memory* memory_;
  };
} // namespace mopo

#endif // DELAY_H
