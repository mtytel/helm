/* Copyright 2013-2015 Matt Tytel
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

      static const int FIXED_LOOKUP_BITS = 12;
      static const int FIXED_LOOKUP_SIZE = 4096;

      static const int FRACTIONAL_BITS = 20;
      static const int FRACTIONAL_SIZE = 1048576;

      static const int HARMONIC_BITS = 7;
      static const int HARMONICS = 128;
      static const int HARMONIC_MASK = 127;

      constexpr static const mopo_float SCALE = 0.6;

      typedef int (*wave_type)[FIXED_LOOKUP_SIZE];

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

      int sin_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int triangle_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int square_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int down_saw_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int up_saw_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int three_step_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int four_step_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int eight_step_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int three_pyramid_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int five_pyramid_[HARMONICS + 1][FIXED_LOOKUP_SIZE];
      int nine_pyramid_[HARMONICS + 1][FIXED_LOOKUP_SIZE];

      wave_type waves_[kNumFixedPointWaveforms];
  };

  class FixedPointWave {
    public:
      static inline int wave(int waveform, unsigned int t, int phase_inc) {
        int ratio = INT_MAX / phase_inc;
        int less_harmonics2 = CLAMP(FixedPointWaveLookup::HARMONICS + 1 - ratio,
                                    0, FixedPointWaveLookup::HARMONICS - 1);
        unsigned int index = t >> FixedPointWaveLookup::FRACTIONAL_BITS;
        return lookup_.waves_[waveform][less_harmonics2][index];
      }

      static inline int* getBuffer(int waveform, int phase_inc) {
        int ratio = INT_MAX / phase_inc;
        int less_harmonics2 = CLAMP(FixedPointWaveLookup::HARMONICS + 1 - ratio,
                                    0, FixedPointWaveLookup::HARMONICS - 1);
        return lookup_.waves_[waveform][less_harmonics2];
      }

      static inline unsigned int getIndex(unsigned int t) {
        return t >> FixedPointWaveLookup::FRACTIONAL_BITS;
      }

    protected:
      static const FixedPointWaveLookup lookup_;
  };
} // namespace mopo

#endif // FIXED_POINT_WAVE_H
