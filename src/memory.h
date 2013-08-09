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

#define MAX_MEMORY 1000000

namespace laf {

  class Memory {
    public:
      Memory() : offset_(0) {
        memset(memory_, 0, MAX_MEMORY * sizeof(laf_sample));
      }

      inline void push(laf_sample sample) {
        offset_ = (offset_ + 1) % MAX_MEMORY;
        memory_[offset_] = sample;
      }

      inline laf_sample getIndex(int index) const {
        return memory_[(MAX_MEMORY + offset_ - index) % MAX_MEMORY];
      }

      inline laf_sample get(laf_sample past) const {
        int index = std::max<laf_sample>(past, 1);
        laf_sample sample_fraction = past - index;

        // TODO(mtytel): Quadratic or all-pass interpolation is better.
        laf_sample from = getIndex(index - 1);
        laf_sample to = getIndex(index);
        return INTERPOLATE(from, to, sample_fraction);
      }

    protected:
      laf_sample memory_[MAX_MEMORY];
      int offset_;
  };
} // namespace laf

#endif // MEMORY_H
