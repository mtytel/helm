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
#ifndef REVERB_H
#define REVERB_H

#include "processor_router.h"

namespace mopo {

  // A comb filter with low pass filtering useful in a reverb processor.
  class Reverb : public ProcessorRouter {
    public:
      enum Inputs {
        kAudio,
        kFeedback,
        kDamping,
        kStereoWidth,
        kWet,
        kNumInputs
      };

      Reverb();
      virtual ~Reverb() { }

      void process() override;

      virtual Processor* clone() const override { return new Reverb(*this); }

    protected:
      Processor* reverb_wet_left_;
      Processor* reverb_wet_right_;

      mopo_float current_dry_;
      mopo_float current_wet_;
  };
} // namespace mopo

#endif // REVERB_H
