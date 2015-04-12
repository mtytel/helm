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

#include "twytch_lfo.h"

#include <cmath>

namespace mopo {

  TwytchLfo::TwytchLfo() : Processor(kNumInputs, kNumOutputs), offset_(0.0) { }

  void TwytchLfo::process() {
    static mopo_float integral;
    int num_samples = buffer_size_;

    if (input(kReset)->source->triggered &&
        input(kReset)->source->trigger_value == kVoiceReset) {
      num_samples = buffer_size_ - input(kReset)->source->trigger_offset;
      offset_ = 0.0;
    }
    
    Wave::Type waveform = static_cast<Wave::Type>(static_cast<int>(input(kWaveform)->at(0)));
    mopo_float frequency = input(kFrequency)->at(0);
    mopo_float phase = input(kPhase)->at(0);

    offset_ += num_samples * frequency / sample_rate_;
    offset_ = modf(offset_, &integral);
    mopo_float phased_offset = modf(offset_ + phase, &integral);
    output(kOscPhase)->buffer[0] = phased_offset;
    output(kValue)->buffer[0] = Wave::wave(waveform, phased_offset);
  }
} // namespace mopo
