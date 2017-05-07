/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "noise_oscillator.h"

namespace mopo {

  NoiseOscillator::NoiseOscillator() : Processor(kNumInputs, 1) {
    current_noise_value_ = NOISE_CONSTANT;
  }

  void NoiseOscillator::process() {
    mopo_float amplitude = input(kAmplitude)->source->buffer[0];
    mopo_float* dest = output()->buffer;

    if (amplitude == 0.0) {
      if (dest[0] != 0.0 || dest[buffer_size_ - 1 != 0.0])
        utils::zeroBuffer(dest, buffer_size_);
      return;
    }

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, dest, amplitude);

      current_noise_value_ = rand() / mopo_float(RAND_MAX);
    }
    for (; i < buffer_size_; ++i)
      tick(i, dest, amplitude);
  }
} // namespace mopo
