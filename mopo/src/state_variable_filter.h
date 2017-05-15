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
#ifndef STATE_VARIABLE_FILTER_H
#define STATE_VARIABLE_FILTER_H

#include "processor.h"
#include "utils.h"

#include <complex>

namespace mopo {

  class StateVariableFilter : public Processor {
    public:
      enum Inputs {
        kAudio,
        kType,
        kCutoff,
        kResonance,
        kGain,
        kReset,
        k24db,
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
        kNumTypes,
      };

      StateVariableFilter();
      virtual ~StateVariableFilter() { }

      virtual Processor* clone() const { return new StateVariableFilter(*this); }
      virtual void process();
      void process12db(const mopo_float* audio_buffer, mopo_float* dest);
      void process24db(const mopo_float* audio_buffer, mopo_float* dest);
      void processAllPass(const mopo_float* audio_buffer, mopo_float* dest);

      void computeCoefficients(Type type,
                               mopo_float cutoff,
                               mopo_float resonance,
                               mopo_float gain,
                               bool db24);

      inline void tick(int i, mopo_float* dest, const mopo_float* audio_buffer);
      inline void tick24db(int i, mopo_float* dest, const mopo_float* audio_buffer);

    private:
      void reset();

      mopo_float a1_, a2_, a3_;
      mopo_float m0_, m1_, m2_;
      mopo_float target_m0_, target_m1_, target_m2_;

      mopo_float ic1eq_a_, ic2eq_a_;
      mopo_float ic1eq_b_, ic2eq_b_;

      mopo_float last_out_a_, last_out_b_;
      Type last_type_;
  };
} // namespace mopo

#endif // STATE_VARIABLE_FILTER_H
