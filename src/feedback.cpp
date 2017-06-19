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

#include "feedback.h"

#include "processor_router.h"

namespace mopo {

  void Feedback::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

    refreshOutput();

    if (control_rate_)
      buffer_[0] = input(0)->at(0);
    else
      utils::copyBuffer(buffer_, input(0)->source->buffer, buffer_size_);
  }

  void Feedback::refreshOutput() {
    if (control_rate_)
      output(0)->buffer[0] = buffer_[0];
    else
      utils::copyBuffer(output(0)->buffer, buffer_, MAX_BUFFER_SIZE);
  }
} // namespace mopo
