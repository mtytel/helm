/* Copyright 2013-2014 Little IO
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
#ifndef TWYTCH_STRINGS_H
#define TWYTCH_STRINGS_H

namespace mopo {

  // Container for all the strings we'll use in the UI.
  class TwytchStrings {
    public:
      static const char* filter_strings_[];
      static const char* legato_strings_[];
      static const char* portamento_strings_[];
      static const char* wave_strings_[];
  };
} // namespace mopo

#endif // TWYTCH_STRINGS_H
