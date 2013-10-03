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

#include "send_receive.h"

#include "processor_router.h"

namespace laf {

  Send::Send() : Processor(1, 0) { }

  void Send::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i)
      memory_.push(inputs_[0]->at(i));
  }

  Receive::Receive() : Processor(1, 1) { }

  void Receive::process() {
    LAF_ASSERT(send_);
    laf_float adjust = 0;
    if (router_ && !router_->areOrdered(send_, this))
      adjust = BUFFER_SIZE;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
      laf_float delay_time = inputs_[0]->at(i) + adjust;
      outputs_[0]->buffer[i] = send_->get(delay_time);
    }
  }
} // namespace laf
