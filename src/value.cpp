/* Copyright 2013-2016 Matt Tytel
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
      output()->buffer[i] = value_;
  }

  void Value::process() {
    output()->clearTrigger();
    if (output()->buffer[0] == value_ &&
        output()->buffer[buffer_size_ - 1] == value_ &&
        !input(kSet)->source->triggered) {
      return;
    }

    if (input(kSet)->source->triggered) {
      int i = 0;
      int offset = input(kSet)->source->trigger_offset;
      for (; i < offset; ++i)
        output()->buffer[i] = value_;

      value_ = input(kSet)->source->trigger_value;

      for (; i < buffer_size_; ++i)
        output()->buffer[i] = value_;

      output()->trigger(value_, input(kSet)->source->trigger_offset);
    }
    else {
      for (int i = 0; i < buffer_size_; ++i)
        output()->buffer[i] = value_;
    }
  }

  void Value::set(mopo_float value) {
    value_ = value;
    for (int i = 0; i < MAX_BUFFER_SIZE; ++i)
      output()->buffer[i] = value_;
  }
} // namespace mopo
