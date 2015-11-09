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
#ifndef COMMON_H
#define COMMON_H

#include <mmintrin.h>

// Utilities.
#define UNUSED(x) (void)(x)
#define INTERPOLATE(s, e, f) ((s) + (f) * ((e) - (s)))

#define MOPO_DOUBLE

// Debugging.
#if DEBUG
#include <cassert>
#define MOPO_ASSERT(x) assert(x)
#else
#define MOPO_ASSERT(x) ((void)0)
#endif // DEBUG

namespace mopo {

#ifdef MOPO_DOUBLE
  typedef double mopo_float;
#else
  typedef float mopo_float;
#endif

  const mopo_float PI = 3.1415926535897932384626433832795;
  const int MAX_BUFFER_SIZE = 2048;
  const int DEFAULT_BUFFER_SIZE = 1024;
  const int DEFAULT_SAMPLE_RATE = 44100;
  const int MIDI_SIZE = 128;
  const int PPQ = 960; // Pulses per quarter note.
  const mopo_float VOICE_KILL_TIME = 0.02;

  // Common types of events across different Processors.
  enum VoiceEvent {
    kVoiceOff,     // Stop. (e.g. release in an envelope)
    kVoiceOn,      // Start. (e.g. start attack in an envelope)
    kVoiceReset,   // Reset. Immediately reset to initial state and play.
    kVoiceKill,   // Kill. Silence voice as fast as possible.
  };
} // namespace mopo

#endif // COMMON_H
