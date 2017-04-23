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

#include "dc_filter.h"

namespace mopo {

  DcFilter::DcFilter() : Processor(DcFilter::kNumInputs, 1) {
    coefficient_ = 0.0;
    past_in_ = past_out_ = 0.0;
  }

  void DcFilter::process() {
    computeCoefficients();

    const mopo_float* source = input(kAudio)->source->buffer;
    mopo_float* dest = output()->buffer;
    int i = 0;
    if (inputs_->at(kReset)->source->triggered &&
        inputs_->at(kReset)->source->trigger_value == kVoiceReset) {
      int trigger_offset = inputs_->at(kReset)->source->trigger_offset;
      for (; i < trigger_offset; ++i)
        tick(i, dest, source);

      reset();
    }
    for (; i < buffer_size_; ++i)
      tick(i, dest, source);
  }

  void DcFilter::reset() {
    past_in_ = past_out_ = 0.0;
  }

} // namespace mopo
