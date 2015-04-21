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

      FixedPointWaveLookup() {
        // Straight lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
          square_[0][i] = INT_MAX * SCALE * Wave::square((1.0 * i) / FIXED_LOOKUP_SIZE);
          up_saw_[0][i] = INT_MAX * SCALE * Wave::upsaw((1.0 * i) / FIXED_LOOKUP_SIZE);
          triangle_[0][i] = INT_MAX * SCALE * Wave::triangle((1.0 * i) / FIXED_LOOKUP_SIZE);
        }

        // Sin lookup table.
        for (int h = 0; h < HARMONICS + 1; ++h) {
          for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i)
            sin_[h][i] = INT_MAX * SCALE * sin((2 * PI * i) / FIXED_LOOKUP_SIZE);
        }

        // Triangle harmonic lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
          int p = i;
          mopo_float scale = 8.0 / (PI * PI);
          triangle_[HARMONICS][i] = scale * sin_[0][p];

          for (int h = 1; h < HARMONICS; ++h) {
            p = (p + i) % FIXED_LOOKUP_SIZE;
            triangle_[HARMONICS - h][i] = triangle_[HARMONICS - h + 1][i];
            int harmonic = scale * sin_[0][p] / ((h + 1) * (h + 1));

            if (h % 4 == 0)
              triangle_[HARMONICS - h][i] += harmonic;
            else if (h % 2 == 0)
              triangle_[HARMONICS - h][i] -= harmonic;
          }
        }

        // Square harmonic lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
          int p = i;
          mopo_float scale = 4.0 / PI;
          square_[HARMONICS][i] = scale * sin_[0][p];

          for (int h = 1; h < HARMONICS; ++h) {
            p = (p + i) % FIXED_LOOKUP_SIZE;
            square_[HARMONICS - h][i] = square_[HARMONICS - h + 1][i];

            if (h % 2 == 0)
              square_[HARMONICS - h][i] += scale * sin_[0][p] / (h + 1);
          }
        }

        // Saw harmonic lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
          int index = (i + (FIXED_LOOKUP_SIZE / 2)) % FIXED_LOOKUP_SIZE;
          int p = i;
          mopo_float scale = 2.0 / PI;
          up_saw_[HARMONICS][index] = scale * sin_[0][p];

          for (int h = 1; h < HARMONICS; ++h) {
            p = (p + i) % FIXED_LOOKUP_SIZE;
            int harmonic = scale * sin_[0][p] / (h + 1);

            if (h % 2 == 0)
              up_saw_[HARMONICS - h][index] = up_saw_[HARMONICS - h + 1][index] + harmonic;
            else
              up_saw_[HARMONICS - h][index] = up_saw_[HARMONICS - h + 1][index] - harmonic;
          }
        }

        for (int h = 0; h < HARMONICS + 1; ++h) {
          for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
            down_saw_[h][i] = -up_saw_[h][i];
          }
        }

        for (int h = 0; h < HARMONICS + 1; ++h) {
          for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
              static const size_t squares = steps - 1;
        static const mopo_float phase_increment = 1.0 / (2.0 * squares);

        mopo_float phase = 0.75 + t;
        mopo_float out = 0.0;

        mopo_float integral;
        for (size_t i = 0; i < squares; ++i) {
          out += square(modf(phase, &integral));
          phase += phase_increment;
        }
        out /= squares;
        return out;

        wave_type waves[kNumFixedPointWaveforms] =
            { sin_, triangle_, square_, down_saw_, up_saw_,
              three_step_, four_step_, eight_step_,
              three_pyramid_, five_pyramid_, nine_pyramid_ };
        memcpy(waves_, waves, kNumFixedPointWaveforms * sizeof(wave_type));
      }

      typedef int (*wave_type)[FIXED_LOOKUP_SIZE];

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
