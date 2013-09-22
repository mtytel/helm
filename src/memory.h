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
#ifndef MEMORY_H
#define MEMORY_H

#include "laf.h"

#include <algorithm>
#include <string.h>

// MAX_MEMORY must be a power of 2.
#define MAX_MEMORY 1048576
#define MEMORY_BITMASK 1048575

namespace laf {

  class Memory {
    public:
      Memory() : offset_(0) {
        memset(memory_, 0, MAX_MEMORY * sizeof(laf_float));
      }

      inline void push(laf_float sample) {
        offset_ = (offset_ + 1) & MEMORY_BITMASK;
        memory_[offset_] = sample;
      }

      inline laf_float getIndex(int index) const {
        return memory_[(offset_ - index) & MEMORY_BITMASK];
      }

      inline laf_float get(laf_float past) const {
        double float_index;
        laf_float sample_fraction = modf(past, &float_index);
        int index = std::max<int>(float_index, 1);

        // TODO(mtytel): Quadratic or all-pass interpolation is better.
        laf_float from = getIndex(index - 1);
        laf_float to = getIndex(index);
        return INTERPOLATE(from, to, sample_fraction);
      }

    protected:
      laf_float memory_[MAX_MEMORY];
      unsigned int offset_;
  };
} // namespace laf

#endif // MEMORY_H
