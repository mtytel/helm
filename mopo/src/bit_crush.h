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
#ifndef BIT_CRUSH_H
#define BIT_CRUSH_H

#include "memory.h"
#include "processor.h"

namespace mopo {

  // A bit crush distortion processor with wet/dry.
  class BitCrush : public Processor {
    public:
      enum Inputs {
        kAudio,
        kWet,
        kBits,
        kNumInputs
      };

      BitCrush();

      virtual Processor* clone() const { return new BitCrush(*this); }
      virtual void process();

      void tick(int i) {
        mopo_float input = inputs_[kAudio]->at(i);
        mopo_float wet = inputs_[kWet]->at(i);

        mopo_float output = round(magnification_ * input) / magnification_;

        outputs_[0]->buffer[i] = INTERPOLATE(input, output, wet);
      }

    protected:
      mopo_float magnification_;
  };
} // namespace mopo

#endif // BIT_CRUSH_H
