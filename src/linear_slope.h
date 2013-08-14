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

#pragma once
#ifndef LINEAR_SLOPE_H
#define LINEAR_SLOPE_H

#include "value.h"

namespace laf {

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
      laf_sample tick(int i);

      laf_sample last_value_;
  };
} // namespace laf

#endif // LINEAR_SLOPE_H
