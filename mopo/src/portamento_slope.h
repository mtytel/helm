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
#ifndef PORTAMENTO_SLOPE_H
#define PORTAMENTO_SLOPE_H

#include "value.h"

namespace mopo {

  // A processor that will slope to the target value over a given amount of
  // time. Useful for portamento or smoothing out values.
  class PortamentoSlope : public Processor {
    public:
      enum Inputs {
        kTarget,
        kPortamentoType,
        kNoteNumber,
        kRunSeconds,
        kTriggerJump,
        kTriggerStart,
        kNumInputs
      };

      enum State {
        kPortamentoOff,
        kPortamentoAuto,
        kPortamentoOn,
        kNumPortamentoStates,
      };

      PortamentoSlope();
      virtual ~PortamentoSlope() { }

      virtual Processor* clone() const override {
        return new PortamentoSlope(*this);
      }

      void processTriggers();
      void processBypass(int start);
      virtual void process() override;
      void tick(int i, mopo_float target, mopo_float increment, mopo_float decay);

    private:
      mopo_float last_value_;
  };
} // namespace mopo

#endif // PORTAMENTO_SLOPE_H
