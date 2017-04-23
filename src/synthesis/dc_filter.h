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
#ifndef DC_FILTER_H
#define DC_FILTER_H

#include "processor.h"

#define COEFFICIENT_TO_SR_CONSTANT 25.0

namespace mopo {

  class DcFilter : public Processor {
    public:
      enum Inputs {
        kAudio,
        kReset,
        kNumInputs
      };

      DcFilter();
      virtual ~DcFilter() { }

      virtual Processor* clone() const { return new DcFilter(*this); }
      virtual void process();

      void computeCoefficients() {
        coefficient_ = 1.0 - COEFFICIENT_TO_SR_CONSTANT / getSampleRate();
      }

      void tick(int i, mopo_float* dest, const mopo_float* source) {
        mopo_float audio = source[i];
        mopo_float out = audio - past_in_ + coefficient_ * past_out_;
        past_in_ = audio;
        past_out_ = out;
        dest[i] = out;
      }

    private:
      void reset();

      // Current biquad coefficients.
      mopo_float coefficient_;

      // Past input and output values.
      mopo_float past_in_;
      mopo_float past_out_;
  };
} // namespace mopo

#endif // DC_FILTER_H
