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

#include "smooth_value.h"

#include <cmath>

#include "utils.h"

#define SMOOTH_CUTOFF 3.0

namespace mopo {

  SmoothValue::SmoothValue(mopo_float value) :
      Value(value), target_value_(value), decay_(1.0) { }

  void SmoothValue::setSampleRate(int sample_rate) {
    sample_rate_ = sample_rate;
    decay_ = 1 - exp(-2.0 * PI * SMOOTH_CUTOFF / sample_rate_);
  }

  void SmoothValue::process() {
    if (value_ == target_value_ && value_ == output()->buffer[0] &&
        value_ == output()->buffer[buffer_size_ - 1]) {
      return;
    }
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }

  inline void SmoothValue::tick(int i) {
    value_ = utils::interpolate(value_, target_value_, decay_);
    output()->buffer[i] = value_;
  }

  namespace cr {

    SmoothValue::SmoothValue(mopo_float value) :
        Value(value), target_value_(value), decay_(1.0), num_samples_(1) { }

    void SmoothValue::setSampleRate(int sample_rate) {
      Value::setSampleRate(sample_rate);
      computeDecay();
    }

    void SmoothValue::setBufferSize(int buffer_size) {
      Value::setBufferSize(buffer_size);
      num_samples_ = buffer_size;
      computeDecay();
    }

    void SmoothValue::process() {
      value_ = utils::interpolate(value_, target_value_, decay_);
      output()->buffer[0] = value_;
    }

    void SmoothValue::computeDecay() {
      decay_ = 1 - exp(-2.0 * PI * SMOOTH_CUTOFF * num_samples_ / sample_rate_);
    }
  } // namespace cr
} // namespace mopo
