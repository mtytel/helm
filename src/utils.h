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

#include "laf.h"

#define EPSILON 0.000000000001

namespace laf {

  namespace utils {

    bool isSilent(const laf_sample* buffer, int length) {
      for (int i = 0; i < length; ++i) {
        if (buffer[i] >= EPSILON || buffer[i] <= -EPSILON)
          return false;
      }
      return true;
    }
  } // namespace utils
} // namespace laf
