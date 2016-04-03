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
#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include "value.h"
#include <cmath>
#include <cstdlib>

#ifdef __SSE2__
#include <emmintrin.h>
#else
#include <algorithm>
#endif

namespace mopo {

  namespace {
    const mopo_float EPSILON = 1e-16;
    const mopo_float DB_GAIN_CONVERSION_MULT = 40.0;
    const mopo_float MIDI_0_FREQUENCY = 8.1757989156;
    const int NOTES_PER_OCTAVE = 12;
    const int CENTS_PER_NOTE = 100;
    const int CENTS_PER_OCTAVE = NOTES_PER_OCTAVE * CENTS_PER_NOTE;
    const int MAX_CENTS = MIDI_SIZE * CENTS_PER_NOTE;
    const mopo_float MAX_Q_POW = 4.0;
    const mopo_float MIN_Q_POW = -1.0;
  }

  namespace utils {

    const Value value_zero(0.0);
    const Value value_one(1.0);
    const Value value_two(2.0);
    const Value value_half(0.5);
    const Value value_pi(PI);
    const Value value_2pi(2.0 * PI);
    const Value value_neg_one(-1.0);

#ifdef __SSE2__
    inline mopo_float min(mopo_float one, mopo_float two) {
      _mm_store_sd(&one, _mm_min_sd(_mm_set_sd(one),_mm_set_sd(two)));
      return one;
    }

    inline mopo_float max(mopo_float one, mopo_float two) {
      _mm_store_sd(&one, _mm_max_sd(_mm_set_sd(one),_mm_set_sd(two)));
      return one;
    }

    inline mopo_float clamp(mopo_float value, mopo_float min, mopo_float max) {
      _mm_store_sd(&value, _mm_min_sd(_mm_max_sd(_mm_set_sd(value),
                                                 _mm_set_sd(min)),
                                                 _mm_set_sd(max)));
      return value;
    }
#else
    inline mopo_float min(mopo_float one, mopo_float two) {
      return std::min(one, two);
    }

    inline mopo_float max(mopo_float one, mopo_float two) {
      return std::max(one, two);
    }

    inline mopo_float clamp(mopo_float value, mopo_float min, mopo_float max) {
      return std::min(max, std::max(value, min));
    }
#endif

    inline mopo_float mod(mopo_float value, mopo_float* integral) {
      return modf(value, integral);
    }

    inline mopo_float iclamp(int value, int min, int max) {
      return value > max ? max : (value < min ? min : value);
    }

    inline bool closeToZero(mopo_float value) {
      return value <= EPSILON && value >= -EPSILON;
    }

    inline mopo_float gainToDb(mopo_float gain) {
      return DB_GAIN_CONVERSION_MULT * log10(gain);
    }

    inline mopo_float dbToGain(mopo_float decibals) {
      return std::pow(10.0, decibals / DB_GAIN_CONVERSION_MULT);
    }

    inline mopo_float midiCentsToFrequency(mopo_float cents) {
      return MIDI_0_FREQUENCY * pow(2.0, cents / CENTS_PER_OCTAVE);
    }

    inline mopo_float midiNoteToFrequency(mopo_float note) {
      return midiCentsToFrequency(note * CENTS_PER_NOTE);
    }

    inline mopo_float frequencyToMidiNote(mopo_float frequency) {
      return NOTES_PER_OCTAVE * log(frequency / MIDI_0_FREQUENCY) / log(2.0);
    }

    inline mopo_float frequencyToMidiCents(mopo_float frequency) {
      return CENTS_PER_NOTE * frequencyToMidiNote(frequency);
    }

    inline mopo_float magnitudeToQ(mopo_float magnitude) {
      return pow(2.0, INTERPOLATE(MIN_Q_POW, MAX_Q_POW, magnitude));
    }

    inline mopo_float qToMagnitude(mopo_float q) {
      return (pow(0.5, q) - MIN_Q_POW) / (MAX_Q_POW - MIN_Q_POW);
    }

    inline bool isSilent(const mopo_float* buffer, int length) {
      for (int i = 0; i < length; ++i) {
        if (!closeToZero(buffer[i]))
          return false;
      }
      return true;
    }
  } // namespace utils
} // namespace mopo

#endif // UTILS_H
