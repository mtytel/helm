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

#include "twytch_common.h"

namespace mopo {

  ValueDetailsLookup::ValueDetailsLookup() {
    int num_parameters = sizeof(parameter_list) / sizeof(ValueDetails);
    for (int i = 0; i < num_parameters; ++i)
      details_lookup_[parameter_list[i].name] = parameter_list[i];
  }

  ValueDetailsLookup Parameters::lookup_;

} // namespace mopo
