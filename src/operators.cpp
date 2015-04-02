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

#if defined (__APPLE__)
  #include <Accelerate/Accelerate.h>
  #define USE_APPLE_ACCELERATE
#endif

namespace mopo {

  void Operator::process() {
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }


  void Clamp::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vclipD(input()->source->buffer, 1,
                &min_, &max_,
                output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void Negate::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vnegD(input()->source->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void LinearScale::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vsmulD(input()->source->buffer, 1, &scale_,
                output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void Add::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vaddD(input(0)->source->buffer, 1,
               input(1)->source->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void Subtract::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vsubD(input(0)->source->buffer, 1,
               input(1)->source->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void Multiply::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vmulD(input(0)->source->buffer, 1,
               input(1)->source->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void Interpolate::process() {
#ifdef USE_APPLE_ACCELERATE
    vDSP_vsbmD(input(kTo)->source->buffer, 1,
               input(kFrom)->source->buffer, 1,
               input(kFractional)->source->buffer, 1,
               output()->buffer, 1, buffer_size_);

    vDSP_vaddD(input(kFrom)->source->buffer, 1,
               output()->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
  }

  void BilinearInterpolate::process() {
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }

  void VariableAdd::process() {
    memset(output()->buffer, 0, buffer_size_ * sizeof(mopo_float));

    int num_inputs = inputs_->size();
    for (int i = 0; i < num_inputs; ++i) {
      if (input(i)->source != &Processor::null_source_) {
#ifdef USE_APPLE_ACCELERATE
        vDSP_vaddD(input(i)->source->buffer, 1,
                   output()->buffer, 1,
                   output()->buffer, 1, buffer_size_);
#else
        for (int s = 0; s < buffer_size_; ++s)
          output()->buffer[s] += input(i)->at(s);
#endif
      }
    }
  }

  void SampleAndHoldBuffer::process() {
    if (new_value == output()->buffer[0])
      return;
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }

  void LinearSmoothBuffer::process() {
    mopo_float new_value = input()->source->buffer[0];
    if (new_value == last_value_ && last_start_value_ == new_value)
      return;

    mopo_float inc = (new_value - last_value_) / buffer_size_;
    last_start_value_ = last_value_;

    for (int i = 0; i < buffer_size_; ++i) {
      last_value_ += inc;
      output()->buffer[i] = last_value_;
    }
    last_value_ = new_value;
  }
} // namespace mopo
