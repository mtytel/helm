/* Copyright 2013 Little IO
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
#ifndef MOPO_H
#define MOPO_H

// Utilities.
#define UNUSED(x) (void)(x)
#define CLAMP(v, s, b) ((v) < (s) ? (s) : ((v) > (b) ? (b) : (v)))
#define INTERPOLATE(s, e, f) ((s) + (f) * ((e) - (s)))

// Debugging.
#define DEBUG
#ifdef DEBUG
#include <cassert>
#define MOPO_ASSERT(x) assert(x)
#else
#define MOPO_ASSERT(x) (void)(x)
#endif // DEBUG

namespace mopo {

  const double PI = 3.1415926535897932384626433832795;
  const int MAX_BUFFER_SIZE = 4096;
  const int DEFAULT_BUFFER_SIZE = 64;
  const int DEFAULT_SAMPLE_RATE = 44100;
  const int MIDI_SIZE = 128;
  const int PPQ = 15360; // Pulses per quarter note.

  typedef double mopo_float;

  // Common types of events across different Processors.
  enum VoiceEvent {
    kVoiceOff,     // Stop. (e.g. release in an envelope)
    kVoiceOn,      // Start. (e.g. start attack in an envelope)
    kVoiceReset,   // Reset. Immediately reset to initial state.
  };
} // namespace mopo

#endif // MOPO_H
