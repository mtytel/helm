/* Copyright 2013 Little IO
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
#ifndef OPERATORS_H
#define OPERATORS_H

#include "processor.h"

namespace mopo {

  class Clamp : public Processor {
    public:
      Clamp(mopo_float min = -1, mopo_float max = 1) : Processor(1, 1),
                                                       min_(min), max_(max) { }

      virtual Processor* clone() const { return new Clamp(*this); }
      void process();

    private:
      mopo_float min_, max_;
  };

  class Negate : public Processor {
    public:
      Negate() : Processor(1, 1) { }

      virtual Processor* clone() const { return new Negate(*this); }
      void process();
  };

  class LinearScale : public Processor {
    public:
      LinearScale(mopo_float scale = 1) : Processor(1, 1), scale_(scale) { }
      virtual Processor* clone() const { return new LinearScale(*this); }
      void process();

    private:
      mopo_float scale_;
  };

  class MidiScale : public Processor {
    public:
      MidiScale() : Processor(1, 1) { }

      virtual Processor* clone() const { return new MidiScale(*this); }
      void process();
  };

  class Add : public Processor {
    public:
      Add() : Processor(2, 1) { }

      virtual Processor* clone() const { return new Add(*this); }
      void process();
  };

  class VariableAdd : public Processor {
    public:
      VariableAdd(int num_inputs) : Processor(num_inputs, 1) { }

      virtual Processor* clone() const { return new VariableAdd(*this); }
      void process();
  };

  class Subtract : public Processor {
    public:
      Subtract() : Processor(2, 1) { }

      virtual Processor* clone() const { return new Subtract(*this); }
      void process();
  };

  class Multiply : public Processor {
    public:
      Multiply() : Processor(2, 1) { }

      virtual Processor* clone() const { return new Multiply(*this); }
      void process();
  };

  class Interpolate : public Processor {
    public:
      enum Inputs {
        kFrom,
        kTo,
        kFractional,
        kNumInputs
      };

      Interpolate() : Processor(kNumInputs, 1) { }

      virtual Processor* clone() const { return new Interpolate(*this); }
      void process();
  };
} // namespace mopo

#endif // OPERATORS_H
