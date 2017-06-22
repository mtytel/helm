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

#include "fixed_point_wave.h"

namespace mopo {

  FixedPointWaveLookup::FixedPointWaveLookup() {
    preprocessSin();
    preprocessTriangle();
    preprocessSquare();
    preprocessUpSaw();
    preprocessDownSaw();
    preprocessStep<3>(three_step_);
    preprocessStep<4>(four_step_);
    preprocessStep<8>(eight_step_);
    preprocessPyramid<3>(three_pyramid_);
    preprocessPyramid<5>(five_pyramid_);
    preprocessPyramid<9>(nine_pyramid_);

    wave_type waves[kNumFixedPointWaveforms] =
        { sin_, triangle_, square_, down_saw_, up_saw_,
          three_step_, four_step_, eight_step_,
          three_pyramid_, five_pyramid_, nine_pyramid_ };

    memcpy(waves_, waves, kNumFixedPointWaveforms * sizeof(wave_type));
  }

  void FixedPointWaveLookup::preprocessSin() {
    for (int h = 0; h < HARMONICS + 1; ++h) {
      for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i)
        sin_[h][i] = sin((2 * PI * i) / FIXED_LOOKUP_SIZE);
    }

    preprocessDiffs(sin_);
  }

  void FixedPointWaveLookup::preprocessTriangle() {
    for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
      triangle_[0][i] = Wave::triangle((1.0 * i) / FIXED_LOOKUP_SIZE);

      int p = i;
      mopo_float scale = 8.0 / (PI * PI);
      triangle_[HARMONICS][i] = scale * sin_[0][p];

      for (int h = 1; h < HARMONICS; ++h) {
        p = (p + i) % FIXED_LOOKUP_SIZE;
        triangle_[HARMONICS - h][i] = triangle_[HARMONICS - h + 1][i];
        mopo_float harmonic = scale * sin_[0][p] / ((h + 1) * (h + 1));

        if (h % 4 == 0)
          triangle_[HARMONICS - h][i] += harmonic;
        else if (h % 2 == 0)
          triangle_[HARMONICS - h][i] -= harmonic;
      }
    }

    preprocessDiffs(triangle_);
  }

  void FixedPointWaveLookup::preprocessSquare() {
    for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
      square_[0][i] = Wave::square((1.0 * i) / FIXED_LOOKUP_SIZE);

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

    preprocessDiffs(square_);
  }

  void FixedPointWaveLookup::preprocessDownSaw() {
    for (int h = 0; h < HARMONICS + 1; ++h) {
      for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i)
        down_saw_[h][i] = -up_saw_[h][i];
    }

    preprocessDiffs(down_saw_);
  }

  void FixedPointWaveLookup::preprocessUpSaw() {
    for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
      up_saw_[0][i] = Wave::upsaw((1.0 * i) / FIXED_LOOKUP_SIZE);

      int index = (i + (FIXED_LOOKUP_SIZE / 2)) % FIXED_LOOKUP_SIZE;
      int p = i;
      mopo_float scale = 2.0 / PI;
      up_saw_[HARMONICS][index] = scale * sin_[0][p];

      for (int h = 1; h < HARMONICS; ++h) {
        p = (p + i) % FIXED_LOOKUP_SIZE;
        mopo_float harmonic = scale * sin_[0][p] / (h + 1);

        if (h % 2 == 0)
          up_saw_[HARMONICS - h][index] = up_saw_[HARMONICS - h + 1][index] + harmonic;
        else
          up_saw_[HARMONICS - h][index] = up_saw_[HARMONICS - h + 1][index] - harmonic;
      }
    }

    preprocessDiffs(up_saw_);
  }

  template<size_t steps>
  void FixedPointWaveLookup::preprocessStep(wave_type buffer) {
    static int num_steps = steps;
    static const mopo_float step_size = num_steps / (num_steps - 1.0);

    for (int h = 0; h < HARMONICS + 1; ++h) {
      int base_num_harmonics = HARMONICS + 1 - h;
      int harmony_num_harmonics = base_num_harmonics / num_steps;
      int harmony_h = HARMONICS + 1 - harmony_num_harmonics;

      for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
        buffer[h][i] = step_size * up_saw_[h][i];

        if (harmony_num_harmonics) {
          int harm_index = (num_steps * i) % FIXED_LOOKUP_SIZE;
          buffer[h][i] += step_size * down_saw_[harmony_h][harm_index] / num_steps;
        }
      }
    }

    preprocessDiffs(buffer);
  }

  template<size_t steps>
  void FixedPointWaveLookup::preprocessPyramid(wave_type buffer) {
    static const int squares = steps - 1;
    static const int offset = 3 * FIXED_LOOKUP_SIZE / 4;

    for (int h = 0; h < HARMONICS + 1; ++h) {
      for (int i = 0; i < FIXED_LOOKUP_SIZE; ++i) {
        buffer[h][i] = 0;

        for (size_t s = 0; s < squares; ++s) {
          int square_offset = (s * FIXED_LOOKUP_SIZE) / (2 * squares);
          int phase = (i + offset + square_offset) % FIXED_LOOKUP_SIZE;
          buffer[h][i] += square_[h][phase] / squares;
        }
      }
    }

    preprocessDiffs(buffer);
  }

  void FixedPointWaveLookup::preprocessDiffs(wave_type wave) {
    for (int h = 0; h < HARMONICS + 1; ++h) {
      for (int i = 0; i < FIXED_LOOKUP_SIZE - 1; ++i)
        wave[h][i + FIXED_LOOKUP_SIZE] = FRACTIONAL_MULT * (wave[h][i + 1] - wave[h][i]);

      mopo_float last_delta = wave[h][0] - wave[h][FIXED_LOOKUP_SIZE - 1];
      wave[h][2 * FIXED_LOOKUP_SIZE - 1] = FRACTIONAL_MULT * last_delta;
    }
  }

  const FixedPointWaveLookup FixedPointWave::lookup_;
} // namespace mopo
