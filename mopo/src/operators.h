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

#include "magnitude_lookup.h"
#include "midi_lookup.h"
#include "resonance_lookup.h"
#include "processor.h"

#if defined (__APPLE__)
  #include <Accelerate/Accelerate.h>
  #define USE_APPLE_ACCELERATE
#endif

#define PROCESS_TICK_FUNCTION \
void process() { \
  for (int i = 0; i < buffer_size_; ++i) \
    tick(i); \
}

namespace mopo {

  // A base class for arithmetic operators.
  class Operator : public Processor {
    public:
      Operator(int num_inputs, int num_outputs) :
          Processor(num_inputs, num_outputs) { }

      virtual void process();
      virtual void tick(int i) = 0;

    private:
      Operator() : Processor(0, 0) { }
  };

  // A processor that will clamp a signal output to a given window.
  class Clamp : public Operator {
    public:
      Clamp(mopo_float min = -1, mopo_float max = 1) : Operator(1, 1),
                                                       min_(min), max_(max) { }

      virtual Processor* clone() const { return new Clamp(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = CLAMP(input(0)->at(i), min_, max_);
      }

#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vclipD(input(0)->source->buffer, 1,
                    &min_, &max_,
                    output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif
    private:
      mopo_float min_, max_;
  };

  // A processor that will negate the amplitude of a signal.
  class Negate : public Operator {
    public:
      Negate() : Operator(1, 1) { }

      virtual Processor* clone() const { return new Negate(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = -input(0)->at(i);
      }

#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vnegD(input(0)->source->buffer, 1,
                   output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif 
  };

  // A processor that will invert a signal value by value x -> x^-1.
  class Inverse : public Operator {
    public:
      Inverse() : Operator(1, 1) { }

      virtual Processor* clone() const { return new Inverse(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = 1.0 / input(0)->at(i);
      }

      PROCESS_TICK_FUNCTION
  };

  // A processor that will scale a signal by a given scalar.
  class LinearScale : public Operator {
    public:
      LinearScale(mopo_float scale = 1) : Operator(1, 1), scale_(scale) { }
      virtual Processor* clone() const { return new LinearScale(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = scale_ * input(0)->at(i);
      }

#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vsmulD(input(0)->source->buffer, 1, &scale_,
                    output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif

    private:
      mopo_float scale_;
  };

  // A processor that will convert a stream of midi to a stream of frequencies.
  class MidiScale : public Operator {
    public:
      MidiScale() : Operator(1, 1) { }

      virtual Processor* clone() const { return new MidiScale(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] =
            MidiLookup::centsLookup(CENTS_PER_NOTE * input(0)->at(i));
      }

      PROCESS_TICK_FUNCTION
  };

  // A processor that will convert a stream of magnitudes to a stream of
  // q resonance values.
  class ResonanceScale : public Operator {
    public:
      ResonanceScale() : Operator(1, 1) { }

      virtual Processor* clone() const { return new ResonanceScale(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] =
            ResonanceLookup::qLookup(input(0)->at(i));
      }

      PROCESS_TICK_FUNCTION
  };

  // A processor that will convert a stream of decibals to a stream of
  // magnitudes.
  class MagnitudeScale : public Operator {
    public:
      MagnitudeScale() : Operator(1, 1) { }

      virtual Processor* clone() const { return new MagnitudeScale(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] =
            MagnitudeLookup::magnitudeLookup(input(0)->at(i));
      }

      PROCESS_TICK_FUNCTION
  };

  // A processor that will add two streams together.
  class Add : public Operator {
    public:
      Add() : Operator(2, 1) { }

      virtual Processor* clone() const { return new Add(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = input(0)->at(i) + input(1)->at(i);
      }
#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vaddD(input(0)->source->buffer, 1,
                   input(1)->source->buffer, 1,
                   output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif
  };

  // A processor that will add a variable number of strings together.
  class VariableAdd : public Operator {
    public:
      VariableAdd(int num_inputs) : Operator(num_inputs, 1) { }

      virtual Processor* clone() const { return new VariableAdd(*this); }

      void process();
      inline void tick(int i) {
        int num_inputs = inputs_->size();
        output(0)->buffer[i] = 0.0;
        for (int in = 0; in < num_inputs; ++in)
          output(0)->buffer[i] += input(in)->at(i);
      }
  };

  // A processor that will subtract one stream from another.
  class Subtract : public Operator {
    public:
      Subtract() : Operator(2, 1) { }

      virtual Processor* clone() const { return new Subtract(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = input(0)->at(i) - input(1)->at(i);
      }
#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vsubD(input(0)->source->buffer, 1,
                   input(1)->source->buffer, 1,
                   output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif
  };

  // A processor that will multiply to streams together.
  class Multiply : public Operator {
    public:
      Multiply() : Operator(2, 1) { }

      virtual Processor* clone() const { return new Multiply(*this); }

      inline void tick(int i) {
        output(0)->buffer[i] = input(0)->at(i) * input(1)->at(i);
      }

#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vmulD(input(0)->source->buffer, 1,
                   input(1)->source->buffer, 1,
                   output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif
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
        output(0)->buffer[i] =
            INTERPOLATE(input(kFrom)->at(i),
                        input(kTo)->at(i),
                        input(kFractional)->at(i));
      }

#ifdef USE_APPLE_ACCELERATE
      void process() {
        vDSP_vsbmD(input(kTo)->source->buffer, 1,
                   input(kFrom)->source->buffer, 1,
                   input(kFractional)->source->buffer, 1,
                   output(0)->buffer, 1, buffer_size_);

        vDSP_vaddD(input(kFrom)->source->buffer, 1,
                   output(0)->buffer, 1,
                   output(0)->buffer, 1, buffer_size_);
      }
#else
      PROCESS_TICK_FUNCTION
#endif
  };
} // namespace mopo

#endif // OPERATORS_H
