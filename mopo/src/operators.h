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
#ifndef OPERATORS_H
#define OPERATORS_H

#include "midi_lookup.h"
#include "processor.h"

namespace mopo {

  // A base class for arithmetic operators.
  class Operator : public Processor {
    public:
      Operator(int num_inputs, int num_outputs) :
          Processor(num_inputs, num_outputs) { }

      virtual void process();
      virtual void tick(int i) = 0;
  };

  // A processor that will clamp a signal output to a given window.
  class Clamp : public Operator {
    public:
      Clamp(mopo_float min = -1, mopo_float max = 1) : Operator(1, 1),
                                                       min_(min), max_(max) { }

      virtual Processor* clone() const { return new Clamp(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = CLAMP(inputs_[0]->at(i), min_, max_);
      }

    private:
      mopo_float min_, max_;
  };

  // A processor that will invert a signal.
  class Negate : public Operator {
    public:
      Negate() : Operator(1, 1) { }

      virtual Processor* clone() const { return new Negate(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = -inputs_[0]->at(i);
      }
  };

  // A processor that will scale a signal by a given scalar.
  class LinearScale : public Operator {
    public:
      LinearScale(mopo_float scale = 1) : Operator(1, 1), scale_(scale) { }
      virtual Processor* clone() const { return new LinearScale(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = scale_ * inputs_[0]->at(i);
      }

    private:
      mopo_float scale_;
  };

  // A processor that will convert a stream of midi to a stream of frequencies.
  class MidiScale : public Operator {
    public:
      MidiScale() : Operator(1, 1) { }

      virtual Processor* clone() const { return new MidiScale(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] =
            MidiLookup::centsLookup(CENTS_PER_NOTE * inputs_[0]->at(i));
      }
  };

  // A processor that will add two streams together.
  class Add : public Operator {
    public:
      Add() : Operator(2, 1) { }

      virtual Processor* clone() const { return new Add(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = inputs_[0]->at(i) + inputs_[1]->at(i);
      }
  };

  // A processor that will add a variable number of strings together.
  class VariableAdd : public Operator {
    public:
      VariableAdd(int num_inputs) : Operator(num_inputs, 1) { }

      virtual Processor* clone() const { return new VariableAdd(*this); }

      void process();
      inline void tick(int i) {
        int num_inputs = inputs_.size();
        outputs_[0]->buffer[i] = 0.0;
        for (int input = 0; input < num_inputs; ++input)
          outputs_[0]->buffer[i] += inputs_[input]->at(i);
      }
  };

  // A processor that will subtract one stream from another.
  class Subtract : public Operator {
    public:
      Subtract() : Operator(2, 1) { }

      virtual Processor* clone() const { return new Subtract(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = inputs_[0]->at(i) - inputs_[1]->at(i);
      }
  };

  // A processor that will multiply to streams together.
  class Multiply : public Operator {
    public:
      Multiply() : Operator(2, 1) { }

      virtual Processor* clone() const { return new Multiply(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = inputs_[0]->at(i) * inputs_[1]->at(i);
      }
  };

  // A processor that will interpolate two streams by an interpolation stream.
  class Interpolate : public Operator {
    public:
      enum Inputs {
        kFrom,
        kTo,
        kFractional,
        kNumInputs
      };

      Interpolate() : Operator(kNumInputs, 1) { }

      virtual Processor* clone() const { return new Interpolate(*this); }

      inline void tick(int i) {
        outputs_[0]->buffer[i] = INTERPOLATE(inputs_[kFrom]->at(i),
                                             inputs_[kTo]->at(i),
                                             inputs_[kFractional]->at(i));
      }
  };
} // namespace mopo

#endif // OPERATORS_H
