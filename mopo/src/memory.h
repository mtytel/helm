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
#ifndef MEMORY_H
#define MEMORY_H

#include "mopo.h"

#include <algorithm>
#include <cmath>
#include <cstring>

// MAX_MEMORY must be a power of 2.
#define MAX_MEMORY 1048576
#define MEMORY_BITMASK 1048575

namespace mopo {

  // A processor utility to store a stream of data for later lookup.
  class Memory {
    public:
      Memory() : offset_(0) {
        memset(memory_, 0, MAX_MEMORY * sizeof(mopo_float));
      }

      inline void push(mopo_float sample) {
        offset_ = (offset_ + 1) & MEMORY_BITMASK;
        memory_[offset_] = sample;
      }

      inline mopo_float getIndex(int index) const {
        return memory_[(offset_ - index) & MEMORY_BITMASK];
      }

      inline mopo_float get(mopo_float past) const {
        double float_index;
        mopo_float sample_fraction = modf(past, &float_index);
        int index = std::max<int>(float_index, 1);

        // TODO(mtytel): Quadratic or all-pass interpolation is better.
        mopo_float from = getIndex(index - 1);
        mopo_float to = getIndex(index);
        return INTERPOLATE(from, to, sample_fraction);
      }

    protected:
      mopo_float memory_[MAX_MEMORY];
      unsigned int offset_;
  };
} // namespace mopo

#endif // MEMORY_H
