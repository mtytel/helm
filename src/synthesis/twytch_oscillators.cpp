/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "twytch_oscillators.h"

#define RAND_DECAY 0.8

namespace mopo {

  namespace {
    inline mopo_float getRandomPitchChange() {
      static const int RESOLUTION = 10000;
      static const mopo_float RAND_RATIO = 0.01;

      return (RAND_RATIO * (rand() % RESOLUTION)) / RESOLUTION - RAND_RATIO / 2.0;
    }
  } // namespace

  TwytchOscillators::TwytchOscillators() : Processor(kNumInputs, 1) {
    oscillator1_cross_mod_ = 0.0;
    oscillator2_cross_mod_ = 0.0;

    for (int i = 0; i < MAX_UNISON; ++i) {
      oscillator1_phases_[i] = 0;
      oscillator2_phases_[i] = 0;
      oscillator1_rand_offset_[i] = 0.0;
      oscillator2_rand_offset_[i] = 0.0;
      detune1_amounts_[i] = 1.0;
      detune2_amounts_[i] = 1.0;
    }
  }

  void TwytchOscillators::process() {
    int wave1 = static_cast<int>(input(kOscillator1Waveform)->source->buffer[0] + 0.5);
    int wave2 = static_cast<int>(input(kOscillator2Waveform)->source->buffer[0] + 0.5);
    int unison1 = CLAMP(input(kUnisonVoices1)->source->buffer[0], 1, MAX_UNISON);
    int unison2 = CLAMP(input(kUnisonVoices2)->source->buffer[0], 1, MAX_UNISON);
    mopo_float detune1 = input(kUnisonDetune1)->source->buffer[0];
    mopo_float detune2 = input(kUnisonDetune2)->source->buffer[0];
    mopo_float harmonize1 = input(kHarmonize1)->source->buffer[0] + 1;
    mopo_float harmonize2 = input(kHarmonize2)->source->buffer[0] + 1;

    for (int i = 0; i < MAX_UNISON; ++i) {
      oscillator1_rand_offset_[i] += getRandomPitchChange();
      oscillator2_rand_offset_[i] += getRandomPitchChange();
      oscillator1_rand_offset_[i] *= RAND_DECAY;
      oscillator2_rand_offset_[i] *= RAND_DECAY;
    }

    detune1_amounts_[0] = 1.0 + oscillator1_rand_offset_[0];
    for (int i = 0; i < unison1 / 2; ++i) {
      mopo_float amount = detune1 * (i + 1.0) / (unison1 / 2);
      mopo_float exponent = amount / mopo::CENTS_PER_OCTAVE;

      int index_up = 2 * i + 1;
      int index_down = 2 * i + 2;
      int base_up = 1.0;
      int base_down = 1.0;
      if (harmonize1) {
        base_up = index_up;
        base_down = index_down;
      }

      mopo_float rand_offset_up = oscillator1_rand_offset_[index_up];
      detune1_amounts_[index_up] = base_up + std::pow(2.0, exponent + rand_offset_up);
      mopo_float rand_offset_down = oscillator1_rand_offset_[index_down];
      detune1_amounts_[index_down] = base_down + std::pow(2.0, -exponent + rand_offset_down);
    }

    detune2_amounts_[0] = 1.0 + oscillator2_rand_offset_[0];
    for (int i = 0; i < unison2 / 2; ++i) {
      mopo_float amount = detune2 * (i + 1.0) / (unison2 / 2);
      mopo_float exponent = amount / mopo::CENTS_PER_OCTAVE;

      int index_up = 2 * i + 1;
      int index_down = 2 * i + 2;
      int base_up = 1.0;
      int base_down = 1.0;
      if (harmonize2) {
        base_up = index_up;
        base_down = index_down;
      }

      mopo_float rand_offset_up = oscillator2_rand_offset_[index_up];
      detune2_amounts_[index_up] = base_up + std::pow(2.0, exponent + rand_offset_up);
      mopo_float rand_offset_down = oscillator2_rand_offset_[index_down];
      detune2_amounts_[index_down] = base_down + std::pow(2.0, -exponent + rand_offset_down);
    }

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, wave1, wave2, unison1, unison2);

      oscillator1_cross_mod_ = 0.0;
      oscillator2_cross_mod_ = 0.0;

      for (int i = 0; i < MAX_UNISON; ++i) {
        oscillator1_phases_[i] = 0;
        oscillator2_phases_[i] = 0;
        oscillator1_rand_offset_[i] = 0.0;
        oscillator2_rand_offset_[i] = 0.0;
      }
    }
    for (; i < buffer_size_; ++i)
      tick(i, wave1, wave2, unison1, unison2);
  }
} // namespace mopo
