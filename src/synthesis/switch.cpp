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

#include "switch.h"
#include "utils.h"
#include <cmath>

namespace mopo {

  Switch::Switch(mopo_float value, bool control_rate) : Value(value, control_rate) {
    original_buffer_ = output()->buffer;
  }

  void Switch::destroy() {
    output()->buffer = original_buffer_;
    Value::destroy();
  }

  void Switch::process() {
    int source = (int)input(Value::kSet)->at(0);
    setSource(source);
  }

  void Switch::set(mopo_float value) {
    value_ = value;
    setSource(value);
  }

  inline void Switch::setSource(int source) {
    source = utils::iclamp(source, 0, numInputs() - kNumInputs - 1);
    output()->buffer = input(kNumInputs + source)->source->buffer;
  }
} // namespace mopo
