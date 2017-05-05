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

#include "resonance_cancel.h"
#include "biquad_filter.h"

namespace mopo {

  ResonanceCancel::ResonanceCancel() : Processor(kNumInputs, 1, true) { }

  void ResonanceCancel::process() {
    BiquadFilter::Type type = static_cast<BiquadFilter::Type>(
        static_cast<int>(input(kFilterType)->at(0)));

    if (type == BiquadFilter::kLowShelf ||
        type == BiquadFilter::kHighShelf ||
        type == BiquadFilter::kBandShelf) {
      output()->buffer[0] = 1.0;
    }
    else {
      output()->buffer[0] = input(kResonance)->source->buffer[0];
    }
  }
} // namespace mopo
