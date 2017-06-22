/* Copyright 2013-2017 Matt Tytel
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

  class HelmOscillators : public Processor {
    public:
      static const int MAX_UNISON = 15;
      static const mopo_float scales[];

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
      void reset(int i);
      void loadBasePhaseInc();
      void computeDetuneRatios(int* detune_diffs,
                               int oscillator_diff,
                               bool harmonize, mopo_float detune,
                               int voices);
      void prepareBuffers(mopo_float** wave_buffers,
                          const int* detune_diffs,
                          const int* oscillator_phase_diffs,
                          int waveform);

      void processInitial();
      void processCrossMod();
      void processVoices();
      void finishVoices(int voices1, int voices2);

      inline void tickCrossMod(int i, const mopo_float cross_mod,
                               int* dest_cross_mod1, int* dest_cross_mod2,
                               unsigned int phase1, unsigned int phase2) {
        const static mopo_float mult = (1.0 / UINT_MAX);

        int master_phase1 = dest_cross_mod2[i] + phase1;
        int master_phase2 = dest_cross_mod1[i] + phase2;
        mopo_float sin1 = utils::quickerSin(mult * master_phase1);
        mopo_float sin2 = utils::quickerSin(mult * master_phase2);
        dest_cross_mod1[i + 1] = sin1 * cross_mod * INT_MAX;
        dest_cross_mod2[i + 1] = sin2 * cross_mod * INT_MAX;
      }

      inline void tickInitialVoices(int i) {
        int phase1 = oscillator2_cross_mods_[i] + oscillator1_phases_[0] + oscillator1_phase_diffs_[i];
        int phase2 = oscillator1_cross_mods_[i] + oscillator2_phases_[0] + oscillator2_phase_diffs_[i];

        oscillator1_totals_[i] += FixedPointWave::interpretWave(wave_buffers1_[0], phase1);
        oscillator2_totals_[i] += FixedPointWave::interpretWave(wave_buffers2_[0], phase2);
      }

      inline void tickVoice1(int i, int voice, const mopo_float* wave_buffer,
                             unsigned int start_phase, int detune) {
        int phase = oscillator1_cross_mods_[i] + start_phase +
                    i * detune + oscillator1_phase_diffs_[i];
        oscillator1_totals_[i] += FixedPointWave::interpretWave(wave_buffer, phase);
      }

      inline void tickVoice2(int i, int voice, const mopo_float* wave_buffer,
                             unsigned int start_phase, int detune) {
        int phase = oscillator2_cross_mods_[i] + start_phase +
                    i * detune + oscillator2_phase_diffs_[i];
        oscillator2_totals_[i] += FixedPointWave::interpretWave(wave_buffer, phase);
      }

      inline void tickOut(int i, mopo_float* dest,
                          const mopo_float* amp1, const mopo_float* amp2,
                          const mopo_float* oscillator1_totals,
                          const mopo_float* oscillator2_totals,
                          mopo_float scale1, mopo_float scale2) {
        mopo_float mixed = amp1[i] * scale1 * oscillator1_totals[i] +
                           amp2[i] * scale2 * oscillator2_totals[i];
        dest[i] = mixed;
        MOPO_ASSERT(std::isfinite(dest[i]));
      }

      int oscillator1_cross_mods_[MAX_BUFFER_SIZE + 1];
      int oscillator2_cross_mods_[MAX_BUFFER_SIZE + 1];

      mopo_float oscillator1_totals_[MAX_BUFFER_SIZE];
      mopo_float oscillator2_totals_[MAX_BUFFER_SIZE];

      unsigned int oscillator1_phase_base_;
      unsigned int oscillator2_phase_base_;
      unsigned int oscillator1_phases_[MAX_UNISON];
      unsigned int oscillator2_phases_[MAX_UNISON];

      mopo_float* wave_buffers1_[MAX_UNISON];
      mopo_float* wave_buffers2_[MAX_UNISON];
      int detune_diffs1_[MAX_UNISON];
      int detune_diffs2_[MAX_UNISON];
      int oscillator1_phase_diffs_[MAX_BUFFER_SIZE];
      int oscillator2_phase_diffs_[MAX_BUFFER_SIZE];
  };
} // namespace mopo

#endif // HELM_OSCILLATORS_H
