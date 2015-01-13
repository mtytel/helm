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

#include "smooth_value.h"

#include <cmath>

#define SMOOTH_CUTOFF 3.0

namespace mopo {

  SmoothValue::SmoothValue(mopo_float value) :
      Value(value), target_value_(value) { }

  void SmoothValue::setSampleRate(int sample_rate) {
    sample_rate_ = sample_rate;
    decay_ = 1 - exp(-2.0 * PI * SMOOTH_CUTOFF / sample_rate_);
  }

  void SmoothValue::process() {
    for (int i = 0; i < buffer_size_; ++i)
      outputs_[0]->buffer[i] = tick();
  }

  inline mopo_float SmoothValue::tick() {
    value_ = INTERPOLATE(value_, target_value_, decay_);
    return value_;
  }
} // namespace mopo
