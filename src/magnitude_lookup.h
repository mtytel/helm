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
#ifndef MAGNITUDE_LOOKUP_H
#define MAGNITUDE_LOOKUP_H

#include "common.h"
#include "utils.h"

#include <cmath>

namespace mopo {

  namespace {
    const mopo_float MIN_DB_LOOKUP = -60.0;
    const mopo_float MAX_DB_LOOKUP = 60.0;
    const mopo_float DB_RANGE = MAX_DB_LOOKUP - MIN_DB_LOOKUP;
    const int MAGNITUDE_LOOKUP_RESOLUTION = 2046;

  } // namespace

  class MagnitudeLookupSingleton {
    public:
      MagnitudeLookupSingleton() {
        for (int i = 0; i < MAGNITUDE_LOOKUP_RESOLUTION + 2; ++i) {
          mopo_float t = (1.0 * i) / MAGNITUDE_LOOKUP_RESOLUTION;
          mopo_float decibels = utils::interpolate(MIN_DB_LOOKUP, MAX_DB_LOOKUP, t);
          magnitude_lookup_[i] = utils::dbToGain(decibels);
        }
      }

      mopo_float magnitudeLookup(mopo_float decibels) const {
        mopo_float t = (decibels - MIN_DB_LOOKUP) / DB_RANGE;
        mopo_float index = MAGNITUDE_LOOKUP_RESOLUTION * utils::clamp(t, 0.0, 1.0);
        int int_index = index;
        mopo_float fraction = index - int_index;

        return utils::interpolate(magnitude_lookup_[int_index],
                                  magnitude_lookup_[int_index + 1], fraction);
      }

    private:
      mopo_float magnitude_lookup_[MAGNITUDE_LOOKUP_RESOLUTION + 2];
  };

  class MagnitudeLookup {
    public:
      static mopo_float magnitudeLookup(mopo_float decibels) {
        return lookup_.magnitudeLookup(decibels);
      }

    private:
      static const MagnitudeLookupSingleton lookup_;
  };
} // namespace mopo

#endif // MAGNITUDE_LOOKUP_H
