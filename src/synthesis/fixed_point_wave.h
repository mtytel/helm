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
#ifndef FIXED_POINT_WAVE_H
#define FIXED_POINT_WAVE_H

#include "common.h"
#include "wave.h"
#include "utils.h"
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace mopo {

  class FixedPointWaveLookup {
    public:
      enum Type {
        kSin,
        kTriangle,
        kSquare,
        kDownSaw,
        kUpSaw,
        kThreeStep,
        kFourStep,
        kEightStep,
        kThreePyramid,
        kFivePyramid,
        kNinePyramid,
        kWhiteNoise,
        kNumFixedPointWaveforms
      };

      static const int FIXED_LOOKUP_BITS = 10;
      static const int FIXED_LOOKUP_SIZE = 1024;

      static const int FRACTIONAL_BITS = 22;
      static const int FRACTIONAL_SIZE = 4194304;
      constexpr static const int FRACTIONAL_MASK = FRACTIONAL_SIZE - 1;
      constexpr static const mopo_float FRACTIONAL_MULT = 1.0 / FRACTIONAL_SIZE;

      static const int HARMONICS = 63;

      typedef mopo_float (*wave_type)[2 * FIXED_LOOKUP_SIZE];

      FixedPointWaveLookup();

      void preprocessSin();
      void preprocessTriangle();
      void preprocessSquare();
      void preprocessDownSaw();
      void preprocessUpSaw();
      template<size_t steps>
      void preprocessStep(wave_type buffer);
      template<size_t steps>
      void preprocessPyramid(wave_type buffer);
      void preprocessDiffs(wave_type wave);

      mopo_float sin_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float triangle_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float square_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float down_saw_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float up_saw_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float three_step_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float four_step_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float eight_step_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float three_pyramid_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float five_pyramid_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];
      mopo_float nine_pyramid_[HARMONICS + 1][2 * FIXED_LOOKUP_SIZE];

      wave_type waves_[kNumFixedPointWaveforms];
  };

  class FixedPointWave {
    public:
      static inline mopo_float harmonicWave(int waveform, unsigned int t, int harmonic) {
        return lookup_.waves_[waveform][harmonic][getIndex(t)];
      }

      static inline mopo_float wave(int waveform, unsigned int t, int phase_inc) {
        return lookup_.waves_[waveform][getHarmonicIndex(phase_inc)][getIndex(t)];
      }

      static inline mopo_float wave(int waveform, unsigned int t) {
        unsigned int index = getIndex(t);
        return lookup_.waves_[waveform][0][index];
      }

      static inline mopo_float* getBuffer(int waveform, int phase_inc) {
        int clamped_inc = mopo::utils::iclamp(phase_inc, 1, INT_MAX);
        return lookup_.waves_[waveform][getHarmonicIndex(clamped_inc)];
      }

      static inline int getHarmonicIndex(int phase_inc) {
        int ratio = INT_MAX / phase_inc;
        return mopo::utils::iclamp(FixedPointWaveLookup::HARMONICS + 1 - ratio,
                                   0, FixedPointWaveLookup::HARMONICS - 1);
      }

      static inline mopo_float interpretWave(const mopo_float* buffer, unsigned int t) {
        int index = getIndex(t);
        mopo_float mult = getFractional(t);
        mopo_float inc = mult * buffer[index + FixedPointWaveLookup::FIXED_LOOKUP_SIZE];
        return buffer[index] + inc;
      }

      static inline unsigned int getIndex(unsigned int t) {
        return t >> FixedPointWaveLookup::FRACTIONAL_BITS;
      }

      static inline unsigned int getFractional(unsigned int t) {
        return t & FixedPointWaveLookup::FRACTIONAL_MASK;
      }

    protected:
      static const FixedPointWaveLookup lookup_;
  };
} // namespace mopo

#endif // FIXED_POINT_WAVE_H
