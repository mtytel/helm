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
#ifndef MIDI_LOOKUP_H
#define MIDI_LOOKUP_H

#include "laf.h"

#define MIDI_0_FREQUENCY 8.1757989156
#define NOTES_PER_OCTAVE 12
#define CENTS_PER_NOTE 100
#define MAX_CENTS (MIDI_SIZE * CENTS_PER_NOTE)

namespace laf {

  class MidiLookupSingleton {
    public:
      MidiLookupSingleton() {
        laf_sample cents_per_octave = CENTS_PER_NOTE * NOTES_PER_OCTAVE;
        for (int i = 0; i <= MAX_CENTS; ++i) {
          frequency_lookup_[i] = MIDI_0_FREQUENCY *
                                 pow(2, i / cents_per_octave);
        }
      }

      laf_sample centsLookup(laf_sample cents_from_0) const {
        if (cents_from_0 >= MAX_CENTS)
          return frequency_lookup_[MAX_CENTS];
        if (cents_from_0 < 0)
          return frequency_lookup_[0];

        int full_cents = cents_from_0;
        laf_sample fraction_cents = cents_from_0 - full_cents;

        return INTERPOLATE(frequency_lookup_[full_cents],
                           frequency_lookup_[full_cents + 1], fraction_cents);
      }

    private:
      laf_sample frequency_lookup_[MAX_CENTS + 1];
  };

  class MidiLookup {
    public:
      static laf_sample centsLookup(laf_sample cents_from_0) {
        return lookup_.centsLookup(cents_from_0);
      }

    private:
      static const MidiLookupSingleton lookup_;
  };
} // namespace laf

#endif // MIDI_LOOKUP_H
