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

#include "send_receive.h"

#include "processor_router.h"

namespace mopo {

  Send::Send() : Processor(1, 1) {
    memory_output_ = new MemoryOutput();
    memory_output_->owner = this;
    memory_output_->memory = &memory_;
  }

  void Send::process() {
    for (int i = 0; i < buffer_size_; ++i)
      memory_.push(inputs_[0]->at(i));
    memory_output_->memory = &memory_;
  }

  Receive::Receive() : Processor(kNumInputs, 1) {
    memory_input_ = new MemoryInput();
    memory_input_->owner = this;
  }

  void Receive::process() {
    mopo_float adjust = buffer_size_;
    if (router_ && !router_->areOrdered(memory_input_->source->owner,
                                        memory_input_->owner)) {
      adjust = 0;
    }

    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float time = inputs_[kDelayTime]->at(i) * sample_rate_ + adjust;
      outputs_[0]->buffer[i] = memory_input_->get(time);
      adjust -= 1.0;
    }
  }

  void Receive::setSend(const Send* send, bool dependent) {
    memory_input_->source = send->memory_output();
    if (dependent)
      plug(send, kDependent);
  }
} // namespace mopo
