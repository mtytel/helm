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
#ifndef ALIAS_H
#define ALIAS_H

#include "memory.h"
#include "processor.h"

namespace mopo {

  // A signal delay processor with wet/dry, delay time and feedback controls.
  // Handles fractional delay amounts through interpolation.
  class Alias : public Processor {
    public:
      enum Inputs {
        kAudio,
        kWet,
        kAliasTime,
        kNumInputs
      };

      Alias();

      virtual Processor* clone() const { return new Alias(*this); }
      virtual void process();

    protected:
      mopo_float tick(int i) {
        mopo_float input = inputs_[kAudio]->at(i);
        mopo_float wet = inputs_[kWet]->at(i);
        mopo_float period = inputs_[kAliasTime]->at(i) * sample_rate_;

        static_samples_ += 1.0;
        if (static_samples_ >= period) {
          static_samples_ -= period;
          current_sample_ = input;
        }

        return INTERPOLATE(input, current_sample_, wet);
      }

      mopo_float current_sample_;
      mopo_float static_samples_;
  };
} // namespace mopo

#endif // ALIAS_H
