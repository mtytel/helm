/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TWYTCH_COMMON_H
#define TWYTCH_COMMON_H

#include "mopo.h"
#include "value.h"
#include "operators.h"

#include <map>
#include <string>

namespace mopo {

  const mopo_float MAX_STEPS = 32;
  const int NUM_FORMANTS = 4;

  typedef std::map<std::string, Value*> control_map;
  typedef std::map<char, std::string> midi_learn_map;
  typedef std::map<std::string, Processor*> input_map;
  typedef std::map<std::string, Processor::Output*> output_map;

  const mopo::mopo_float synced_freq_ratios[] = {
    1.0 / 32.0,
    1.0 / 16.0,
    1.0 / 12.0,
    1.0 / 8.0,
    1.0 / 7.0,
    1.0 / 6.0,
    3.0 / 16.0,
    1.0 / 5.0,
    1.0 / 4.0,
    1.0 / 3.0,
    3.0 / 8.0,
    1.0 / 2.0,
    2.0 / 3.0,
    3.0 / 4.0,
    1.0,
    4.0 / 3.0,
    3.0 / 2.0,
    2.0,
    3.0,
    4.0,
    6.0,
    8.0,
    12.0,
    16.0
  };

  struct ModulationConnection {
    ModulationConnection(std::string from, std::string to) :
        source(from), destination(to) { }

    std::string source;
    std::string destination;
    Value amount;
    Multiply modulation_scale;
  };
} // namespace mopo

#endif // TWYTCH_COMMON_H
