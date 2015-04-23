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
#include "fixed_point_wave.h"

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
        kUnisonVoices1,
        kUnisonVoices2,
        kUnisonDetune1,
        kUnisonDetune2,
        kReset,
        kCrossMod,
        kMix,
        kNumInputs
      };

      TwytchOscillators();

      virtual void process();
      virtual Processor* clone() const { return new TwytchOscillators(*this); }

      Output* getOscillator1Output() { return output(0); }
      Output* getOscillator2Output() { return output(1); }

      // Process one sample of the oscillators. Must be done in the correct
      // order currently.
      void tick(int i, int waveform1, int waveform2,
                int unison1, int unison_phase_detune1, int unison2, int unison_phase_detune2) {
        mopo_float cross_mod = input(kCrossMod)->source->buffer[i];
        mopo_float mix = input(kMix)->source->buffer[i];
        int base_phase1 = UINT_MAX * input(kOscillator1PhaseInc)->source->buffer[i];
        int base_phase2 = UINT_MAX * input(kOscillator2PhaseInc)->source->buffer[i];
        int phase_diff1 = cross_mod * oscillator2_value_;
        int phase_diff2 = cross_mod * oscillator1_value_;
        oscillator1_phase_ += base_phase1;
        oscillator2_phase_ += base_phase2;
        int phase1_total = oscillator1_phase_ + phase_diff1;
        int phase2_total = oscillator2_phase_ + phase_diff2;
        oscillator1_value_ = FixedPointWave::wave(waveform1, phase1_total, base_phase1);
        oscillator2_value_ = FixedPointWave::wave(waveform2, phase2_total, base_phase2);

        int oscillator1_total = oscillator1_value_ / unison1;
        int oscillator2_total = oscillator2_value_ / unison2;

        // Unison.
        for (int i = 1; i < unison1; ++i) {
          oscillator1_detune_offsets_[i] += i * unison_phase_detune1;
          float phase1 = phase1_total + oscillator1_detune_offsets_[i];
          oscillator1_total += FixedPointWave::wave(waveform1, phase1, base_phase1) / unison1;
        }

        for (int i = 1; i < unison2; ++i) {
          oscillator2_detune_offsets_[i] += i * unison_phase_detune2;
          float phase2 = phase2_total + oscillator2_detune_offsets_[i];
          oscillator2_total += FixedPointWave::wave(waveform2, phase2, base_phase2) / unison2;
        }

        mopo_float mixed = (1.0 - mix) * oscillator1_total + mix * oscillator2_total;
        output(0)->buffer[i] = (mixed / FixedPointWaveLookup::SCALE) / INT_MAX;
      }

    protected:
      unsigned int oscillator1_phase_;
      int oscillator1_value_;
      unsigned int oscillator2_phase_;
      int oscillator2_value_;

      unsigned int oscillator1_detune_offsets_[16];
      unsigned int oscillator2_detune_offsets_[16];
  };
} // namespace mopo

#endif // TWYTCH_OSCILLATORS_H
