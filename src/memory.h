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
#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

#include <algorithm>
#include <cmath>

namespace mopo {

  // A processor utility to store a stream of data for later lookup.
  class Memory {
    public:
      Memory(int size);
      Memory(const Memory& other);
      ~Memory();

      void push(mopo_float sample) {
        offset_ = (offset_ + 1) & bitmask_;
        memory_[offset_] = sample;
      }

      mopo_float getIndex(int index) const {
        return memory_[(offset_ - index) & bitmask_];
      }

      mopo_float get(mopo_float past) const {
        MOPO_ASSERT(past >= 0.0);
        int index = std::max<int>(past, 1);
        mopo_float sample_fraction = past - index;

        mopo_float from = getIndex(index - 1);
        mopo_float to = getIndex(index);
        return INTERPOLATE(from, to, sample_fraction);
      }

      unsigned int getOffset() const { return offset_; }

      void setOffset(int offset) { offset_ = offset; }

      const mopo_float* getPointer(int past) const {
        return memory_ + ((offset_ - past) & bitmask_);
      }

      const mopo_float* getBuffer() const {
        return memory_;
      }

      int getSize() const {
        return size_;
      }

    protected:
      mopo_float* memory_;
      unsigned int size_;
      unsigned int bitmask_;
      unsigned int offset_;
  };
} // namespace mopo

#endif // MEMORY_H
