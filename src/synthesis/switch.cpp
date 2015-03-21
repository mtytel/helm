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

#include "switch.h"

#include <cmath>

namespace mopo {

  Switch::Switch() : Processor(kNumInputs, 1) { }

  void Switch::process() {
    unsigned int source = static_cast<int>(input(kSource)->at(0));
    source = CLAMP(source, 0, numInputs() - kNumInputs);

    memcpy(output()->buffer, input(kNumInputs + source)->source->buffer,
           buffer_size_ * sizeof(mopo_float));
  }
} // namespace mopo
