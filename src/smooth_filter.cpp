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

#include "smooth_filter.h"

namespace laf {

  SmoothFilter::SmoothFilter() : Processor(SmoothFilter::kNumInputs, 1) {
    last_value_ = 0.0;
  }

  void SmoothFilter::process() {
    laf_sample decay = inputs_[kDecay]->at(0);

    for (int i = 0; i < BUFFER_SIZE; ++i) {
      laf_sample target = inputs_[kTarget]->at(i);
      last_value_ = INTERPOLATE(last_value_, target, decay);
      outputs_[0]->buffer[i] = last_value_;
    }
  }
} // namespace laf
