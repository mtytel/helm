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

#include "bypass_router.h"

namespace mopo {

  BypassRouter::BypassRouter(int num_inputs, int num_outputs) :
      ProcessorRouter(num_inputs, num_outputs) { }

  void BypassRouter::process() {
    mopo_float should_bypass = input(kBypass)->at(0);
    if (should_bypass) {
      memcpy(output()->buffer, input()->source->buffer,
             buffer_size_ * sizeof(mopo_float));
    }
    else
      ProcessorRouter::process();
  }
} // namespace mopo
