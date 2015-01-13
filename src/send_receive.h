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
#ifndef SEND_RECEIVE_H
#define SEND_RECEIVE_H

#include "memory.h"
#include "processor.h"

// TODO(mtytel): This classes don't work with polyphony. Fix that.

namespace mopo {

  class Send;
  class Receive;

  struct MemoryOutput {
    MemoryOutput() : owner(0), memory(0) { }

    const Send* owner;
    Memory* memory;

    inline mopo_float get(mopo_float past) const {
      return memory->get(past);
    }
  };

  struct MemoryInput {
    MemoryInput() : owner(0), source(0) { }

    const Receive* owner;
    const MemoryOutput* source;

    inline mopo_float get(mopo_float past) const {
      return source->get(past);
    }
  };

  class Send : public Processor {
    public:
      Send();

      virtual Processor* clone() const { return new Send(*this); }
      virtual void process();

      inline mopo_float get(mopo_float past) const {
        return memory_.get(past);
      }

      const MemoryOutput* memory_output() const { return memory_output_; }

    protected:
      Memory memory_;

      MemoryOutput* memory_output_;
  };

  class Receive : public Processor {
    public:
      enum Inputs {
        kDependent,
        kDelayTime,
        kNumInputs
      };

      Receive();

      virtual Processor* clone() const { return new Receive(*this); }
      virtual void process();

      void setSend(const Send* send, bool dependent = true);

    protected:
      MemoryInput* memory_input_;
  };
} // namespace mopo

#endif // SEND_RECEIVE_H
