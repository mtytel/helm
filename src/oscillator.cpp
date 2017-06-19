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

#include "oscillator.h"

#include <cmath>

namespace mopo {

  Oscillator::Oscillator() : Processor(kNumInputs, kNumOutputs),
                             offset_(0.0), waveform_(Wave::kSin) { }

  void Oscillator::preprocess() {
    int int_wave = static_cast<int>(input(kWaveform)->at(0) + 0.5);
    waveform_ = static_cast<Wave::Type>(int_wave);
  }

  void Oscillator::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kFrequency));
    MOPO_ASSERT(inputMatchesBufferSize(kPhase));

    preprocess();

    int i = 0;
    if (input(kReset)->source->triggered &&
        input(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i);

      offset_ = 0.0;
    }
    for (; i < buffer_size_; ++i)
      tick(i);
  }
} // namespace mopo
