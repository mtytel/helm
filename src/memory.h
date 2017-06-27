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

#include "utils.h"

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

      void pushBlock(const mopo_float* samples, int num) {
        int next_offset = (offset_ + num) & bitmask_;
        if (next_offset < offset_) {
          int block1 = num - next_offset - 1;
          memcpy(memory_ + offset_ + 1, samples, sizeof(mopo_float) * block1);
          memcpy(memory_, samples + block1, sizeof(mopo_float) * next_offset);
        }
        else
          memcpy(memory_ + offset_ + 1, samples, sizeof(mopo_float) * num);

        offset_ = next_offset;
      }

      void pushZero(int num) {
        int next_offset = (offset_ + num) & bitmask_;
        if (next_offset < offset_) {
          int block1 = num - next_offset - 1;
          memset(memory_ + offset_ + 1, 0, sizeof(mopo_float) * block1);
          memset(memory_, 0, sizeof(mopo_float) * next_offset);
        }
        else
          memset(memory_ + offset_ + 1, 0, sizeof(mopo_float) * num);

        offset_ = next_offset;
      }

      inline mopo_float getIndex(int index) const {
        int spot = (offset_ - index) & bitmask_;
        MOPO_ASSERT(spot < size_);
        return memory_[spot];
      }

      inline mopo_float get(mopo_float past) const {
        MOPO_ASSERT(past >= 0.0);
        int index = utils::imax(past, 1);
        mopo_float sample_fraction = past - index;

        mopo_float from = getIndex(index - 1);
        mopo_float to = getIndex(index);
        return utils::interpolate(from, to, sample_fraction);
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
