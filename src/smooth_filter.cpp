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

#include "smooth_filter.h"
#include <cmath>

#include "utils.h"

namespace mopo {

  SmoothFilter::SmoothFilter(mopo_float start_value) :
      Processor(SmoothFilter::kNumInputs, 1), last_value_(start_value) { }

  void SmoothFilter::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kTarget));

    mopo_float half_life = input(kHalfLife)->at(0);
    mopo_float decay = 0.0;
    if (half_life > 0.0)
      decay = std::pow(0.5, 1.0 / (half_life * sample_rate_));

    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float target = input(kTarget)->at(i);
      last_value_ = utils::interpolate(target, last_value_, decay);
      output(0)->buffer[i] = last_value_;
    }
  }

  namespace cr {
    SmoothFilter::SmoothFilter(mopo_float start_value) :
        Processor(SmoothFilter::kNumInputs, 1, true), last_value_(start_value) { }

    void SmoothFilter::process() {
      mopo_float half_life = input(kHalfLife)->at(0);
      mopo_float decay = 0.0;
      if (half_life > 0.0)
        decay = std::pow(0.5, samples_to_process_ / (half_life * sample_rate_));

      mopo_float target = input(kTarget)->at(0);
      last_value_ = utils::interpolate(target, last_value_, decay);
      output(0)->buffer[0] = last_value_;
    }
  }
} // namespace mopo
