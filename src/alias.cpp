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

#include "alias.h"

namespace mopo {

  Alias::Alias() : Processor(Alias::kNumInputs, 1),
                   current_sample_(0.0), static_samples_(0.0) { }

  void Alias::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));
    MOPO_ASSERT(inputMatchesBufferSize(kWet));
    MOPO_ASSERT(inputMatchesBufferSize(kFrequency));

    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }
} // namespace mopo
