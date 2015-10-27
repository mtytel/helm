/* Copyright 2013-2015 Matt Tytel
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

#include "fixed_point_oscillator.h"

namespace mopo {

  FixedPointOscillator::FixedPointOscillator() : Processor(kNumInputs, 1) {
    phase_ = 0;
  }

  void FixedPointOscillator::process() {
    int phase_inc = UINT_MAX * input(kPhaseInc)->source->buffer[0];

    int waveform = static_cast<int>(input(kWaveform)->source->buffer[0] + 0.5);

    waveform = CLAMP(waveform, 0, FixedPointWaveLookup::kWhiteNoise - 1);

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, waveform, phase_inc);

      phase_ = 0;
    }
    for (; i < buffer_size_; ++i)
      tick(i, waveform, phase_inc);
  }
} // namespace mopo
