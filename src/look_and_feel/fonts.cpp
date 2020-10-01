/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fonts.h"

Fonts::Fonts() {
  proportional_regular_ = Font(Typeface::createSystemTypefaceFor(
      BinaryData::MontserratLight_ttf, BinaryData::MontserratLight_ttfSize));
  proportional_light_ = Font(Typeface::createSystemTypefaceFor(
      BinaryData::MontserratExtraLight_ttf, BinaryData::MontserratExtraLight_ttfSize));
  monospace_ = Font(Typeface::createSystemTypefaceFor(
      BinaryData::ShareTechMonoRegular_ttf, BinaryData::ShareTechMonoRegular_ttfSize));
}
