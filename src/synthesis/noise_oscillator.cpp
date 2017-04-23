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
    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i);

      current_noise_value_ = rand() / mopo_float(RAND_MAX);
    }
    for (; i < buffer_size_; ++i)
      tick(i);
  }
} // namespace mopo
