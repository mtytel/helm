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
#ifndef LADDER_FILTER_H
#define LADDER_FILTER_H

#include "processor.h"

namespace mopo {

  /*
   * This ladder filter implementation is based on the version in:
   * An Improved Virtual Analog Model of the Moog Ladder Filter
   * Authors: Stefano D'Angelo, Vesa Välimäki
   */

  class LadderFilter : public Processor {
    public:
      enum Inputs {
        kAudio,
        kCutoff,
        kResonance,
        kDrive,
        kReset,
        kNumInputs
      };

      LadderFilter();
      virtual ~LadderFilter() { }

      virtual Processor* clone() const { return new LadderFilter(*this); }
      virtual void process();

      void computeCoefficients(mopo_float cutoff);

      inline void tick(int i, mopo_float* dest, const mopo_float* audio_buffer,
                       mopo_float g, mopo_float resonance, mopo_float two_sr);

    private:
      void reset();

      mopo_float current_resonance_, current_drive_;

      double v_[4];
      double delta_v_[4];
      double tanh_v_[4];
      mopo_float g_;
      mopo_float resonance_multiple_;
  };
} // namespace mopo

#endif // LADDER_FILTER_H
