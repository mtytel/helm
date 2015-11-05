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

#include "helm_oscillators.h"

#define RAND_DECAY 0.999

namespace mopo {
  namespace {
    inline mopo_float getRandomPitchChange() {
      static const int RESOLUTION = 10000;
      static const mopo_float RAND_RATIO = 0.00001;

      return (RAND_RATIO * (rand() % RESOLUTION)) / RESOLUTION - RAND_RATIO / 2.0;
    }
  } // namespace

  const mopo_float HelmOscillators::SCALE_OUT = 0.5 / (FixedPointWaveLookup::SCALE * INT_MAX);

  HelmOscillators::HelmOscillators() : Processor(kNumInputs, 1) {
    oscillator1_cross_mod_ = 0;
    oscillator2_cross_mod_ = 0;

    for (int i = 0; i < MAX_UNISON; ++i) {
      oscillator1_phases_[i] = 0;
      oscillator2_phases_[i] = 0;
      oscillator1_rand_offset_[i] = 0.0;
      oscillator2_rand_offset_[i] = 0.0;
      oscillator1_phase_diffs_[i] = 0;
      oscillator2_phase_diffs_[i] = 0;
      wave_buffers1_[i] = nullptr;
      wave_buffers2_[i] = nullptr;
    }
  }

  void HelmOscillators::addRandomPhaseToVoices() {
    for (int i = 0; i < MAX_UNISON; ++i) {
      oscillator1_rand_offset_[i] += getRandomPitchChange();
      oscillator2_rand_offset_[i] += getRandomPitchChange();
      oscillator1_rand_offset_[i] *= RAND_DECAY;
      oscillator2_rand_offset_[i] *= RAND_DECAY;
    }
  }

  void HelmOscillators::reset() {
    oscillator1_cross_mod_ = 0;
    oscillator2_cross_mod_ = 0;

    oscillator1_phases_[0] = 0;
    oscillator2_phases_[0] = 0;
    oscillator1_rand_offset_[0] = 0.0;
    oscillator2_rand_offset_[0] = 0.0;

    for (int i = 1; i < MAX_UNISON; ++i) {
      oscillator1_phases_[i] = rand();
      oscillator2_phases_[i] = rand();
      oscillator1_rand_offset_[i] = 0.0;
      oscillator2_rand_offset_[i] = 0.0;
    }
  }

  void HelmOscillators::computeDetuneRatios(int* oscillator_phase_diffs,
                                            const mopo_float* random_offsets,
                                            int base_phase,
                                            bool harmonize, mopo_float detune,
                                            int voices) {
    for (int i = 0; i < MAX_UNISON; ++i) {
      mopo_float amount = (detune * ((i + 1) / 2)) / ((voices + 1) / 2);

      mopo_float exponent = amount / mopo::CENTS_PER_OCTAVE;
      if (i % 2)
        exponent = -exponent;

      mopo_float harmonic = 0.0;
      if (harmonize)
        harmonic = i;

      mopo_float total_detune = harmonic + std::pow(2.0, exponent) + amount * random_offsets[i];
      oscillator_phase_diffs[i] = total_detune * base_phase;
    }
  }

  void HelmOscillators::prepareBuffers(int** wave_buffers,
                                       const int* oscillator_phase_diffs,
                                       int base_phase,
                                       int waveform) {
    for (int v = 0; v < MAX_UNISON; ++v) {
      wave_buffers[v] = FixedPointWave::getBuffer(waveform, oscillator_phase_diffs[v]);
    }
  }

  void HelmOscillators::process() {
    int base_phase1 = UINT_MAX * input(kOscillator1PhaseInc)->source->buffer[0];
    int base_phase2 = UINT_MAX * input(kOscillator2PhaseInc)->source->buffer[0];
    int voices1 = CLAMP(input(kUnisonVoices1)->source->buffer[0], 1, MAX_UNISON);
    int voices2 = CLAMP(input(kUnisonVoices2)->source->buffer[0], 1, MAX_UNISON);
    mopo_float detune1 = input(kUnisonDetune1)->source->buffer[0];
    mopo_float detune2 = input(kUnisonDetune2)->source->buffer[0];
    mopo_float harmonize1 = input(kHarmonize1)->source->buffer[0];
    mopo_float harmonize2 = input(kHarmonize2)->source->buffer[0];

    addRandomPhaseToVoices();
    computeDetuneRatios(oscillator1_phase_diffs_, oscillator1_rand_offset_,
                        base_phase1, harmonize1, detune1, voices1);
    computeDetuneRatios(oscillator2_phase_diffs_, oscillator2_rand_offset_,
                        base_phase2, harmonize2, detune2, voices2);

    int wave1 = static_cast<int>(input(kOscillator1Waveform)->source->buffer[0] + 0.5);
    int wave2 = static_cast<int>(input(kOscillator2Waveform)->source->buffer[0] + 0.5);
    wave1 = CLAMP(wave1, 0, FixedPointWaveLookup::kWhiteNoise - 1);
    wave2 = CLAMP(wave2, 0, FixedPointWaveLookup::kWhiteNoise - 1);

    prepareBuffers(wave_buffers1_, oscillator1_phase_diffs_, base_phase1, wave1);
    prepareBuffers(wave_buffers2_, oscillator2_phase_diffs_, base_phase2, wave2);

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, voices1, voices2);

      reset();
    }
    for (; i < buffer_size_; ++i)
      tick(i, voices1, voices2);
  }
} // namespace mopo
