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
        kOn,
        kStyle,
        kPassBlend,
        kShelfChoice,
        kCutoff,
        kResonance,
        kGain,
        kDrive,
        kReset,
        kNumInputs
      };

      enum Styles {
        k12dB,
        k24dB,
        kShelf,
        kNumStyles
      };

      enum Shelves {
        kLowShelf,
        kBandShelf,
        kHighShelf,
        kNumShelves
      };

      StateVariableFilter();
      virtual ~StateVariableFilter() { }

      virtual Processor* clone() const { return new StateVariableFilter(*this); }
      virtual void process();
      void process12db(const mopo_float* audio_buffer, mopo_float* dest);
      void process24db(const mopo_float* audio_buffer, mopo_float* dest);
      void processAllPass(const mopo_float* audio_buffer, mopo_float* dest);

      void computePassCoefficients(mopo_float blend,
                                   mopo_float cutoff,
                                   mopo_float resonance,
                                   bool db24);

      void computeShelfCoefficients(Shelves choice,
                                    mopo_float cutoff,
                                    mopo_float gain);

      inline void tick(int i, mopo_float* dest, const mopo_float* audio_buffer);
      inline void tick24db(int i, mopo_float* dest, const mopo_float* audio_buffer);

    private:
      void reset();

      mopo_float a1_, a2_, a3_;
      mopo_float m0_, m1_, m2_;
      mopo_float target_m0_, target_m1_, target_m2_;
      mopo_float drive_, target_drive_;

      mopo_float ic1eq_a_, ic2eq_a_;
      mopo_float ic1eq_b_, ic2eq_b_;

      mopo_float last_in_, last_distort_;
      Styles last_style_;
      Shelves last_shelf_;
  };
} // namespace mopo

#endif // STATE_VARIABLE_FILTER_H
