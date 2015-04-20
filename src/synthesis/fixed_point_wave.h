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

#define FIXED_LOOKUP_BITS 12
#define FIXED_LOOKUP_SIZE 4096

#define FRACTIONAL_BITS 20
#define FRACTIONAL_SIZE 1048576

#define HARMONIC_BITS 7
#define HARMONICS 128
#define HARMONIC_MASK 127

namespace mopo {

  class FixedPointWaveLookup {
    public:
      FixedPointWaveLookup() {
        // Straight lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
          square_[0][i] = INT_MAX * Wave::square((1.0 * i) / FIXED_LOOKUP_SIZE) / 1.5;
          saw_[0][i] = INT_MAX * Wave::upsaw((1.0 * i) / FIXED_LOOKUP_SIZE) / 1.5;
          triangle_[0][i] = INT_MAX * Wave::triangle((1.0 * i) / FIXED_LOOKUP_SIZE) / 1.5;
        }
        square_[0][FIXED_LOOKUP_SIZE] = square_[0][0];
        saw_[0][FIXED_LOOKUP_SIZE] = saw_[0][0];
        triangle_[0][FIXED_LOOKUP_SIZE] = triangle_[0][0];

        // Sin lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE + 1; ++i) {
          for (int h = 0; h < HARMONICS + 1; ++h)
            sin_[h][i] = INT_MAX * sin((2 * PI * i) / FIXED_LOOKUP_SIZE) / 1.5;
        }

        // Square harmonic lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE + 1; ++i) {
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
        for (int i = 0; i < FIXED_LOOKUP_SIZE + 1; ++i) {
          int index = (i + (FIXED_LOOKUP_SIZE / 2)) % FIXED_LOOKUP_SIZE;
          int p = i;
          mopo_float scale = 2.0 / PI;
          saw_[HARMONICS][index] = scale * sin_[0][p];

          for (int h = 1; h < HARMONICS; ++h) {
            p = (p + i) % FIXED_LOOKUP_SIZE;
            int harmonic = scale * sin_[0][p] / (h + 1);

            if (h % 2 == 0)
              saw_[HARMONICS - h][index] = saw_[HARMONICS - h + 1][index] + harmonic;
            else
              saw_[HARMONICS - h][index] = saw_[HARMONICS - h + 1][index] - harmonic;
          }
        }

        // Triangle harmonic lookup table.
        for (int i = 0; i < FIXED_LOOKUP_SIZE + 1; ++i) {
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

        waves_[0] = sin_;
        waves_[1] = triangle_;
        waves_[2] = square_;
        waves_[3] = saw_;
      }

      typedef int (*wave_type)[FIXED_LOOKUP_SIZE + 1];

      int sin_[HARMONICS + 1][FIXED_LOOKUP_SIZE + 1];
      int square_[HARMONICS + 1][FIXED_LOOKUP_SIZE + 1];
      int saw_[HARMONICS + 1][FIXED_LOOKUP_SIZE + 1];
      int triangle_[HARMONICS + 1][FIXED_LOOKUP_SIZE + 1];
      wave_type waves_[4];
  };

  class FixedPointWave {
    public:
      enum Type {
        kSin,
        kTriangle,
        kSquare,
        kDownSaw,
        kWhiteNoise,
        kNumFixedPointWaveforms
      };

      static inline int wave(int waveform, unsigned int t, int phase_inc) {
        int ratio = INT_MAX / phase_inc;
        int less_harmonics2 = CLAMP(HARMONICS + 1 - ratio, 0, HARMONICS - 1);
        int index = t >> FRACTIONAL_BITS;
        return lookup_.waves_[waveform][less_harmonics2][index];
      }

    protected:
      static const FixedPointWaveLookup lookup_;
  };
} // namespace mopo

#endif // FIXED_POINT_WAVE_H
