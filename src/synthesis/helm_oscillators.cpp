/* Copyright 2013-2016 Matt Tytel
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

#include "detune_lookup.h"

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

    for (int v = 0; v < MAX_UNISON; ++v) {
      oscillator1_phases_[v] = 0;
      oscillator2_phases_[v] = 0;
      oscillator1_rand_offset_[v] = 0.0;
      oscillator2_rand_offset_[v] = 0.0;
      wave_buffers1_[v] = nullptr;
      wave_buffers2_[v] = nullptr;
      detune_diffs1_[v] = 0;
      detune_diffs2_[v] = 0;
    }

    for (int i = 0; i < MAX_BUFFER_SIZE; ++i) {
      oscillator1_phase_diffs_[i] = 0;
      oscillator2_phase_diffs_[i] = 0;
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

  void HelmOscillators::loadBasePhaseInc() {
    int samples = buffer_size_;

    int* dest1 = oscillator1_phase_diffs_;
    int* dest2 = oscillator2_phase_diffs_;

    const mopo_float* src1 = input(kOscillator1PhaseInc)->source->buffer;
    const mopo_float* src2 = input(kOscillator2PhaseInc)->source->buffer;

    #pragma clang loop vectorize(enable) interleave(enable)
    for (int i = 0; i < samples; ++i) {
      dest1[i] = UINT_MAX * src1[i];
      dest2[i] = UINT_MAX * src2[i];
    }
  }

  void HelmOscillators::computeDetuneRatios(int* detune_diffs,
                                            int oscillator_diff,
                                            const mopo_float* random_offsets,
                                            bool harmonize, mopo_float detune,
                                            int voices) {
    int harmonize_mult = harmonize ? 1 : 0;
    for (int v = 0; v < MAX_UNISON; ++v) {
      mopo_float amount = (detune * ((v + 1) / 2)) / ((voices + 1) / 2);

      if (v % 2)
        amount = -amount;

      mopo_float harmonic = harmonize_mult * v;

      mopo_float detune_ratio = harmonic + DetuneLookup::detuneLookup(amount) +
                                amount * random_offsets[v];
      detune_diffs[v] = detune_ratio * oscillator_diff - oscillator_diff;
    }
  }

  void HelmOscillators::prepareBuffers(int** wave_buffers,
                                       const int* detune_diffs,
                                       const int* oscillator_phase_diffs,
                                       int waveform) {
    for (int v = 0; v < MAX_UNISON; ++v) {
      int phase_diff = detune_diffs[v] + oscillator_phase_diffs[0];
      wave_buffers[v] = FixedPointWave::getBuffer(waveform, phase_diff);
    }
  }

  void HelmOscillators::process() {
    loadBasePhaseInc();
    int voices1 = utils::iclamp(input(kUnisonVoices1)->source->buffer[0], 1, MAX_UNISON);
    int voices2 = utils::iclamp(input(kUnisonVoices2)->source->buffer[0], 1, MAX_UNISON);
    mopo_float detune1 = input(kUnisonDetune1)->source->buffer[0];
    mopo_float detune2 = input(kUnisonDetune2)->source->buffer[0];
    mopo_float harmonize1 = input(kHarmonize1)->source->buffer[0];
    mopo_float harmonize2 = input(kHarmonize2)->source->buffer[0];

    addRandomPhaseToVoices();
    computeDetuneRatios(detune_diffs1_, oscillator1_phase_diffs_[0],
                        oscillator1_rand_offset_,
                        harmonize1, detune1, voices1);
    computeDetuneRatios(detune_diffs2_, oscillator2_phase_diffs_[0],
                        oscillator2_rand_offset_,
                        harmonize2, detune2, voices2);

    int wave1 = static_cast<int>(input(kOscillator1Waveform)->source->buffer[0] + 0.5);
    int wave2 = static_cast<int>(input(kOscillator2Waveform)->source->buffer[0] + 0.5);
    wave1 = utils::iclamp(wave1, 0, FixedPointWaveLookup::kWhiteNoise - 1);
    wave2 = utils::iclamp(wave2, 0, FixedPointWaveLookup::kWhiteNoise - 1);

    prepareBuffers(wave_buffers1_, detune_diffs1_, oscillator1_phase_diffs_, wave1);
    prepareBuffers(wave_buffers2_, detune_diffs2_, oscillator2_phase_diffs_, wave2);

    float scale1 = 1.0f / ((voices1 >> 1) + 1);
    float scale2 = 1.0f / ((voices2 >> 1) + 1);

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, voices1, voices2, scale1, scale2);

      reset();
    }
    for (; i < buffer_size_; ++i)
      tick(i, voices1, voices2, scale1, scale2);
  }
} // namespace mopo
