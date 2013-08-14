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
#ifndef LAF_H
#define LAF_H

#define BUFFER_SIZE 64
#define DEFAULT_SAMPLE_RATE 44100
#define MIDI_SIZE 128
#define PI 3.1415926535897932384626433832795

#define UNUSED(x) (void)(x)
#define CLAMP(s, b, v) ((v) < (s) ? (s) : ((v) > (b) ? (b) : (v)))
#define INTERPOLATE(s, e, f) ((s) + (f) * ((e) - (s)))
#define NORMALIZE(t) ((t) - floor(t))

#define DEBUG

#ifdef DEBUG
#define LAF_ASSERT(x) assert(x)
#else
#define LAF_ASSERT(x) (void)(x)
#endif // DEBUG

#include <assert.h>
#include <math.h>

namespace laf {

  typedef double laf_sample;

  // Common types of events across different Processors.
  enum VoiceEvent {
    kOff,     // Stop. (e.g. releast in an envelope)
    kOn,      // Start. (e.g. start attack in an envelope)
    kReset,   // Reset. Immediately reset to initial state.
  };
} // namespace laf

#endif // LAF_H
