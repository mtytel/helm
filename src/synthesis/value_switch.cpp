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

#include "value_switch.h"
#include "utils.h"
#include <cmath>

namespace mopo {

  ValueSwitch::ValueSwitch(mopo_float value) : cr::Value(value) {
    while (numOutputs() < kNumOutputs)
      addOutput();

    original_buffer_ = output(kSwitch)->buffer;
    enable(false);
  }

  void ValueSwitch::destroy() {
    output(kSwitch)->buffer = original_buffer_;
    cr::Value::destroy();
  }

  void ValueSwitch::set(mopo_float value) {
    cr::Value::set(value);
    setSource(value);
  }

  inline void ValueSwitch::setSource(int source) {
    bool enable_processors = source != 0;
    source = utils::iclamp(source, 0, numInputs() - 1);
    output(kSwitch)->buffer = input(source)->source->buffer;

    for (Processor* processor : processors_)
      processor->enable(enable_processors);
  }
} // namespace mopo
