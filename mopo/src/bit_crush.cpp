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

#include "bit_crush.h"

#include <cmath>

namespace mopo {

  BitCrush::BitCrush() : Processor(BitCrush::kNumInputs, 1),
                         magnification_(0.0) { }

  void BitCrush::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));
    MOPO_ASSERT(inputMatchesBufferSize(kWet));

    mopo_float bits = input(kBits)->at(0);
    magnification_ = pow(2.0, bits / 2.0);

    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }
} // namespace mopo
