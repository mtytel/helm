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

#pragma once
#ifndef LINEAR_SLOPE_H
#define LINEAR_SLOPE_H

#include "value.h"

namespace mopo {

  // A processor that will slope to the target value over a given amount of
  // time. Useful for portamento or smoothing out values.
  class LinearSlope : public Processor {
    public:
      enum Inputs {
        kTarget,
        kRunSeconds,
        kTriggerJump,
        kNumInputs
      };

      LinearSlope();

      virtual Processor* clone() const { return new LinearSlope(*this); }
      virtual void process();

    private:
      mopo_float tick(int i);

      mopo_float last_value_;
  };
} // namespace mopo

#endif // LINEAR_SLOPE_H
