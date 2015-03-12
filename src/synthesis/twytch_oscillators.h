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
  class TwytchOscillators : public TickRouter {
    public:

      enum Inputs {
        kOscillator1Waveform,
        kOscillator2Waveform,
        kOscillator1Reset,
        kOscillator2Reset,
        kOscillator1BaseFrequency,
        kOscillator2BaseFrequency,
        kOscillator1FM,
        kOscillator2FM,
        kNumInputs
      };

      TwytchOscillators();
      TwytchOscillators(const TwytchOscillators& original) : TickRouter(original) {
        oscillator1_ = new Oscillator(*original.oscillator1_);
        oscillator2_ = new Oscillator(*original.oscillator2_);
        frequency1_ = new Multiply(*original.frequency1_);
        frequency2_ = new Multiply(*original.frequency2_);
        freq_mod1_ = new Multiply(*original.freq_mod1_);
        freq_mod2_ = new Multiply(*original.freq_mod2_);
        normalized_fm1_ = new Add(*original.normalized_fm1_);
        normalized_fm2_ = new Add(*original.normalized_fm2_);
      }

      virtual void process();
      virtual Processor* clone() const { return new TwytchOscillators(*this); }

      Output* getOscillator1Output() { return oscillator1_->output(); }
      Output* getOscillator2Output() { return oscillator2_->output(); }

      // Process one sample of the oscillators. Must be done in the correct
      // order currently.
      void tick(int i) {
        freq_mod1_->tick(i);
        normalized_fm1_->tick(i);
        frequency1_->tick(i);
        oscillator1_->tick(i);
        freq_mod2_->tick(i);
        normalized_fm2_->tick(i);
        frequency2_->tick(i);
        oscillator2_->tick(i);
      }

    protected:
      Oscillator* oscillator1_;
      Oscillator* oscillator2_;
      Multiply* frequency1_;
      Multiply* frequency2_;
      Multiply* freq_mod1_;
      Multiply* freq_mod2_;
      Add* normalized_fm1_;
      Add* normalized_fm2_;
  };
} // namespace mopo

#endif // TWYTCH_OSCILLATORS_H
