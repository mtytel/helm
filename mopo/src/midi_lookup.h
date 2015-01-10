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
#ifndef MIDI_LOOKUP_H
#define MIDI_LOOKUP_H

#include "mopo.h"

#include <cmath>

namespace mopo {

  namespace {
    const mopo_float MIDI_0_FREQUENCY = 8.1757989156;
    const int NOTES_PER_OCTAVE = 12;
    const int CENTS_PER_NOTE = 100;
    const int MAX_CENTS = MIDI_SIZE * CENTS_PER_NOTE;
  } // namespace

  class MidiLookupSingleton {
    public:
      MidiLookupSingleton() {
        mopo_float cents_per_octave = CENTS_PER_NOTE * NOTES_PER_OCTAVE;
        for (int i = 0; i < MAX_CENTS + 2; ++i) {
          frequency_lookup_[i] = MIDI_0_FREQUENCY *
                                 pow(2, i / cents_per_octave);
        }
      }

      mopo_float centsLookup(mopo_float cents_from_0) const {
        int full_cents = CLAMP(cents_from_0, 0.0, MAX_CENTS);
        mopo_float fraction_cents = cents_from_0 - full_cents;

        return INTERPOLATE(frequency_lookup_[full_cents],
                           frequency_lookup_[full_cents + 1], fraction_cents);
      }

    private:
      mopo_float frequency_lookup_[MAX_CENTS + 2];
  };

  class MidiLookup {
    public:
      static mopo_float centsLookup(mopo_float cents_from_0) {
        return lookup_.centsLookup(cents_from_0);
      }

    private:
      static const MidiLookupSingleton lookup_;
  };
} // namespace mopo

#endif // MIDI_LOOKUP_H
