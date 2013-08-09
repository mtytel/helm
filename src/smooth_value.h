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
#ifndef SMOOTH_VALUE_H
#define SMOOTH_VALUE_H

#include "value.h"

namespace laf {

  class SmoothValue : public Value {
    public:
      SmoothValue(laf_sample value = 0.0);

      virtual Processor* clone() const { return new SmoothValue(*this); }
      virtual void process();
      void setSampleRate(int sample_rate);

      void set(laf_sample value) { target_value_ = value; }
      void setHard(laf_sample value) {
        Value::set(value);
        target_value_ = value;
      }

    private:
      laf_sample tick();

      laf_sample target_value_;
      laf_sample decay_;
  };
} // namespace laf

#endif // SMOOTH_VALUE_H
