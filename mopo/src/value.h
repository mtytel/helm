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
#ifndef VALUE_H
#define VALUE_H

#include "processor.h"

namespace mopo {

  class Value : public Processor {
    public:
      enum Inputs {
        kSet,
        kNumInputs
      };

      Value(mopo_float value = 0.0);

      virtual Processor* clone() const { return new Value(*this); }
      virtual void process();

      mopo_float value() const { return value_; }
      virtual void set(mopo_float value);

    protected:
      mopo_float value_;
  };
} // namespace mopo

#endif // VALUE_H
