/* Copyright 2013 Little IO
 *
 * laf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * laf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with laf.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mono_panner.h"

#include "laf.h"

namespace laf {

  MonoPanner::MonoPanner() :
      Processor(MonoPanner::kNumInputs, MonoPanner::kNumOutputs) { }

  void MonoPanner::process() {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
      laf_float audio = inputs_[kAudio]->at(i);
      laf_float pan = inputs_[kPan]->at(i) + PI / 4;
      laf_float left_gain = cos(pan);
      laf_float right_gain = sin(pan);

      outputs_[kLeft]->buffer[i] = audio * left_gain;
      outputs_[kRight]->buffer[i] = audio * right_gain;
    }
  }
} // namespace laf
