/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "twytch_oscillators.h"

namespace mopo {

  TwytchOscillators::TwytchOscillators() : Processor(kNumInputs, 2) { }

  void TwytchOscillators::process() {
    Wave::Type waveform1 = static_cast<Wave::Type>(input(kOscillator1Waveform)->source->buffer[0]);
    Wave::Type waveform2 = static_cast<Wave::Type>(input(kOscillator2Waveform)->source->buffer[0]);

    mopo_float base_phase1 = input(kOscillator1PhaseInc)->source->buffer[0];
    mopo_float base_phase2 = input(kOscillator2PhaseInc)->source->buffer[0];

    int i = 0;
    if (input(kReset)->source->triggered) {
      int trigger_offset = input(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, base_phase1, base_phase2, waveform1, waveform2);

      oscillator1_phase_ = 0.0;
      oscillator2_phase_ = 0.0;
      oscillator1_value_ = 0.0;
      oscillator1_value_ = 0.0;
    }
    for (; i < buffer_size_; ++i)
      tick(i, base_phase1, base_phase2, waveform1, waveform2);
  }
} // namespace mopo
