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

// Constants.
#define BUFFER_SIZE 64
#define DEFAULT_SAMPLE_RATE 44100
#define MIDI_SIZE 128
#define PI 3.1415926535897932384626433832795
#define PPQ 15360 // Pulses per quarter note.

// Utilities.
#define UNUSED(x) (void)(x)
#define CLAMP(v, s, b) ((v) < (s) ? (s) : ((v) > (b) ? (b) : (v)))
#define INTERPOLATE(s, e, f) ((s) + (f) * ((e) - (s)))

// Debugging.
#define DEBUG
#ifdef DEBUG
#include <assert.h>
#define MOPO_ASSERT(x) assert(x)
#else
#define MOPO_ASSERT(x) (void)(x)
#endif // DEBUG

// Common Types.
namespace mopo {

  typedef double mopo_float;

  // Common types of events across different Processors.
  enum VoiceEvent {
    kVoiceOff,     // Stop. (e.g. release in an envelope)
    kVoiceOn,      // Start. (e.g. start attack in an envelope)
    kVoiceReset,   // Reset. Immediately reset to initial state.
  };
} // namespace mopo

#endif // MOPO_H
