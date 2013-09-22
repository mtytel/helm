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
#ifndef OPERATORS_H
#define OPERATORS_H

#include "processor.h"

namespace laf {

  class UnaryOperator : public Processor {
    public:
      UnaryOperator();
  };

  class BinaryOperator : public Processor {
    public:
      BinaryOperator();
  };

  class Negate : public UnaryOperator {
    public:
      virtual Processor* clone() const { return new Negate(*this); }
      void process();
  };

  class LinearScale : public UnaryOperator {
    public:
      LinearScale(laf_float scale = 1) : scale_(scale) { }
      virtual Processor* clone() const { return new LinearScale(*this); }
      void process();

    private:
      laf_float scale_;
  };

  class MidiScale : public UnaryOperator {
    public:
      virtual Processor* clone() const { return new MidiScale(*this); }
      void process();
  };

  class Add : public BinaryOperator {
    public:
      virtual Processor* clone() const { return new Add(*this); }
      void process();
  };

  class Subtract : public BinaryOperator {
    public:
      virtual Processor* clone() const { return new Subtract(*this); }
      void process();
  };

  class Multiply : public BinaryOperator {
    public:
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

      Interpolate();

      virtual Processor* clone() const { return new Interpolate(*this); }
      void process();
  };
} // namespace laf

#endif // OPERATORS_H
