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

#include "processor.h"

#include "processor_router.h"

namespace laf {

  const Processor::Output Processor::null_source_;

  Processor::Processor(int num_inputs, int num_outputs) : router_(0) {
    for (int i = 0; i < num_inputs; ++i) {
      Input* input = new Input();

      // All inputs start off with null input.
      input->source = &Processor::null_source_;
      registerInput(input);
    }

    for (int i = 0; i < num_outputs; ++i) {
      Output* output = new Output();

      // All outputs are owned by this Processor.
      output->owner = this;
      registerOutput(output);
    }
  }

  void Processor::plug(const Output* source, unsigned int input_index) {
    LAF_ASSERT(input_index < inputs_.size());
    LAF_ASSERT(source);

    inputs_[input_index]->source = source;

    if (router_)
      router_->reorder(this);
  }

  void Processor::plug(const Processor* source, unsigned int input_index) {
    plug(source->output(), input_index);
  }

  void Processor::registerInput(Input* input) {
    inputs_.push_back(input);
  }

  void Processor::registerOutput(Output* output) {
    outputs_.push_back(output);
  }

  Processor::Input* Processor::input(unsigned int index) const {
    LAF_ASSERT(index < inputs_.size());

    return inputs_[index];
  }

  Processor::Output* Processor::output(unsigned int index) const {
    LAF_ASSERT(index < outputs_.size());

    return outputs_[index];
  }
} // namespace laf
