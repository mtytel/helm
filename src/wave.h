/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef WAVE_H
#define WAVE_H

#include "laf.h"
#include <math.h>
#include <stdlib.h>

#define LOOKUP_SIZE 2048
#define TRANSITION_OCTAVE 300
#define HIGH_FREQUENCY 20000
#define MAX_HARMONICS 100

namespace laf {

  class WaveLookup {
    public:
      WaveLookup() {
        // Sin lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i)
          sin_[i] = sin((2 * PI * i) / LOOKUP_SIZE);

        // Square lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i) {
          square_[0][i] = 4.0f * sin_[i] / PI;

          int p = (i + i) % LOOKUP_SIZE;
          for (int h = 1; h < MAX_HARMONICS; ++h) {
            if (h % 2 == 0) {
              square_[h][i] = square_[h - 1][i] +
                              4.0f * sin_[p] / (PI * (h + 1));
            }
            else
              square_[h][i] = square_[h - 1][i];

            p = (p + i) % LOOKUP_SIZE;
          }
        }

        // Saw lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i) {
          saw_[0][i] = 2.0f / PI * sin_[i];

          int p = (i + i) % LOOKUP_SIZE;
          for (int h = 1; h < MAX_HARMONICS; ++h) {
            if (h % 2 == 0)
              saw_[h][i] = saw_[h - 1][i] + 2.0f * sin_[p] / (PI * (h + 1));
            else
              saw_[h][i] = saw_[h - 1][i] - 2.0f * sin_[p] / (PI * (h + 1));
            p = (p + i) % LOOKUP_SIZE;
          }
        }
      }

      inline laf_float fullsin(laf_float t) const {
        int index = t * LOOKUP_SIZE;
        laf_float fractional = t * LOOKUP_SIZE - index;
        return INTERPOLATE(sin_[index], sin_[index + 1], fractional);
      }

      inline laf_float square(laf_float t, int harmonics) const {
        int index = t * LOOKUP_SIZE;
        laf_float fractional = t * LOOKUP_SIZE - index;
        return INTERPOLATE(square_[harmonics][index],
                           square_[harmonics][index + 1], fractional);
      }

      inline laf_float upsaw(laf_float t, int harmonics) const {
        double integral;
        laf_float normalized = modf(t + 0.5, &integral);
        int index = normalized * LOOKUP_SIZE;
        laf_float fractional = normalized * LOOKUP_SIZE - index;
        return INTERPOLATE(saw_[harmonics][index],
                           saw_[harmonics][index + 1], fractional);
      }

      inline laf_float downsaw(laf_float t, int harmonics) const {
        return -upsaw(t, harmonics);
      }

      inline laf_float step(laf_float t, int steps, int harmonics) const {
        return (1.0 * steps) / (steps - 1) * (upsaw(t, harmonics) +
               downsaw(steps * t, harmonics / steps) / steps);
      }

      inline laf_float pyramid(laf_float t, int steps, int harmonics) const {
        laf_float out = 0.0;

        unsigned int squares = steps - 1;
        for (unsigned int i = 0; i < squares; ++i)
          out += square(0.5f + t + i / (2.0f * squares), harmonics) / squares;
        return out;
      }

    private:
      // Make them 1 larger for wrapping.
      laf_float sin_[LOOKUP_SIZE + 1];
      laf_float square_[MAX_HARMONICS][LOOKUP_SIZE + 1];
      laf_float saw_[MAX_HARMONICS][LOOKUP_SIZE + 1];
  };

  class Wave {
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
        kNumWaveforms
      };

      static inline laf_float blwave(Type waveform, laf_float t,
                                     laf_float frequency) {
        if (fabs(frequency) < 1)
          return wave(waveform, t);
        int harmonics = HIGH_FREQUENCY / fabs(frequency) - 1;
        if (harmonics >= MAX_HARMONICS)
          return wave(waveform, t);

        switch (waveform) {
          case kSin:
            return lookup_.fullsin(t);
          case kSquare:
            return lookup_.square(t, harmonics);
          case kDownSaw:
            return lookup_.downsaw(t, harmonics);
          case kUpSaw:
            return lookup_.upsaw(t, harmonics);
          case kThreeStep:
            return lookup_.step(t, 3, harmonics);
          case kFourStep:
            return lookup_.step(t, 4, harmonics);
          case kEightStep:
            return lookup_.step(t, 8, harmonics);
          case kThreePyramid:
            return lookup_.pyramid(t, 3, harmonics);
          case kFivePyramid:
            return lookup_.pyramid(t, 5, harmonics);
          case kNinePyramid:
            return lookup_.pyramid(t, 9, harmonics);
          default:
            return wave(waveform, t);
        }
      }

      static inline laf_float wave(Type waveform, laf_float t) {
        switch (waveform) {
          case kSin:
            return fullsin(t);
          case kSquare:
            return square(t);
          case kTriangle:
            return triangle(t);
          case kDownSaw:
            return downsaw(t);
          case kUpSaw:
            return upsaw(t);
          case kThreeStep:
            return step(t, 3);
          case kFourStep:
            return step(t, 4);
          case kEightStep:
            return step(t, 8);
          case kThreePyramid:
            return pyramid(t, 3);
          case kFivePyramid:
            return pyramid(t, 5);
          case kNinePyramid:
            return pyramid(t, 9);
          case kWhiteNoise:
            return whitenoise();
          default:
            return 0;
        }
      }

      static inline laf_float nullwave() {
        return 0;
      }

      static inline laf_float whitenoise() {
        return (2.0 * rand()) / RAND_MAX - 1;
      }

      static inline laf_float fullsin(laf_float t) {
        return lookup_.fullsin(t);
      }

      static inline laf_float square(laf_float t) {
        return t < 0.5 ? 1 : -1;
      }

      static inline laf_float triangle(laf_float t) {
        double integral;
        return fabsf(2.0f - 4.0f * modf(t - 0.25f, &integral)) - 1;
      }

      static inline laf_float downsaw(laf_float t) {
        return -upsaw(t);
      }

      static inline laf_float upsaw(laf_float t) {
        return t * 2 - 1;
      }

      static inline laf_float hannwave(laf_float t) {
        return 0.5f * (1.0f - cosf(2.0f * PI * t));
      }

      static inline laf_float step(laf_float t, int steps) {
        laf_float section = (int)(steps * t);
        return 2 * section / (steps - 1) - 1;
      }

      static inline laf_float pyramid(laf_float t, int steps) {
        laf_float output = 0.0f;

        int squares = steps - 1;
        for (int i = 0; i < squares; ++i)
          output += square(0.5f + t + i / (2.0f * squares)) / squares;

        return output;
      }

    protected:
      static const WaveLookup lookup_;
  };
} // namespace laf

#endif // WAVE_H
