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

  class TwytchOscillators : public Processor {
    public:
      static const int MAX_UNISON = 15;

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

      void tickCrossMod(int base_phase1, int base_phase2, int phase_diff1, int phase_diff2) {
        int master_phase1 = phase_diff1 + oscillator1_phases_[0];
        int master_phase2 = phase_diff2 + oscillator2_phases_[0];
        int sin1 = FixedPointWave::wave(FixedPointWaveLookup::kSin, master_phase1, base_phase1);
        int sin2 = FixedPointWave::wave(FixedPointWaveLookup::kSin, master_phase2, base_phase2);
        oscillator1_cross_mod_ = sin1 / FixedPointWaveLookup::SCALE;
        oscillator2_cross_mod_ = sin2 / FixedPointWaveLookup::SCALE;
      }

      void tick(int i, int waveform1, int waveform2, int unison1, int unison2) {
        mopo_float cross_mod = input(kCrossMod)->source->buffer[i];
        mopo_float mix = input(kMix)->source->buffer[i];
        int base_phase1 = UINT_MAX * input(kOscillator1PhaseInc)->source->buffer[i];
        int base_phase2 = UINT_MAX * input(kOscillator2PhaseInc)->source->buffer[i];

        int phase_diff1 = cross_mod * oscillator2_cross_mod_;
        int phase_diff2 = cross_mod * oscillator1_cross_mod_;

        int oscillator1_total = 0;
        int oscillator2_total = 0;

        // Unison.
        for (int u = 0; u < unison1; ++u) {
          int osc_phase_diff = detune1_amounts_[u] * base_phase1;
          oscillator1_phases_[u] += osc_phase_diff;
          int phase = phase_diff1 + oscillator1_phases_[u];
          oscillator1_total += FixedPointWave::wave(waveform1, phase, osc_phase_diff);
        }

        for (int u = 0; u < unison2; ++u) {
          int osc_phase_diff = detune2_amounts_[u] * base_phase2;
          oscillator2_phases_[u] += osc_phase_diff;
          int phase = phase_diff2 + oscillator2_phases_[u];
          oscillator2_total += FixedPointWave::wave(waveform2, phase, osc_phase_diff);
        }

        tickCrossMod(base_phase1, base_phase2, phase_diff1, phase_diff2);

        mopo_float mixed = (1.0 - mix) * oscillator1_total + mix * oscillator2_total;
        output(0)->buffer[i] = (mixed / FixedPointWaveLookup::SCALE) / INT_MAX;
      }

    protected:
      int oscillator1_cross_mod_;
      int oscillator2_cross_mod_;

      unsigned int oscillator1_phases_[MAX_UNISON];
      unsigned int oscillator2_phases_[MAX_UNISON];
      mopo_float detune1_amounts_[MAX_UNISON];
      mopo_float detune2_amounts_[MAX_UNISON];
      mopo_float oscillator1_rand_offset_[MAX_UNISON];
      mopo_float oscillator2_rand_offset_[MAX_UNISON];
  };
} // namespace mopo

#endif // TWYTCH_OSCILLATORS_H
