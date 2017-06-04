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

#include "trigger_random.h"

#include <cstdlib>

namespace mopo {

  TriggerRandom::TriggerRandom() : Processor(1, 1, true), value_(0.0) { }

  void TriggerRandom::process() {
    if (input()->source->triggered)
      value_ = 2.0 * rand() / RAND_MAX - 1.0;

    output()->buffer[0] = value_;
  }
} // namespace mopo
