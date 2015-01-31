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
#ifndef FILTER_H
#define FILTER_H

#include "processor.h"

#include <complex>

namespace mopo {

  namespace {
    const mopo::mopo_float COEFFICIENT_FILTERING = 0.99;
  } // namespace

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
        kNotch,
        kLowShelf,
        kHighShelf,
        kAllPass,
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
                               mopo_float gain) {
        static const mopo_float shelf_slope = 1.2;

        mopo_float phase_delta = 2.0 * PI * cutoff / sample_rate_;
        mopo_float real_delta = cos(phase_delta);
        mopo_float imag_delta = sin(phase_delta);

        switch(type) {
          case kLowPass: {
            mopo_float alpha = imag_delta / (2.0 * resonance);
            mopo_float norm = 1.0 + alpha;
            target_in_0_ = (1.0 - real_delta) / (2.0 * norm);
            target_in_1_ = (1.0 - real_delta) / norm;
            target_in_2_ = target_in_0_;
            target_out_1_ = -2.0 * real_delta / norm;
            target_out_2_ = (1.0 - alpha) / norm;
            break;
          }
          case kHighPass: {
            mopo_float alpha = imag_delta / (2.0 * resonance);
            mopo_float norm = 1.0 + alpha;
            target_in_0_ = (1.0 + real_delta) / (2.0 * norm);
            target_in_1_ = -(1.0 + real_delta) / norm;
            target_in_2_ = target_in_0_;
            target_out_1_ = -2.0 * real_delta / norm;
            target_out_2_ = (1.0 - alpha) / norm;
            break;
          }
          case kBandPass: {
            mopo_float alpha = imag_delta / (2.0 * resonance);
            mopo_float norm = 1.0 + alpha;
            target_in_0_ = (imag_delta / 2.0) / norm;
            target_in_1_ = 0;
            target_in_2_ = -target_in_0_;
            target_out_1_ = -2.0 * real_delta / norm;
            target_out_2_ = (1.0 - alpha) / norm;
            break;
          }
          case kNotch: {
            mopo_float alpha = imag_delta / (2.0 * resonance);
            mopo_float norm = 1.0 + alpha;
            target_in_0_ = 1.0 / norm;
            target_in_1_ = -2.0 * real_delta / norm;
            target_in_2_ = target_in_0_;
            target_out_1_ = target_in_1_;
            target_out_2_ = (1.0 - alpha) / norm;
            break;
          }
          case kLowShelf: {
            mopo_float alpha = (imag_delta / 2.0) *
                               std::sqrt((gain + 1.0 / gain) *
                                         (1.0 / shelf_slope - 1) + 2.0);
            mopo_float sq = 2 * std::sqrt(gain) * alpha;
            mopo_float norm = (gain + 1) + (gain - 1) * real_delta + sq;

            target_in_0_ = ((gain + 1) - (gain - 1) * real_delta + sq) *
                           (gain / norm);
            target_in_1_ = 2 * ((gain - 1) - (gain + 1) * real_delta) *
                           (gain / norm);
            target_in_2_ = ((gain + 1) - (gain - 1) * real_delta - sq) *
                           (gain / norm);
            target_out_1_ = -2 * ((gain - 1) + (gain + 1) * real_delta) / norm;
            target_out_2_ = ((gain + 1) + (gain - 1) * real_delta - sq) / norm;
            break;
          }
          case kHighShelf: {
            mopo_float alpha = (imag_delta / 2.0) *
                               std::sqrt((gain + 1.0 / gain) *
                                         (1.0 / shelf_slope - 1) + 2.0);
            mopo_float sq = 2 * std::sqrt(gain) * alpha;
            mopo_float norm = (gain + 1) - (gain - 1) * real_delta + sq;

            target_in_0_ = ((gain + 1) + (gain - 1) * real_delta + sq) *
                           (gain / norm);
            target_in_1_ = -2 * ((gain - 1) + (gain + 1) * real_delta) *
                           (gain / norm);
            target_in_2_ = ((gain + 1) + (gain - 1) * real_delta - sq) *
                           (gain / norm);
            target_out_1_ = 2 * ((gain - 1) - (gain + 1) * real_delta) / norm;
            target_out_2_ = ((gain + 1) - (gain - 1) * real_delta - sq) / norm;
            break;
          }
          case kAllPass: {
            mopo_float alpha = imag_delta / (2.0 * resonance);
            mopo_float norm = 1.0 + alpha;
            target_in_0_ = (1.0 - alpha) / norm;
            target_in_1_ = -2.0 * real_delta / norm;
            target_in_2_ = 1.0;
            target_out_1_ = -2.0 * real_delta / norm;
            target_out_2_ = (1.0 - alpha) / norm;
            break;
          }
          default: {
            target_in_0_ = 1.0;
            target_in_2_ = target_in_1_ = target_out_2_ = target_out_1_ = 0.0;
          }
        }

        current_cutoff_ = cutoff;
        current_resonance_ = resonance;
      }

      void tick(int i) {
        mopo_float audio = input(kAudio)->at(i);
        in_0_ = INTERPOLATE(target_in_0_, in_0_, COEFFICIENT_FILTERING);
        in_1_ = INTERPOLATE(target_in_1_, in_1_, COEFFICIENT_FILTERING);
        in_2_ = INTERPOLATE(target_in_2_, in_2_, COEFFICIENT_FILTERING);
        out_1_ = INTERPOLATE(target_out_1_, out_1_, COEFFICIENT_FILTERING);
        out_2_ = INTERPOLATE(target_out_2_, out_2_, COEFFICIENT_FILTERING);

        mopo_float out = audio * in_0_ +
                         past_in_1_ * in_1_ +
                         past_in_2_ * in_2_ -
                         past_out_1_ * out_1_ -
                         past_out_2_ * out_2_;
        past_in_2_ = past_in_1_;
        past_in_1_ = audio;
        past_out_2_ = past_out_1_;
        past_out_1_ = out;
        output(0)->buffer[i] = out;
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
