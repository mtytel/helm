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
#ifndef DETUNE_LOOKUP_H
#define DETUNE_LOOKUP_H

#include "common.h"
#include "utils.h"

#include <cmath>

namespace mopo {

  namespace {
    const mopo_float MIN_LOOKUP_CENTS = -100.0;
    const mopo_float MAX_LOOKUP_CENTS = 100.0;
    const mopo_float CENTS_RANGE = MAX_LOOKUP_CENTS - MIN_LOOKUP_CENTS;
    const int DETUNE_LOOKUP_RESOLUTION = 2046;
  } // namespace

  class DetuneLookupSingleton {
    public:
      DetuneLookupSingleton() {
        for (int i = 0; i < DETUNE_LOOKUP_RESOLUTION + 2; ++i) {
          mopo_float t = (1.0 * i) / DETUNE_LOOKUP_RESOLUTION;
          mopo_float cents = utils::interpolate(MIN_LOOKUP_CENTS, MAX_LOOKUP_CENTS, t);
          detune_lookup_[i] = utils::centsToRatio(cents);
        }
      }

      mopo_float detuneLookup(mopo_float cents) const {
        mopo_float t = (cents - MIN_LOOKUP_CENTS) / CENTS_RANGE;
        mopo_float index = DETUNE_LOOKUP_RESOLUTION * utils::clamp(t, 0.0, 1.0);
        int int_index = index;
        mopo_float fraction = index - int_index;

        return utils::interpolate(detune_lookup_[int_index],
                                  detune_lookup_[int_index + 1], fraction);
      }

    private:
      mopo_float detune_lookup_[DETUNE_LOOKUP_RESOLUTION + 2];
  };

  class DetuneLookup {
    public:
      static mopo_float detuneLookup(mopo_float cents) {
        return lookup_.detuneLookup(cents);
      }

    private:
      static const DetuneLookupSingleton lookup_;
  };
} // namespace mopo

#endif // DETUNE_LOOKUP_H
