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

#include "value.h"

namespace laf {

  Value::Value(laf_sample value) : Processor(0, 1), value_(value) {
    process();
  }

  void Value::process() {
    if (outputs_[0]->buffer[0] == value_)
      return;

    for (int i = 0; i < BUFFER_SIZE; ++i)
      outputs_[0]->buffer[i] = value_;
  }

  void Value::set(laf_sample value) {
    value_ = value;
    process();
  }
} // namespace laf
