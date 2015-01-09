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

#include "smooth_filter.h"

namespace mopo {

  SmoothFilter::SmoothFilter() : Processor(SmoothFilter::kNumInputs, 1) {
    last_value_ = 0.0;
  }

  void SmoothFilter::process() {
    mopo_float decay = inputs_[kDecay]->at(0);

    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float target = inputs_[kTarget]->at(i);
      last_value_ = INTERPOLATE(last_value_, target, decay);
      outputs_[0]->buffer[i] = last_value_;
    }
  }
} // namespace mopo
