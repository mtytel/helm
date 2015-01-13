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
#ifndef SMOOTH_VALUE_H
#define SMOOTH_VALUE_H

#include "value.h"

namespace mopo {

  class SmoothValue : public Value {
    public:
      SmoothValue(mopo_float value = 0.0);

      virtual Processor* clone() const { return new SmoothValue(*this); }
      virtual void process();

      virtual void setSampleRate(int sample_rate);

      void set(mopo_float value) { target_value_ = value; }
      void setHard(mopo_float value) {
        Value::set(value);
        target_value_ = value;
      }

    private:
      mopo_float tick();

      mopo_float target_value_;
      mopo_float decay_;
  };
} // namespace mopo

#endif // SMOOTH_VALUE_H
