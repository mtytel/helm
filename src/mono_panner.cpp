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

#include "mono_panner.h"

#include "wave.h"

#include <cmath>

#define LEFT_ROTATION 100.325
#define RIGHT_ROTATION 100.125

namespace mopo {

  MonoPanner::MonoPanner() :
      Processor(MonoPanner::kNumInputs, MonoPanner::kNumOutputs) { }

  void MonoPanner::process() {
    MOPO_ASSERT(inputMatchesBufferSize(kAudio));
    MOPO_ASSERT(inputMatchesBufferSize(kPan));

    mopo_float integral;
    for (int i = 0; i < buffer_size_; ++i) {
      mopo_float audio = input(kAudio)->at(i);
      mopo_float pan = input(kPan)->at(i);
      mopo_float left_gain = Wave::fullsin(utils::mod(pan + LEFT_ROTATION,
                                                      &integral));
      mopo_float right_gain = Wave::fullsin(utils::mod(pan + RIGHT_ROTATION,
                                                       &integral));

      output(kLeft)->buffer[i] = audio * left_gain;
      output(kRight)->buffer[i] = audio * right_gain;
    }
  }
} // namespace mopo
