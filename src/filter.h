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
#ifndef FILTER_H
#define FILTER_H

#include "processor.h"

#include <complex>

namespace mopo {

  // Implements RBJ biquad filters of different types.
  class Filter : public Processor {
    public:
      enum Inputs {
        kAudio,
        kType,
        kCutoff,
        kResonance,
        kGain,
        kReset,
        kNumInputs
      };

      enum Type {
        kLowPass,
        kHighPass,
        kBandPass,
        kLowShelf,
        kHighShelf,
        kBandShelf,
        kAllPass,
        kNotch,
        kGainedBandPass,
        kNumTypes,
      };

      Filter();
      virtual ~Filter() { }

      std::complex<mopo_float> getResponse(mopo_float frequency);

      mopo_float getAmplitudeResponse(mopo_float frequency) {
        return std::abs(getResponse(frequency));
      }

      mopo_float getPhaseResponse(mopo_float frequency) {
        return std::arg(getResponse(frequency));
      }

      virtual Processor* clone() const { return new Filter(*this); }
      virtual void process();

      void computeCoefficients(Type type,
                               mopo_float cutoff,
                               mopo_float resonance,
                               mopo_float gain);

      inline void tick(int i, mopo_float* dest, const mopo_float* audio_buffer) {
        mopo_float audio = audio_buffer[i];
        mopo_float out = audio * in_0_ +
                         past_in_1_ * in_1_ +
                         past_in_2_ * in_2_ -
                         past_out_1_ * out_1_ -
                         past_out_2_ * out_2_;
        past_in_2_ = past_in_1_;
        past_in_1_ = audio;
        past_out_2_ = past_out_1_;
        past_out_1_ = out;
        dest[i] = out;
      }

    private:
      void reset();

      Type current_type_;
      mopo_float current_cutoff_, current_resonance_;

      // Current biquad coefficients.
      mopo_float in_0_, in_1_, in_2_;
      mopo_float out_1_, out_2_;

      // Target biquad coefficients.
      mopo_float target_in_0_, target_in_1_, target_in_2_;
      mopo_float target_out_1_, target_out_2_;

      // Past input and output values.
      mopo_float past_in_1_, past_in_2_;
      mopo_float past_out_1_, past_out_2_;
  };
} // namespace mopo

#endif // FILTER_H
