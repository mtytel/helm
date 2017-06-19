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

#include "operators.h"

#if defined (__APPLE__)
  #include <Accelerate/Accelerate.h>
  #define USE_APPLE_ACCELERATE
#endif

#include <iostream>

namespace mopo {

  void Operator::process() {
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
  }

  void Bypass::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

    utils::copyBuffer(output()->buffer, input()->source->buffer, buffer_size_);

    output()->triggered = input()->source->triggered;
    output()->trigger_value = input()->source->trigger_value;
    output()->trigger_offset = input()->source->trigger_offset;
  }

  void Clamp::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

#ifdef USE_APPLE_ACCELERATE
    vDSP_vclipD(input()->source->buffer, 1,
                &min_, &max_,
                output()->buffer, 1, buffer_size_);
#else
    mopo_float* dest = output()->buffer;
    const mopo_float* source = input()->source->buffer;

    for (int i = 0; i < buffer_size_; ++i)
      bufferTick(dest, source, i);
#endif
    processTriggers();
  }

  void Negate::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

#ifdef USE_APPLE_ACCELERATE
    vDSP_vnegD(input()->source->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
    processTriggers();
  }

  void LinearScale::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

#ifdef USE_APPLE_ACCELERATE
    vDSP_vsmulD(input()->source->buffer, 1, &scale_,
                output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
    processTriggers();
  }

  void Add::process() {
    MOPO_ASSERT(inputMatchesBufferSize(0));
    MOPO_ASSERT(inputMatchesBufferSize(1));

    mopo_float* dest = output()->buffer;
    const mopo_float* source_left = input(0)->source->buffer;
    const mopo_float* source_right = input(1)->source->buffer;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size_; ++i)
      bufferTick(dest, source_left, source_right, i);

    processTriggers();
  }

  void Subtract::process() {
    MOPO_ASSERT(inputMatchesBufferSize(0));
    MOPO_ASSERT(inputMatchesBufferSize(1));

#ifdef USE_APPLE_ACCELERATE
    vDSP_vsubD(input(0)->source->buffer, 1,
               input(1)->source->buffer, 1,
               output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
    processTriggers();
  }

  void Multiply::process() {
    MOPO_ASSERT(inputMatchesBufferSize(0));
    MOPO_ASSERT(inputMatchesBufferSize(1));

    mopo_float* dest = output()->buffer;
    const mopo_float* source_left = input(0)->source->buffer;
    const mopo_float* source_right = input(1)->source->buffer;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size_; ++i)
      bufferTick(dest, source_left, source_right, i);

    processTriggers();
  }

  void Interpolate::process() {
    MOPO_ASSERT(inputMatchesBufferSize(0));
    MOPO_ASSERT(inputMatchesBufferSize(1));
    MOPO_ASSERT(inputMatchesBufferSize(2));

    mopo_float* dest = output()->buffer;
    const mopo_float* from = input(kFrom)->source->buffer;
    const mopo_float* to = input(kTo)->source->buffer;
    const mopo_float* fractional = input(kFractional)->source->buffer;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size_; ++i)
      bufferTick(dest, from, to, fractional, i);

    processTriggers();
  }

  void BilinearInterpolate::process() {
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
    processTriggers();
  }

  void VariableAdd::process() {
#if DEBUG
    for (int i = 0; i < inputs_->size(); ++i)
      MOPO_ASSERT(inputMatchesBufferSize(i));
#endif

    mopo_float* dest = output()->buffer;

    if (isControlRate()) {
      dest[0] = 0.0;

      int num_inputs = inputs_->size();
      for (int i = 0; i < num_inputs; ++i)
        dest[0] += input(i)->at(0);
    }
    else {
      utils::zeroBuffer(dest, buffer_size_);

      int num_inputs = inputs_->size();
      for (int i = 0; i < num_inputs; ++i) {
        if (input(i)->source != &Processor::null_source_) {
#ifdef USE_APPLE_ACCELERATE
          vDSP_vaddD(input(i)->source->buffer, 1,
                     output()->buffer, 1,
                     output()->buffer, 1, buffer_size_);
#else
          const mopo_float* source = input(i)->source->buffer;

          VECTORIZE_LOOP
          for (int s = 0; s < buffer_size_; ++s)
            dest[s] += source[s];
#endif
        }
      }
    }
    processTriggers();
  }

  void FrequencyToPhase::process() {
#ifdef USE_APPLE_ACCELERATE
    mopo_float sample_rate = sample_rate_;
    vDSP_vsdivD(input()->source->buffer, 1, &sample_rate,
                output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
    processTriggers();
  }

  void FrequencyToSamples::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

#ifdef USE_APPLE_ACCELERATE
    mopo_float sample_rate = sample_rate_;
    vDSP_svdivD(&sample_rate, input()->source->buffer, 1,
                output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
    processTriggers();
  }

  void TimeToSamples::process() {
    MOPO_ASSERT(inputMatchesBufferSize());

#ifdef USE_APPLE_ACCELERATE
    mopo_float sample_rate = sample_rate_;
    vDSP_vsmulD(input()->source->buffer, 1, &sample_rate,
                output()->buffer, 1, buffer_size_);
#else
    for (int i = 0; i < buffer_size_; ++i)
      tick(i);
#endif
    processTriggers();
  }

  void SampleAndHoldBuffer::process() {
    mopo_float value = input()->source->buffer[0];
    mopo_float* dest = output()->buffer;
    if (value == dest[0])
      return;

    VECTORIZE_LOOP
    for (int i = 0; i < buffer_size_; ++i)
      bufferTick(dest, value, i);
    processTriggers();
  }

  void LinearSmoothBuffer::process() {
    mopo_float new_value = input(kValue)->source->buffer[0];
    mopo_float* dest = output()->buffer;

    if (input(kTrigger)->source->triggered) {
      int trigger_samples = input(kTrigger)->source->trigger_offset;
      int i = 0;

      mopo_float val = last_value_;
      VECTORIZE_LOOP
      for (; i < trigger_samples; ++i)
        dest[i] = val;

      val = new_value;

      VECTORIZE_LOOP
      for (; i < buffer_size_; ++i)
        dest[i] = val;
    }
    else if (last_value_ == new_value &&
             new_value == output()->buffer[0] &&
             new_value == output()->buffer[buffer_size_ - 1] &&
             (buffer_size_ <= 1 || new_value == output()->buffer[buffer_size_ - 2])) {
      last_value_ = new_value;
      return;
    }
    else {
      mopo_float inc = (new_value - last_value_) / buffer_size_;
      mopo_float val = last_value_ + inc;

      VECTORIZE_LOOP
      for (int i = 0; i < buffer_size_; ++i)
        dest[i] = val + i * inc;
    }

    last_value_ = new_value;
    processTriggers();
  }
} // namespace mopo
