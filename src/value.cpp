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

#include "value.h"

namespace mopo {

  Value::Value(mopo_float value) : Processor(kNumInputs, 1), value_(value) {
    for (int i = 0; i < MAX_BUFFER_SIZE; ++i)
      output(0)->buffer[i] = value_;
  }

  void Value::process() {
    if (output(0)->buffer[0] == value_ &&
        output(0)->buffer[buffer_size_ - 1] == value_ &&
        !input(kSet)->source->triggered) {
      return;
    }

    int i = 0;
    if (input(kSet)->source->triggered) {
      int trigger_offset = input(kSet)->source->trigger_offset;

      for (; i < trigger_offset; ++i)
        output(0)->buffer[i] = value_;

      value_ = input(kSet)->source->trigger_value;
    }

    for (; i < buffer_size_; ++i)
      output(0)->buffer[i] = value_;
  }

  void Value::set(mopo_float value) {
    value_ = value;
    for (int i = 0; i < MAX_BUFFER_SIZE; ++i)
      output(0)->buffer[i] = value_;
  }
} // namespace mopo
