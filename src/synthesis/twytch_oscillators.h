/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TWYTCH_OSCILLATORS_H
#define TWYTCH_OSCILLATORS_H

#include "mopo.h"

namespace mopo {

  // The oscillators of the synthesizer. This section of the synth is processed
  // sample by sample to allow for cross modulation.
  class TwytchOscillators : public Processor {
    public:

      enum Inputs {
        kOscillator1Waveform,
        kOscillator2Waveform,
        kOscillator1PhaseInc,
        kOscillator2PhaseInc,
        kReset,
        kCrossMod,
        kNumInputs
      };

      TwytchOscillators();

      virtual void process();
      virtual Processor* clone() const { return new TwytchOscillators(*this); }

      Output* getOscillator1Output() { return output(0); }
      Output* getOscillator2Output() { return output(1); }

      // Process one sample of the oscillators. Must be done in the correct
      // order currently.
      void tick(int i, mopo_float base_phase1, mopo_float base_phase2,
                Wave::Type waveform1, Wave::Type waveform2) {
        static double integral;
        mopo_float cross_mod = input(kCrossMod)->source->buffer[i];
        mopo_float phase_inc1 = base_phase1 * (cross_mod * oscillator2_value_ + 1.0);
        mopo_float phase_inc2 = base_phase2 * (cross_mod * oscillator1_value_ + 1.0);
        oscillator1_phase_ = modf(oscillator1_phase_ + phase_inc1, &integral);
        oscillator2_phase_ = modf(oscillator2_phase_ + phase_inc2, &integral);
        oscillator1_value_ = Wave::wave(waveform1, oscillator1_phase_);
        oscillator2_value_ = Wave::wave(waveform2, oscillator2_phase_);
        output(0)->buffer[i] = oscillator1_value_;
        output(1)->buffer[i] = oscillator2_value_;
      }

    protected:
      mopo_float oscillator1_phase_;
      mopo_float oscillator1_value_;
      mopo_float oscillator2_phase_;
      mopo_float oscillator2_value_;
  };
} // namespace mopo

#endif // TWYTCH_OSCILLATORS_H
