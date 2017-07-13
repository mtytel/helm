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
    const mopo_float DB_GAIN_CONVERSION_MULT = 20.0;
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
    const Value value_fifth(0.2);
    const Value value_tenth(0.1);
    const Value value_pi(PI);
    const Value value_2pi(2.0 * PI);
    const Value value_neg_one(-1.0);

#ifdef __SSE2__
    inline double min(double one, double two) {
      _mm_store_sd(&one, _mm_min_sd(_mm_set_sd(one),_mm_set_sd(two)));
      return one;
    }

    inline double max(double one, double two) {
      _mm_store_sd(&one, _mm_max_sd(_mm_set_sd(one),_mm_set_sd(two)));
      return one;
    }

    inline double clamp(double value, double min, double max) {
      _mm_store_sd(&value, _mm_min_sd(_mm_max_sd(_mm_set_sd(value),
                                                 _mm_set_sd(min)),
                                      _mm_set_sd(max)));
      return value;
    }

    inline float min(float one, float two) {
      _mm_store_ss(&one, _mm_min_ss(_mm_set_ss(one),_mm_set_ss(two)));
      return one;
    }

    inline float max(float one, float two) {
      _mm_store_ss(&one, _mm_max_ss(_mm_set_ss(one),_mm_set_ss(two)));
      return one;
    }

    inline float clamp(float value, float min, float max) {
      _mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value),
                                                 _mm_set_ss(min)),
                                      _mm_set_ss(max)));
      return value;
    }

#else
    inline mopo_float min(mopo_float one, mopo_float two) {
      return fmin(one, two);
    }

    inline mopo_float max(mopo_float one, mopo_float two) {
      return fmax(one, two);
    }

    inline mopo_float clamp(mopo_float value, mopo_float min, mopo_float max) {
      return fmin(max, fmax(value, min));
    }
#endif

    inline int imax(int one, int two) {
      return (one > two) ? one : two;
    }

    inline int imin(int one, int two) {
      return (one > two) ? two : one;
    }

    inline double interpolate(double from, double to, double t) {
      return t * (to - from) + from;
    }

    inline float interpolate(float from, float to, float t) {
      return fmaf(t, to - from, from);
    }

    inline mopo_float mod(double value, double* integral) {
      return modf(value, integral);
    }

    inline float mod(float value, float* integral) {
      return modff(value, integral);
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

    inline mopo_float dbToGain(mopo_float decibels) {
      return pow(10.0, decibels / DB_GAIN_CONVERSION_MULT);
    }

    inline mopo_float centsToRatio(mopo_float cents) {
      return pow(2.0, cents / CENTS_PER_OCTAVE);
    }

    inline mopo_float midiCentsToFrequency(mopo_float cents) {
      return MIDI_0_FREQUENCY * centsToRatio(cents);
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
      return pow(2.0, interpolate(MIN_Q_POW, MAX_Q_POW, magnitude));
    }

    inline mopo_float qToMagnitude(mopo_float q) {
      return (pow(0.5, q) - MIN_Q_POW) / (MAX_Q_POW - MIN_Q_POW);
    }

    inline int nextPowerOfTwo(mopo_float value) {
      return round(pow(2.0, ceil(log(value) / log(2.0))));
    }

    inline mopo_float quickerTanh(mopo_float value) {
      mopo_float square = value * value;
      return value / (1.0 + square / (3.0 + square / 5.0));
    }

    inline mopo_float quickTanh(mopo_float value) {
      mopo_float abs_value = fabs(value);
      mopo_float square = value * value;

      mopo_float num = value * (2.45550750702956 + 2.45550750702956 * abs_value +
                                square * (0.893229853513558 + 0.821226666969744 * abs_value));
      mopo_float den = 2.44506634652299 + (2.44506634652299 + square) *
                       fabs(value + 0.814642734961073 * value * abs_value);
      return num / den;
    }

    // Version of quick sin where phase is is [-0.5, 0.5]
    inline mopo_float quickerSin(mopo_float phase) {
      return phase * (8.0 - 16.0 * fabs(phase));
    }

    inline mopo_float quickSin(mopo_float phase) {
      mopo_float approx = quickerSin(phase);
      return approx * (0.776 + 0.224 * fabs(approx));
    }

    // Version of quick sin where phase is is [0, 1]
    inline mopo_float quickerSin1(mopo_float phase) {
      phase = 0.5 - phase;
      return phase * (8.0 - 16.0 * fabs(phase));
    }

    inline mopo_float quickSin1(mopo_float phase) {
      mopo_float approx = quickerSin1(phase);
      return approx * (0.776 + 0.224 * fabs(approx));
    }

    inline bool isSilent(const mopo_float* buffer, int length) {
      for (int i = 0; i < length; ++i) {
        if (!closeToZero(buffer[i]))
          return false;
      }
      return true;
    }

    inline mopo_float rms(const mopo_float* buffer, int num) {
      mopo_float square_total = 0.0;
      VECTORIZE_LOOP
      for (int i = 0; i < num; ++i)
        square_total += buffer[i] * buffer[i];

      return sqrt(square_total / num);
    }

    inline mopo_float peak(const mopo_float* buffer, int num, int skip) {
      mopo_float peak = 0.0;
      for (int i = 0; i < num; i += skip)
        peak = fmax(peak, fabs(buffer[i]));

      return peak;
    }

    inline void zeroBuffer(mopo_float* buffer, int size) {
      VECTORIZE_LOOP
      for (int i = 0; i < size; ++i)
        buffer[i] = 0.0;
    }

    inline void zeroBuffer(int* buffer, int size) {
      VECTORIZE_LOOP
      for (int i = 0; i < size; ++i)
        buffer[i] = 0;
    }

    inline void copyBuffer(mopo_float* dest, const mopo_float* source, int size) {
      VECTORIZE_LOOP
      for (int i = 0; i < size; ++i)
        dest[i] = source[i];
    }

    inline void copyBufferf(float* dest, const float* source, int size) {
      VECTORIZE_LOOP
      for (int i = 0; i < size; ++i)
        dest[i] = source[i];
    }
  } // namespace utils
} // namespace mopo

#endif // UTILS_H
