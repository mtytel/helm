/* Copyright 2013-2015 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef HELM_OSCILLATORS_H
#define HELM_OSCILLATORS_H

#include "mopo.h"
#include "fixed_point_wave.h"

namespace mopo {

  namespace {
    const mopo_float SCALE_OUT = 0.5 / (FixedPointWaveLookup::SCALE * INT_MAX);
  }

  class HelmOscillators : public Processor {
    public:
      static const int MAX_UNISON = 15;

      enum Inputs {
        kOscillator1Waveform,
        kOscillator2Waveform,
        kOscillator1PhaseInc,
        kOscillator2PhaseInc,
        kOscillator1Amplitude,
        kOscillator2Amplitude,
        kUnisonVoices1,
        kUnisonVoices2,
        kUnisonDetune1,
        kUnisonDetune2,
        kHarmonize1,
        kHarmonize2,
        kReset,
        kCrossMod,
        kNumInputs
      };

      HelmOscillators();

      virtual void process();
      virtual Processor* clone() const { return new HelmOscillators(*this); }

      Output* getOscillator1Output() { return output(0); }
      Output* getOscillator2Output() { return output(1); }

    protected:
      void addRandomPhaseToVoices();
      void reset();
      void computeDetuneRatios(int* oscillator_phase_diffs,
                               const mopo_float* random_offsets,
                               int base_phase,
                               bool harmonize, mopo_float detune,
                               int voices);
      void prepareBuffers(int** wave_buffers,
                          const int* oscillator_phase_diffs,
                          int base_phase,
                          int waveform);

      void tickCrossMod(int phase_diff1, int phase_diff2) {
        int master_phase1 = phase_diff1 + oscillator1_phases_[0];
        int master_phase2 = phase_diff2 + oscillator2_phases_[0];
        int sin1 = FixedPointWave::wave(FixedPointWaveLookup::kSin, master_phase1);
        int sin2 = FixedPointWave::wave(FixedPointWaveLookup::kSin, master_phase2);
        oscillator1_cross_mod_ = sin1 / FixedPointWaveLookup::SCALE;
        oscillator2_cross_mod_ = sin2 / FixedPointWaveLookup::SCALE;
      }

      void tick(int i, int voices1, int voices2) {
        mopo_float cross_mod = input(kCrossMod)->source->buffer[i];
        mopo_float amp1 = input(kOscillator1Amplitude)->source->buffer[i];
        mopo_float amp2 = input(kOscillator2Amplitude)->source->buffer[i];

        int phase_diff1 = cross_mod * oscillator2_cross_mod_;
        int phase_diff2 = cross_mod * oscillator1_cross_mod_;

        int oscillator1_total = 0;
        int oscillator2_total = 0;

        // Run Voices.
        for (int v = 0; v < voices1; ++v) {
          oscillator1_phases_[v] += oscillator1_phase_diffs_[v];
          int phase = phase_diff1 + oscillator1_phases_[v];
          oscillator1_total += wave_buffers1_[v][FixedPointWave::getIndex(phase)];
        }

        tickCrossMod(phase_diff1, phase_diff2);

        for (int v = 0; v < voices2; ++v) {
          oscillator2_phases_[v] += oscillator2_phase_diffs_[v];
          int phase = phase_diff2 + oscillator2_phases_[v];
          oscillator2_total += wave_buffers2_[v][FixedPointWave::getIndex(phase)];
        }

        oscillator1_total /= ((voices1 >> 1) + 1);
        oscillator2_total /= ((voices2 >> 1) + 1);

        mopo_float mixed = amp1 * oscillator1_total + amp2 * oscillator2_total;
        output(0)->buffer[i] = SCALE_OUT * mixed;
      }

      int oscillator1_cross_mod_;
      int oscillator2_cross_mod_;

      unsigned int oscillator1_phases_[MAX_UNISON];
      unsigned int oscillator2_phases_[MAX_UNISON];
      int* wave_buffers1_[MAX_UNISON];
      int* wave_buffers2_[MAX_UNISON];
      int oscillator1_phase_diffs_[MAX_UNISON];
      int oscillator2_phase_diffs_[MAX_UNISON];
      mopo_float oscillator1_rand_offset_[MAX_UNISON];
      mopo_float oscillator2_rand_offset_[MAX_UNISON];
  };
} // namespace mopo

#endif // HELM_OSCILLATORS_H
