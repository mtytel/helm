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

#include "memory.h"
#include "utils.h"

#include <cmath>
#include <cstring>

namespace mopo {

  Memory::Memory(int size) : offset_(0) {
    // Get the next largest power of two for optimization.
    size_ = pow(2.0, ceil(log(size) / log(2)));
    bitmask_ = size_ - 1;
    memory_ = new mopo_float[size_];
    utils::zeroBuffer(memory_, size_);
  }

  Memory::Memory(const Memory& other) {
    this->memory_ = new mopo_float[other.size_];
    utils::zeroBuffer(this->memory_, other.size_);
    this->size_ = other.size_;
    this->bitmask_ = other.bitmask_;
    this->offset_ = other.offset_;
  }

  Memory::~Memory() {
    delete memory_;
  }
} // namespace mopo
