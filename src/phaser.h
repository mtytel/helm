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
#ifndef PHASER_H
#define PHASER_H

#include "processor_router.h"

namespace mopo {

  class Interpolate;

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class Phaser : public ProcessorRouter {
    public:
      enum Inputs {
        kAudio,
        kMix,
        kOscFrequency,
        kOscWaveform,
        kFilterCutoffMidi,
        kFilterResonance,
        kSemitoneSweep,
        kReset,
        kNumInputs
      };

      Phaser();

      virtual Processor* clone() const { return new Phaser(*this); }
      virtual void process();
  };
} // namespace mopo

#endif // PHASER_H
