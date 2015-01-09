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

#include "operators.h"

namespace mopo {

  void Operator::process() {
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }

  void VariableAdd::process() {
    memset(outputs_[0]->buffer, 0, buffer_size_ * sizeof(mopo_float));

    int num_inputs = inputs_.size();
    for (int i = 0; i < num_inputs; ++i) {
      if (inputs_[i]->source != &Processor::null_source_) {
        for (int s = 0; s < buffer_size_; ++s)
          outputs_[0]->buffer[s] += inputs_[i]->at(s);
      }
    }
  }
} // namespace mopo
