/* Copyright 2013-2017 Matt Tytel
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
#ifndef TRIGGER_OPERATORS_H
#define TRIGGER_OPERATORS_H

#include "processor.h"

namespace mopo {

  class TriggerCombiner : public Processor {
    public:
      TriggerCombiner();

      virtual Processor* clone() const override {
        return new TriggerCombiner(*this);
      }

      void process() override ;
  };

  class TriggerWait : public Processor {
    public:
      enum Inputs {
        kWait,
        kTrigger,
        kNumInputs
      };

      TriggerWait();

      virtual Processor* clone() const override {
        return new TriggerWait(*this);
      }

      void process() override;

    private:
      void waitTrigger(mopo_float trigger_value);
      void sendTrigger(int trigger_offset);

      bool waiting_;
      mopo_float trigger_value_;
  };

  class TriggerFilter : public Processor {
    public:
      enum Inputs {
        kTrigger,
        kNumInputs
      };

      TriggerFilter(mopo_float fitler = 0.0);
      ~TriggerFilter() { }

      virtual Processor* clone() const override {
        return new TriggerFilter(*this);
      }

      void process() override;

    private:
      mopo_float trigger_filter_;
  };

  class TriggerEquals : public Processor {
    public:
      enum Inputs {
        kTrigger,
        kCondition,
        kNumInputs
      };
      TriggerEquals(mopo_float value) : Processor(kNumInputs, 1), value_(value) { }

      virtual Processor* clone() const override {
        return new TriggerEquals(*this);
      }

      void process() override;

    private:
      mopo_float value_;
  };

  class TriggerNonZero : public Processor {
    public:
      enum Inputs {
        kTrigger,
        kCondition,
        kNumInputs
      };
      TriggerNonZero() : Processor(kNumInputs, 1) { }

      virtual Processor* clone() const override {
        return new TriggerNonZero(*this);
      }

      void process() override;
  };

  class LegatoFilter : public Processor {
    public:
      enum Inputs {
        kLegato,
        kTrigger,
        kNumInputs
      };

      enum Outputs {
        kRetrigger,
        kRemain,
        kNumOutputs
      };

      LegatoFilter();

      virtual Processor* clone() const override {
        return new LegatoFilter(*this);
      }

      void process() override;

    private:
      mopo_float last_value_;
  };

  class PortamentoFilter : public Processor {
    public:
      enum Inputs {
        kPortamento,
        kFrequencyTrigger,
        kVoiceTrigger,
        kNumInputs
      };

      enum State {
        kPortamentoOff,
        kPortamentoAuto,
        kPortamentoOn,
        kNumPortamentoStates,
      };

      PortamentoFilter();

      virtual Processor* clone() const override {
        return new PortamentoFilter(*this);
      }

      void process() override;

    private:

      void updateReleased();
      void updateTrigger();
      bool released_;
  };
} // namespace mopo

#endif // TRIGGER_OPERATORS_H
