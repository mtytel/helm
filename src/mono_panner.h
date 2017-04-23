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
#ifndef MONO_PANNER_H
#define MONO_PANNER_H

#include "processor.h"

namespace mopo {

  // A processor that will split a single signal into two signals, left and
  // right. The amplitude of the two outputs is set by the pan amount, ranging
  // from all the way left, all the way right, or equally split.
  class MonoPanner : public Processor {
    public:
      enum Inputs {
        kAudio,
        kPan,
        kNumInputs
      };

      enum Outputs {
        kLeft,
        kRight,
        kNumOutputs
      };

      MonoPanner();
      virtual ~MonoPanner() { }

      virtual Processor* clone() const override {
        return new MonoPanner(*this);
      }

      virtual void process() override;
  };
} // namespace mopo

#endif // MONO_PANNER_H
