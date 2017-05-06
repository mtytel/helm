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
#ifndef HELM_LFO_H
#define HELM_LFO_H

#include "processor.h"
#include "wave.h"

namespace mopo {

  // A processor that produces an oscillation stream based on the input
  // frequency, phase, and waveform. You can reset the waveform stream using
  // the reset input.
  class HelmLfo : public Processor {
    public:
      enum Inputs {
        kFrequency,
        kPhase,
        kWaveform,
        kReset,
        kNumInputs
      };

      enum Outputs {
        kValue,
        kOscPhase,
        kNumOutputs
      };

      HelmLfo();

      virtual Processor* clone() const override { return new HelmLfo(*this); }
      void process() override;
      void correctToTime(mopo_float samples);

    protected:
      mopo_float offset_;
      mopo_float last_random_value_;
      mopo_float current_random_value_;
  };
} // namespace mopo

#endif // HELM_LFO_H
