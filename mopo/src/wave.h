/* Copyright 2013-2014 Little IO
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
#ifndef WAVE_H
#define WAVE_H

#include "mopo.h"
#include <cmath>
#include <cstdlib>

#define LOOKUP_SIZE 2048
#define HIGH_FREQUENCY 20000
#define MAX_HARMONICS 100

namespace mopo {

  class WaveLookup {
    public:
      WaveLookup() {
        // Sin lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i)
          sin_[i] = sin((2 * PI * i) / LOOKUP_SIZE);

        // Square lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i) {
          int p = i;
          mopo_float scale = 4.0 / PI;
          square_[0][i] = scale * sin_[p];

          for (int h = 1; h < MAX_HARMONICS; ++h) {
            p = (p + i) % LOOKUP_SIZE;
            square_[h][i] = square_[h - 1][i];

            if (h % 2 == 0)
              square_[h][i] += scale * sin_[p] / (h + 1);
          }
        }

        // Saw lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i) {
          int index = (i + (LOOKUP_SIZE / 2)) % LOOKUP_SIZE;
          int p = i;
          mopo_float scale = 2.0 / PI;
          saw_[0][index] = scale * sin_[p];

          for (int h = 1; h < MAX_HARMONICS; ++h) {
            p = (p + i) % LOOKUP_SIZE;
            mopo_float harmonic = scale * sin_[p] / (h + 1);

            if (h % 2 == 0)
              saw_[h][index] = saw_[h - 1][index] + harmonic;
            else
              saw_[h][index] = saw_[h - 1][index] - harmonic;
          }
        }

        // Triangle lookup table.
        for (int i = 0; i < LOOKUP_SIZE + 1; ++i) {
          int p = i;
          mopo_float scale = 8.0 / (PI * PI);
          triangle_[0][i] = scale * sin_[p];

          for (int h = 1; h < MAX_HARMONICS; ++h) {
            p = (p + i) % LOOKUP_SIZE;
            triangle_[h][i] = triangle_[h - 1][i];
            mopo_float harmonic = scale * sin_[p] / ((h + 1) * (h + 1));

            if (h % 4 == 0)
              triangle_[h][i] += harmonic;
            else if (h % 2 == 0)
              triangle_[h][i] -= harmonic;
          }
        }
      }

      inline mopo_float fullsin(mopo_float t) const {
        double integral;
        mopo_float fractional = modf(t * LOOKUP_SIZE, &integral);
        int index = integral;
        return INTERPOLATE(sin_[index], sin_[index + 1], fractional);
      }

      inline mopo_float square(mopo_float t, int harmonics) const {
        double integral;
        mopo_float fractional = modf(t * LOOKUP_SIZE, &integral);
        int index = integral;
        return INTERPOLATE(square_[harmonics][index],
                           square_[harmonics][index + 1], fractional);
      }

      inline mopo_float upsaw(mopo_float t, int harmonics) const {
        double integral;
        mopo_float fractional = modf(t * LOOKUP_SIZE, &integral);
        int index = integral;
        return INTERPOLATE(saw_[harmonics][index],
                           saw_[harmonics][index + 1], fractional);
      }

      inline mopo_float downsaw(mopo_float t, int harmonics) const {
        return -upsaw(t, harmonics);
      }

      inline mopo_float triangle(mopo_float t, int harmonics) const {
        double integral;
        mopo_float fractional = modf(t * LOOKUP_SIZE, &integral);
        int index = integral;
        return INTERPOLATE(triangle_[harmonics][index],
                           triangle_[harmonics][index + 1], fractional);
      }

      template<size_t steps>
      inline mopo_float step(mopo_float t, int harmonics) const {
        return (1.0 * steps) / (steps - 1) * (upsaw(t, harmonics) +
               downsaw(steps * t, harmonics / steps) / steps);
      }

      template<size_t steps>
      inline mopo_float pyramid(mopo_float t, int harmonics) const {
        size_t squares = steps - 1;
        mopo_float phase_increment = 1.0 / (2.0 * squares);

        mopo_float phase = 0.5 + t;
        mopo_float out = 0.0;

        double integral;
        for (size_t i = 0; i < squares; ++i) {
          out += square(modf(phase, &integral), harmonics);
          phase += phase_increment;
        }
        out /= squares;
        return out;
      }

    private:
      // Make them 1 larger for wrapping.
      mopo_float sin_[LOOKUP_SIZE + 1];
      mopo_float square_[MAX_HARMONICS][LOOKUP_SIZE + 1];
      mopo_float saw_[MAX_HARMONICS][LOOKUP_SIZE + 1];
      mopo_float triangle_[MAX_HARMONICS][LOOKUP_SIZE + 1];
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

      static inline mopo_float blwave(Type waveform, mopo_float t,
                                      mopo_float frequency) {
        if (fabs(frequency) < 1)
          return wave(waveform, t);
        int harmonics = HIGH_FREQUENCY / fabs(frequency);
        if (harmonics >= MAX_HARMONICS)
          return wave(waveform, t);

        switch (waveform) {
          case kSin:
            return lookup_.fullsin(t);
          case kTriangle:
            return lookup_.triangle(t, harmonics);
          case kSquare:
            return lookup_.square(t, harmonics);
          case kDownSaw:
            return lookup_.downsaw(t, harmonics);
          case kUpSaw:
            return lookup_.upsaw(t, harmonics);
          case kThreeStep:
            return lookup_.step<3>(t, harmonics);
          case kFourStep:
            return lookup_.step<4>(t, harmonics);
          case kEightStep:
            return lookup_.step<8>(t, harmonics);
          case kThreePyramid:
            return lookup_.pyramid<3>(t, harmonics);
          case kFivePyramid:
            return lookup_.pyramid<5>(t, harmonics);
          case kNinePyramid:
            return lookup_.pyramid<9>(t, harmonics);
          default:
            return wave(waveform, t);
        }
      }

      static inline mopo_float wave(Type waveform, mopo_float t) {
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
            return step<3>(t);
          case kFourStep:
            return step<4>(t);
          case kEightStep:
            return step<8>(t);
          case kThreePyramid:
            return pyramid<3>(t);
          case kFivePyramid:
            return pyramid<5>(t);
          case kNinePyramid:
            return pyramid<9>(t);
          case kWhiteNoise:
            return whitenoise();
          default:
            return 0;
        }
      }

      static inline mopo_float nullwave() {
        return 0;
      }

      static inline mopo_float whitenoise() {
        return (2.0 * rand()) / RAND_MAX - 1;
      }

      static inline mopo_float fullsin(mopo_float t) {
        return lookup_.fullsin(t);
      }

      static inline mopo_float square(mopo_float t) {
        return t < 0.5 ? 1 : -1;
      }

      static inline mopo_float triangle(mopo_float t) {
        double integral;
        return fabsf(2.0f - 4.0f * modf(t + 0.75f, &integral)) - 1;
      }

      static inline mopo_float downsaw(mopo_float t) {
        return -upsaw(t);
      }

      static inline mopo_float upsaw(mopo_float t) {
        return t * 2 - 1;
      }

      static inline mopo_float hannwave(mopo_float t) {
        return 0.5f * (1.0f - cosf(2.0f * PI * t));
      }

      template<size_t steps>
      static inline mopo_float step(mopo_float t) {
        mopo_float section = (int)(steps * t);
        return 2 * section / (steps - 1) - 1;
      }

      template<size_t steps>
      static inline mopo_float pyramid(mopo_float t) {
        size_t squares = steps - 1;
        mopo_float phase_increment = 1.0 / (2.0 * squares);

        mopo_float phase = 0.5 + t;
        mopo_float out = 0.0;

        double integral;
        for (size_t i = 0; i < squares; ++i) {
          out += square(modf(phase, &integral));
          phase += phase_increment;
        }
        out /= squares;
        return out;
      }

    protected:
      static const WaveLookup lookup_;
  };
} // namespace mopo

#endif // WAVE_H
